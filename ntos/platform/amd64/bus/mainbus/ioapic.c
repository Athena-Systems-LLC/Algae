/*
 * ALGAE I/O APIC interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ex/trace.h>
#include <ke/bugCheck.h>
#include <ke/defs.h>
#include <ke/mmio.h>
#include <acpi/acpi.h>

#define traceInfo(fmt, ...) \
    exTrace(EX_TRACE_INFO, "ioapic: " fmt, __VA_ARGS__)

/* Register access window */
#define IOREGSEL 0x00
#define IOWIN    0x10

/* Register offsets */
#define REG_IOAPICID    0x00
#define REG_IOAPICVER   0x01
#define REG_IOAPICARB   0x02
#define REG_IOREDTBL    0x10

static ULONG_PTR mmioBase;

typedef union {
    struct {
        UCHAR vector;
        UCHAR delmod          : 3;
        UCHAR destmod         : 1;
        UCHAR delivs          : 1;
        UCHAR intpol          : 1;
        UCHAR remoteIrr       : 1;
        UCHAR triggerMode     : 1;
        UCHAR interruptMask   : 1;
        UQUAD reserved       : 39;
        UCHAR dest_field;
    };
    UQUAD value;
} IOAPIC_REDENTRY;

/*
 * Read a 32-bit value from an I/O APIC
 * register
 *
 * @reg: Register to read from
 */
static ULONG
ioApicRead32(USHORT reg)
{
    ULONG *iowin, *ioregsel;

    ioregsel = PTR_OFFSET(mmioBase, IOREGSEL);
    iowin = PTR_OFFSET(mmioBase, IOWIN);

    mmioWrite32(ioregsel, reg);
    return mmioRead32(iowin);
}

/*
 * Write a 32-bit value to an I/O APIC register
 *
 * @reg: Register to write to
 * @val: Value to write
 */
static void
ioApicWrite32(USHORT reg, ULONG val)
{
    ULONG *iowin, *ioregsel;

    ioregsel = PTR_OFFSET(mmioBase, IOREGSEL);
    iowin = PTR_OFFSET(mmioBase, IOWIN);

    mmioWrite32(ioregsel, reg);
    mmioWrite32(iowin, val);
}

/*
 * Reads an I/O APIC redirection entry.
 *
 * @entry: Entry variable to read into.
 * @index: Index to read.
 */
static void
ioApicReadRedentry(IOAPIC_REDENTRY *entry, UCHAR index)
{
    ULONG lo;
    ULONG hi;

    lo = ioApicRead32(REG_IOREDTBL + index * 2);
    hi = ioApicRead32(REG_IOREDTBL + index * 2 + 1);
    entry->value = ((UQUAD)hi << 32) | lo;
}

/*
 * Writes an I/O APIC redirection entry.
 *
 * @entry: Entry variable to write.
 * @index: Index to write to.
 */
static void
ioApicWriteRedentry(IOAPIC_REDENTRY *entry, UCHAR index)
{
    ioApicWrite32(REG_IOREDTBL + index * 2, (ULONG)entry->value);
    ioApicWrite32(REG_IOREDTBL + index * 2 + 1, (ULONG)(entry->value >> 32));
}

void
kiIoApicMaskGsi(UCHAR gsi, BOOLEAN mask)
{
    IOAPIC_REDENTRY entry;

    ioApicReadRedentry(&entry, gsi);
    entry.interruptMask = mask;
    ioApicWriteRedentry(&entry, gsi);
}

static int
ioApicGetAddrCb(APIC_HEADER *hdr, USIZE arg)
{
    IO_APIC *ioApic = (IO_APIC *)hdr;

    mmioBase = ioApic->ioapicAddr;
    return 0;
}

void
kiIoApicInit(void)
{
    ULONG idReg, ioApicId;
    ULONG verReg, gsiCount;
    int error;

    error = acpiReadMadt(
        APIC_TYPE_IO_APIC,
        ioApicGetAddrCb,
        0
    );

    if (error < 0) {
        keBugCheck("failed to query I/O APIC base\n");
    }

    idReg = ioApicRead32(REG_IOAPICID);
    ioApicId = (idReg >> 24) & 0xF;
    traceInfo("detected I/O APIC %d @ mainbus\n", ioApicId);

    /* Get the number of input pins */
    verReg = ioApicRead32(REG_IOAPICVER);
    gsiCount = ((verReg >> 16) & 0xFF) + 1;
    traceInfo("detected %d input pins for I/O APIC %d\n", gsiCount, ioApicId);

    /* Mask everything */
    for (int i = 0; i < gsiCount; ++i) {
        kiIoApicMaskGsi(i, 1);
    }
}
