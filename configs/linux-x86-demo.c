/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for linux inmate, 1 CPU, ~60 MB RAM, 1 serial port
 *
 * Copyright (c) Siemens AG, 2013-2015
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <linux/types.h>
#include <jailhouse/cell-config.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

struct {
	struct jailhouse_cell_desc cell;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[4];
	struct jailhouse_cache cache_regions[1];
	struct jailhouse_irqchip irqchips[1];
	__u8 pio_bitmap[0x2000];
	struct jailhouse_pci_device pci_devices[1];
} __attribute__((packed)) config = {
	.cell = {
		.signature = JAILHOUSE_CELL_DESC_SIGNATURE,
		.name = "linux-x86-demo",
		.flags = JAILHOUSE_CELL_PASSIVE_COMMREG,

		.cpu_set_size = sizeof(config.cpus),
		.num_memory_regions = ARRAY_SIZE(config.mem_regions),
		.num_cache_regions = ARRAY_SIZE(config.cache_regions),
		.num_irqchips = ARRAY_SIZE(config.irqchips),
		.pio_bitmap_size = ARRAY_SIZE(config.pio_bitmap),
		.num_pci_devices = ARRAY_SIZE(config.pci_devices),
		.num_pci_caps = 0,
	},

	.cpus = {
		0xe,
	},

	.mem_regions = {
		/* low RAM */ {
			.phys_start = 0x3b600000,
			.virt_start = 0,
			.size = 0x00100000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA |
				JAILHOUSE_MEM_LOADABLE,
		},
		/* communication region */ {
			.virt_start = 0x00900000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_COMM_REGION,
		},
		/* RAM */ {
			.phys_start = 0x3f200000,
			.virt_start = 0x3f200000,
			.size = 0xddf000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA,
		},
		/* ACPI */ {
			.phys_start = 0x3ffdf000,
			.virt_start = 0x3ffdf000,
			.size = 0x30000,
			.flags = JAILHOUSE_MEM_READ,
		},
		/* high RAM */ {
			.phys_start = 0x3b700000,
			.virt_start = 0x00200000,
			.size = 0x3aff000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA |
				JAILHOUSE_MEM_LOADABLE,
		},
		/* IVSHMEM shared memory region */
		{
			.phys_start = 0x3f1ff000,
			.virt_start = 0x3f1ff000,
			.size = 0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_ROOTSHARED,
		},
	},

	.cache_regions = {
		{
			.start = 0,
			.size = 2,
			.type = JAILHOUSE_CACHE_L3,
		},
	},
	.irqchips = {
		/* IOAPIC */ {
			.address = 0xfec00000,
			.id = 0xff01,
			.pin_bitmap = 0x000200, /* ACPI IRQ */
		},
	},
	.pio_bitmap = {
		[     0/8 ...  0x2f7/8] = -1,
		[ 0x2f8/8 ...  0x2ff/8] = 0, /* serial2 */
		[ 0x300/8 ...  0x3f7/8] = -1,
		[ 0x3f8/8 ...  0x3ff/8] = 0,  /* com1 - rtems */
		[ 0x400/8 ...  0x5ff/8] = -1,
		[ 0x600/8 ...  0x607/8] = 0xf0, /* acpi-evt */
		[ 0x608/8 ...  0x7ff/8] = -1,
		[ 0x800/8 ...  0x807/8] = 0xf0 /* apci-pm1a */,
		[ 0x808/8 ...  0x83f/8] = 0,
		[ 0x840/8 ...  0xcf7/8] = -1,
		[ 0xcf8/8 ...  0xcff/8] = 0,  /* PCI devices */
		[ 0xd00/8 ... 0xb007/8] = -1,
		[0xb008/8 ... 0xb0ff/8] = 0, /* QEMU PM Timer */
		[0xb100/8 ... 0xbfff/8] = -1,
		[0xc000/8 ... 0xc0ff/8] = 0, /* PCI devices */
		[0xc100/8 ... 0xdfff/8] = -1,
		[0xe000/8 ... 0xe017/8] = 0, /* OXPCIe952 serial1 */
		[0xe018/8 ... 0xffff/8] = -1,
	},

	.pci_devices = {
		{
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 0x0,
			.bdf = (0x0f<<3),
			.bar_mask = {
				0xffffff00, 0xffffffff, 0x00000000,
				0x00000000, 0xffffffe0, 0xffffffff,
			},
			.shmem_region = 3,
			.num_msix_vectors = 1,
		},
	},
};
