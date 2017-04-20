/*
 * Copyright (C) 2016
 * Ladislav Michl <ladis@linux-mips.org>
 *
 * bootz code:
 * Copyright (C) 2012 Marek Vasut <marek.vasut@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>

#define	LINUX_ARM_ZIMAGE_MAGIC	0x016f2818

struct arm_z_header {
	uint32_t	code[9];
	uint32_t	zi_magic;
	uint32_t	zi_start;
	uint32_t	zi_end;
} __attribute__ ((__packed__));

#if defined(CONFIG_MOCANA_NANOBOOT)
/* cdsxxx */
typedef u32 ubyte4;

extern ubyte4
SB_VERIFY_gModulusLen(void);

extern int
SB_VERIFY(char* data, u32 dataLen);
#endif

int bootz_setup(ulong image, ulong *start, ulong *end)
{
	struct arm_z_header *zi = (struct arm_z_header *)image;

	if (zi->zi_magic != LINUX_ARM_ZIMAGE_MAGIC) {
#ifndef CONFIG_SPL_FRAMEWORK
		puts("Bad Linux ARM zImage magic!\n");
#endif
		return 1;
	}

	*start = zi->zi_start;
	*end = zi->zi_end;

/* cdsxxx */
#if defined(CONFIG_MOCANA_NANOBOOT)

	int modulusLen = SB_VERIFY_gModulusLen();

	debug("image_addr= %lx start_addr= %lx end_addr= %lx\n", image, *start, *end);

	printf("***************************************************\n");
	printf("** Mocana NanoBoot: Verifying Linux Kernel Image **\n");
	printf("***************************************************\n\n");

	if (0 == SB_VERIFY((char*)(uintptr_t)image, *end-*start+modulusLen))
	{
		printf("*************************************\n");
		printf("** Mocana NanoBoot: Verify Success **\n");
		printf("*************************************\n\n");

	}
	else
	{
		printf("*************************************\n");
	    printf("** Mocana NanoBoot: Verify Failure **\n");
		printf("*************************************\n\n");
	    return 1;
	}
/* cdsxxx */
#endif

#ifndef CONFIG_SPL_FRAMEWORK
	printf("Kernel image @ %#08lx [ %#08lx - %#08lx ]\n",
	       image, *start, *end);
#endif

	return 0;
}
