/*
 * Copyright (c) 2018 M4xw
 * Copyright (c) 2018 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include "ianos/ianos.h"
#include "libs/elfload/elfload.h"

#define IRAM_LIB_ADDR 0x4002B000
#define DRAM_LIB_ADDR 0xE0000000

extern heap_t _heap;

extern void *sd_file_read(char *path);
extern bool sd_mount();
extern void sd_unmount();

void *elfBuf = NULL;
void *fileBuf = NULL;

static void _ianos_call_ep(moduleEntrypoint_t entrypoint, void *moduleConfig)
{
	bdkParams_t bdkParameters = (bdkParams_t)malloc(sizeof(struct _bdkParams_t));
	bdkParameters->gfxCon = &g_gfx_con;
	bdkParameters->gfxCtx = &g_gfx_ctxt;
	bdkParameters->memcpy = (memcpy_t)&memcpy;
	bdkParameters->memset = (memset_t)&memset;
	bdkParameters->sharedHeap = &_heap;

	entrypoint(moduleConfig, bdkParameters);
}

static void *_ianos_alloc_cb(el_ctx *ctx, Elf_Addr phys, Elf_Addr virt, Elf_Addr size)
{
	(void)ctx;
	(void)phys;
	(void)size;
	return (void *)virt;
}

static bool _ianos_read_cb(el_ctx *ctx, void *dest, size_t numberBytes, size_t offset)
{
	(void)ctx;

	memcpy(dest, fileBuf + offset, numberBytes);

	return true;
}

void ianos_print_error(int errorno)
{
	switch (errorno)
	{
	case 1:
		gfx_printf(&g_gfx_con, "Can't find library!\n");
		break;
	case 2:
		gfx_printf(&g_gfx_con, "Cant init ELF context!\n");
		break;
	case 3:
		gfx_printf(&g_gfx_con, "Cant alloc memory!\n");
		break;
	case 4:
		gfx_printf(&g_gfx_con, "Error loading ELF!\n");
		break;
	case 5:
		gfx_printf(&g_gfx_con, "Error relcating ELF!\n");
		break;
	}
}

//TODO: Support shared libraries.
int ianos_loader(bool sdmount, char *path, elfType_t type, void *moduleConfig)
{
	int res = 0;

	if (sdmount)
	{
		if (!sd_mount())
		{
			res = 0xFFFF;
			goto elfLoadFinalOut;
		}
	}

	fileBuf = sd_file_read(path);

	if (sdmount)
		sd_unmount();

	if (!fileBuf)
	{
		res = 1;
		goto elfLoadFinalOut;
	}


	el_ctx ctx;
	ctx.pread = _ianos_read_cb;

	if (el_init(&ctx))
	{
		res = 2;
		goto elfLoadFinalOut;
	}

	// Set our relocated library's buffer.
	switch (type & 0xFFFF)
	{
	case EXEC_ELF:
	case AR64_ELF:
		elfBuf = (void *)DRAM_LIB_ADDR;
		sd_unmount();
		break;
	default:
		elfBuf = memalign(ctx.align, ctx.memsz);
	}

	if (!elfBuf)
	{
		res = 3;
		goto elfLoadFinalOut;
	}

	// Load and relocate library.
	ctx.base_load_vaddr = ctx.base_load_paddr = (uintptr_t)elfBuf;
	if (el_load(&ctx, _ianos_alloc_cb))
	{
		res = 4;
		goto elfFreeOut;
	}

	if (el_relocate(&ctx))
	{
		res = 5;
		goto elfFreeOut;
	}

	// Launch.
	uintptr_t epaddr = ctx.ehdr.e_entry + (uintptr_t)elfBuf;
	moduleEntrypoint_t ep = (moduleEntrypoint_t)epaddr;

	_ianos_call_ep(ep, moduleConfig);

elfFreeOut:
	if ((u32)elfBuf >= 0x90000000 && (u32)elfBuf <= DRAM_LIB_ADDR)
		free(elfBuf);
	free(fileBuf);
	elfBuf = NULL;
	fileBuf = NULL;

elfLoadFinalOut:

	return res;
}