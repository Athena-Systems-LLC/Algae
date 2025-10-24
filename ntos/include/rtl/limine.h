/* BSD Zero Clause License */

/* Copyright (C) 2022-2023 mintsuki and contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _LIMINE_H
#define _LIMINE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <ke/types.h>

/* Misc */

#ifdef LIMINE_NO_POINTERS
#  define LIMINE_PTR(TYPE) USIZE
#else
#  define LIMINE_PTR(TYPE) TYPE
#endif

#ifdef __GNUC__
#  define LIMINE_DEPRECATED __attribute__((__deprecated__))
#  define LIMINE_DEPRECATED_IGNORE_START \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#  define LIMINE_DEPRECATED_IGNORE_END \
    _Pragma("GCC diagnostic pop")
#else
#  define LIMINE_DEPRECATED
#  define LIMINE_DEPRECATED_IGNORE_START
#  define LIMINE_DEPRECATED_IGNORE_END
#endif

#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

struct limine_uuid {
    ULONG a;
    USHORT b;
    USHORT c;
    UCHAR d[8];
};

#define LIMINE_MEDIA_TYPE_GENERIC 0
#define LIMINE_MEDIA_TYPE_OPTICAL 1
#define LIMINE_MEDIA_TYPE_TFTP 2

struct limine_file {
    USIZE revision;
    LIMINE_PTR(void *) address;
    USIZE size;
    LIMINE_PTR(char *) path;
    LIMINE_PTR(char *) cmdline;
    ULONG media_type;
    ULONG unused;
    ULONG tftp_ip;
    ULONG tftp_port;
    ULONG partition_index;
    ULONG mbr_disk_id;
    struct limine_uuid gpt_disk_uuid;
    struct limine_uuid gpt_part_uuid;
    struct limine_uuid part_uuid;
};

/* Boot info */

#define LIMINE_BOOTLOADER_INFO_REQUEST { LIMINE_COMMON_MAGIC, 0xf55038d8e2a1202f, 0x279426fcf5f59740 }

struct limine_bootloader_info_response {
    USIZE revision;
    LIMINE_PTR(char *) name;
    LIMINE_PTR(char *) version;
};

struct limine_bootloader_info_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_bootloader_info_response *) response;
};

/* Stack size */

#define LIMINE_STACK_SIZE_REQUEST { LIMINE_COMMON_MAGIC, 0x224ef0460a8e8926, 0xe1cb0fc25f46ea3d }

struct limine_stack_size_response {
    USIZE revision;
};

struct limine_stack_size_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_stack_size_response *) response;
    USIZE stack_size;
};

/* HHDM */

#define LIMINE_HHDM_REQUEST { LIMINE_COMMON_MAGIC, 0x48dcf1cb8ad2b852, 0x63984e959a98244b }

struct limine_hhdm_response {
    USIZE revision;
    USIZE offset;
};

struct limine_hhdm_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_hhdm_response *) response;
};

/* Framebuffer */

#define LIMINE_FRAMEBUFFER_REQUEST { LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b }

#define LIMINE_FRAMEBUFFER_RGB 1

struct limine_video_mode {
    USIZE pitch;
    USIZE width;
    USIZE height;
    USHORT bpp;
    UCHAR memory_model;
    UCHAR red_mask_size;
    UCHAR red_mask_shift;
    UCHAR green_mask_size;
    UCHAR green_mask_shift;
    UCHAR blue_mask_size;
    UCHAR blue_mask_shift;
};

struct limine_framebuffer {
    LIMINE_PTR(void *) address;
    USIZE width;
    USIZE height;
    USIZE pitch;
    USHORT bpp;
    UCHAR memory_model;
    UCHAR red_mask_size;
    UCHAR red_mask_shift;
    UCHAR green_mask_size;
    UCHAR green_mask_shift;
    UCHAR blue_mask_size;
    UCHAR blue_mask_shift;
    UCHAR unused[7];
    USIZE edid_size;
    LIMINE_PTR(void *) edid;
    /* Response revision 1 */
    USIZE mode_count;
    LIMINE_PTR(struct limine_video_mode **) modes;
};

struct limine_framebuffer_response {
    USIZE revision;
    USIZE framebuffer_count;
    LIMINE_PTR(struct limine_framebuffer **) framebuffers;
};

struct limine_framebuffer_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_framebuffer_response *) response;
};

/* Terminal */

#define LIMINE_TERMINAL_REQUEST { LIMINE_COMMON_MAGIC, 0xc8ac59310c2b0844, 0xa68d0c7265d38878 }

#define LIMINE_TERMINAL_CB_DEC 10
#define LIMINE_TERMINAL_CB_BELL 20
#define LIMINE_TERMINAL_CB_PRIVATE_ID 30
#define LIMINE_TERMINAL_CB_STATUS_REPORT 40
#define LIMINE_TERMINAL_CB_POS_REPORT 50
#define LIMINE_TERMINAL_CB_KBD_LEDS 60
#define LIMINE_TERMINAL_CB_MODE 70
#define LIMINE_TERMINAL_CB_LINUX 80

#define LIMINE_TERMINAL_CTX_SIZE ((USIZE)(-1))
#define LIMINE_TERMINAL_CTX_SAVE ((USIZE)(-2))
#define LIMINE_TERMINAL_CTX_RESTORE ((USIZE)(-3))
#define LIMINE_TERMINAL_FULL_REFRESH ((USIZE)(-4))

/* Response revision 1 */
#define LIMINE_TERMINAL_OOB_OUTPUT_GET ((USIZE)(-10))
#define LIMINE_TERMINAL_OOB_OUTPUT_SET ((USIZE)(-11))

#define LIMINE_TERMINAL_OOB_OUTPUT_OCRNL (1 << 0)
#define LIMINE_TERMINAL_OOB_OUTPUT_OFDEL (1 << 1)
#define LIMINE_TERMINAL_OOB_OUTPUT_OFILL (1 << 2)
#define LIMINE_TERMINAL_OOB_OUTPUT_OLCUC (1 << 3)
#define LIMINE_TERMINAL_OOB_OUTPUT_ONLCR (1 << 4)
#define LIMINE_TERMINAL_OOB_OUTPUT_ONLRET (1 << 5)
#define LIMINE_TERMINAL_OOB_OUTPUT_ONOCR (1 << 6)
#define LIMINE_TERMINAL_OOB_OUTPUT_OPOST (1 << 7)

LIMINE_DEPRECATED_IGNORE_START

struct LIMINE_DEPRECATED limine_terminal;

typedef void (*limine_terminal_write)(struct limine_terminal *, const char *, USIZE);
typedef void (*limine_terminal_callback)(struct limine_terminal *, USIZE, USIZE, USIZE, USIZE);

struct LIMINE_DEPRECATED limine_terminal {
    USIZE columns;
    USIZE rows;
    LIMINE_PTR(struct limine_framebuffer *) framebuffer;
};

struct LIMINE_DEPRECATED limine_terminal_response {
    USIZE revision;
    USIZE terminal_count;
    LIMINE_PTR(struct limine_terminal **) terminals;
    LIMINE_PTR(limine_terminal_write) write;
};

struct LIMINE_DEPRECATED limine_terminal_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_terminal_response *) response;
    LIMINE_PTR(limine_terminal_callback) callback;
};

LIMINE_DEPRECATED_IGNORE_END

/* 5-level paging */

#define LIMINE_5_LEVEL_PAGING_REQUEST { LIMINE_COMMON_MAGIC, 0x94469551da9b3192, 0xebe5e86db7382888 }

struct limine_5_level_paging_response {
    USIZE revision;
};

struct limine_5_level_paging_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_5_level_paging_response *) response;
};

/* SMP */

#define LIMINE_SMP_REQUEST { LIMINE_COMMON_MAGIC, 0x95a67b819a1b857e, 0xa0b61b723b6a73e0 }

struct limine_smp_info;

typedef void (*limine_goto_address)(struct limine_smp_info *);

#if defined (__x86_64__) || defined (__i386__)

#define LIMINE_SMP_X2APIC (1 << 0)

struct limine_smp_info {
    ULONG processor_id;
    ULONG lapic_id;
    USIZE reserved;
    LIMINE_PTR(limine_goto_address) goto_address;
    USIZE extra_argument;
};

struct limine_smp_response {
    USIZE revision;
    ULONG flags;
    ULONG bsp_lapic_id;
    USIZE cpu_count;
    LIMINE_PTR(struct limine_smp_info **) cpus;
};

#elif defined (__aarch64__)

struct limine_smp_info {
    ULONG processor_id;
    ULONG gic_iface_no;
    USIZE mpidr;
    USIZE reserved;
    LIMINE_PTR(limine_goto_address) goto_address;
    USIZE extra_argument;
};

struct limine_smp_response {
    USIZE revision;
    ULONG flags;
    USIZE bsp_mpidr;
    USIZE cpu_count;
    LIMINE_PTR(struct limine_smp_info **) cpus;
};

#else
#error Unknown architecture
#endif

struct limine_smp_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_smp_response *) response;
    USIZE flags;
};

/* Memory map */

#define LIMINE_MEMMAP_REQUEST { LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62 }

#define LIMINE_MEMMAP_USABLE                 0
#define LIMINE_MEMMAP_RESERVED               1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE       2
#define LIMINE_MEMMAP_ACPI_NVS               3
#define LIMINE_MEMMAP_BAD_MEMORY             4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULES     6
#define LIMINE_MEMMAP_FRAMEBUFFER            7

struct limine_memmap_entry {
    USIZE base;
    USIZE length;
    USIZE type;
};

struct limine_memmap_response {
    USIZE revision;
    USIZE entry_count;
    LIMINE_PTR(struct limine_memmap_entry **) entries;
};

struct limine_memmap_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_memmap_response *) response;
};

/* Entry point */

#define LIMINE_ENTRY_POINT_REQUEST { LIMINE_COMMON_MAGIC, 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a }

typedef void (*limine_entry_point)(void);

struct limine_entry_point_response {
    USIZE revision;
};

struct limine_entry_point_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_entry_point_response *) response;
    LIMINE_PTR(limine_entry_point) entry;
};

/* Kernel File */

#define LIMINE_KERNEL_FILE_REQUEST { LIMINE_COMMON_MAGIC, 0xad97e90e83f1ed67, 0x31eb5d1c5ff23b69 }

struct limine_kernel_file_response {
    USIZE revision;
    LIMINE_PTR(struct limine_file *) kernel_file;
};

struct limine_kernel_file_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_kernel_file_response *) response;
};

/* Module */

#define LIMINE_MODULE_REQUEST { LIMINE_COMMON_MAGIC, 0x3e7e279702be32af, 0xca1c4f3bd1280cee }

#define LIMINE_INTERNAL_MODULE_REQUIRED (1 << 0)

struct limine_internal_module {
    LIMINE_PTR(const char *) path;
    LIMINE_PTR(const char *) cmdline;
    USIZE flags;
};

struct limine_module_response {
    USIZE revision;
    USIZE module_count;
    LIMINE_PTR(struct limine_file **) modules;
};

struct limine_module_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_module_response *) response;

    /* Request revision 1 */
    USIZE internal_module_count;
    LIMINE_PTR(struct limine_internal_module **) internal_modules;
};

/* RSDP */

#define LIMINE_RSDP_REQUEST { LIMINE_COMMON_MAGIC, 0xc5e77b6b397e7b43, 0x27637845accdcf3c }

struct limine_rsdp_response {
    USIZE revision;
    LIMINE_PTR(void *) address;
};

struct limine_rsdp_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_rsdp_response *) response;
};

/* SMBIOS */

#define LIMINE_SMBIOS_REQUEST { LIMINE_COMMON_MAGIC, 0x9e9046f11e095391, 0xaa4a520fefbde5ee }

struct limine_smbios_response {
    USIZE revision;
    LIMINE_PTR(void *) entry_32;
    LIMINE_PTR(void *) entry_64;
};

struct limine_smbios_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_smbios_response *) response;
};

/* EFI system table */

#define LIMINE_EFI_SYSTEM_TABLE_REQUEST { LIMINE_COMMON_MAGIC, 0x5ceba5163eaaf6d6, 0x0a6981610cf65fcc }

struct limine_efi_system_table_response {
    USIZE revision;
    LIMINE_PTR(void *) address;
};

struct limine_efi_system_table_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_efi_system_table_response *) response;
};

/* Boot time */

#define LIMINE_BOOT_TIME_REQUEST { LIMINE_COMMON_MAGIC, 0x502746e184c088aa, 0xfbc5ec83e6327893 }

struct limine_boot_time_response {
    USIZE revision;
    QUAD boot_time;
};

struct limine_boot_time_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_boot_time_response *) response;
};

/* Kernel address */

#define LIMINE_KERNEL_ADDRESS_REQUEST { LIMINE_COMMON_MAGIC, 0x71ba76863cc55f63, 0xb2644a48c516a487 }

struct limine_kernel_address_response {
    USIZE revision;
    USIZE physical_base;
    USIZE virtual_base;
};

struct limine_kernel_address_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_kernel_address_response *) response;
};

/* Device Tree Blob */

#define LIMINE_DTB_REQUEST { LIMINE_COMMON_MAGIC, 0xb40ddb48fb54bac7, 0x545081493f81ffb7 }

struct limine_dtb_response {
    USIZE revision;
    LIMINE_PTR(void *) dtb_ptr;
};

struct limine_dtb_request {
    USIZE id[4];
    USIZE revision;
    LIMINE_PTR(struct limine_dtb_response *) response;
};

#ifdef __cplusplus
}
#endif

#endif
