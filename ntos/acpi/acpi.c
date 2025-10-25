/*
 * ALGAE common ACPI API
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <mm/vm.h>
#include <ex/trace.h>
#include <rtl/string.h>
#include <ke/bootParam.h>
#include <ke/bugCheck.h>
#include <ke/types.h>

static ACPI_RSDP *rsdp;
static ACPI_ROOT_SDT *sdt;
static USIZE sdtLen;

/*
 * Compute the checksum of a header and return
 * zero if valid
 */
static int
acpiChecksum(ACPI_HEADER *hdr)
{
    UCHAR sum = 0;

    for (int i = 0; i < hdr->length; ++i) {
        sum += ((char *)hdr)[i];
    }

    return sum == 0 ? 0 : -1;
}

void *
acpiQueryTable(const char *signature)
{
    ACPI_HEADER *hdr;
    USIZE sigLen = sizeof(hdr->signature);

    for (USIZE i = 0; i < sdtLen; ++i) {
        hdr = (ACPI_HEADER *)PHYS_TO_VIRT(sdt->table[i]);
        exTrace(EX_TRACE_INFO, "%s\n", hdr->signature);
        if (rtlMemcmp(hdr->signature, signature, sigLen) == 0) {
            return (void *)hdr;
        }
    }

    return NULL;
}

void
acpiInit(void)
{
    struct bootParams bootParams;
    int error;

    error = keGetBootParams(&bootParams, 0);
    if (error < 0) {
        keBugCheck("acpi: failed to get boot params\n");
    }

    rsdp = bootParams.acpiRsdp;
    exTrace(EX_TRACE_INFO, "acpi: rsdp rev %d\n", rsdp->revision);

    /*
     * We are using earlier revisions than 2, the RSDT is
     * whats to be used, otherwise we should use the XSDT.
     */
    if (rsdp->revision < 2) {
        sdt = PHYS_TO_VIRT(rsdp->rsdtAddr);
    } else {
        sdt = PHYS_TO_VIRT(rsdp->xsdtAddr);
    }

    sdtLen = (sdt->hdr.length - sizeof(sdt->hdr)) / 4;
}
