/*
 * ALGAE Program Loader
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ex/loader.h>
#include <ke/types.h>
#include <ke/defs.h>
#include <ke/bootPack.h>
#include <ex/trace.h>
#include <rtl/pe.h>
#include <rtl/string.h>
#include <ntstatus.h>

#define SECT_HDR_LEN 40

#define traceInfo(...) \
    exTrace(EX_TRACE_INFO, "loader: " __VA_ARGS__)

#define traceErr(...) \
    exTrace(EX_TRACE_ERR, "loader: " __VA_ARGS__)


/*
 * Get the current machine type
 *
 * XXX: When porting NTOS to a new architecture, it must be
 *      added to this list for the loader to work
 */
static USHORT
peGetMachineType(void)
{
#if defined(__x86_64__)
    return IMAGE_FILE_MACHINE_AMD64;
#else
    return IMAGE_FILE_MACHINE_UNKNOWN;
#endif
}

/*
 * Verify that the PE header is valid
 *
 * @hdr: Header to verify
 *
 * Returns the PE header from the DOS header if valid,
 * otherwise a value of NULL if invalid.
 */
static IMAGE_PE_HEADER *
peVerifyHeader(IMAGE_DOS_HEADER *hdr)
{
    IMAGE_PE_HEADER *peHdr;

    if (hdr->e_magic != 0x5A4D) {
        return NULL;
    }

    /* Ensure the magic is correct */
    peHdr = PTR_OFFSET(hdr, hdr->e_lfanew);
    if (rtlMemcmp(&peHdr->e_magic, "PE\0\0", 4) != 0) {
        return NULL;
    }

    if (peHdr->e_machine != peGetMachineType()) {
        return NULL;
    }

    return peHdr;
}

static NTSTATUS
loadPe64(IMAGE_PE_HEADER *peHdr)
{
    IMAGE_SECTION_HEADER *sect;
    USIZE off;

    off = sizeof(IMAGE_PE_HEADER) + peHdr->e_opthdr_sz;
    sect = PTR_OFFSET(peHdr, off);

    for (int i = 0; i < peHdr->e_numsect; ++i) {
        traceInfo("discovered section \"%s\"\n", sect->name);
        sect = PTR_OFFSET(sect, SECT_HDR_LEN);
    }
    return STATUS_SUCCESS;
}

NTSTATUS
exLoadFromBootPack(const CHAR *path, LOADER_PROGRAM *result)
{
    CHAR *rawData;
    IMAGE_DOS_HEADER *hdr;
    IMAGE_PE_HEADER *peHdr;

    if (path == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    /* Attempt to load the file */
    rawData = keBootPackGet(path);
    if (rawData == NULL) {
        return STATUS_FILE_UNAVL;
    }

    /* Get the PE header */
    hdr = (IMAGE_DOS_HEADER *)rawData;
    peHdr = peVerifyHeader(hdr);
    if (peHdr == NULL) {
        traceErr("failed to load \"%s\"\n", path);
        return STATUS_PROC_NOEXEC;
    }

    loadPe64(peHdr);
    return STATUS_SUCCESS;
}
