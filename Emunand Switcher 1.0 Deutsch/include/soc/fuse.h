/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2018 shuffle2
 * Copyright (c) 2018 balika011
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

#ifndef _FUSE_H_
#define _FUSE_H_

#include "utils/types.h"

/*! Fuse registers. */
#define FUSE_CTRL 0x0
#define FUSE_ADDR 0x4
#define FUSE_RDATA 0x8
#define FUSE_WDATA 0xC
#define FUSE_TIME_RD1 0x10
#define FUSE_TIME_RD2 0x14
#define FUSE_TIME_PGM1 0x18
#define FUSE_TIME_PGM2 0x1C
#define FUSE_PRIV2INTFC 0x20
#define FUSE_FUSEBYPASS 0x24
#define FUSE_PRIVATEKEYDISABLE 0x28
#define FUSE_DISABLEREGPROGRAM 0x2C
#define FUSE_WRITE_ACCESS_SW 0x30
#define FUSE_PWR_GOOD_SW 0x34
#define FUSE_SKU_INFO 0x110
#define FUSE_FIRST_BOOTROM_PATCH_SIZE 0x19c
#define FUSE_PRIVATE_KEY0 0x1A4
#define FUSE_PRIVATE_KEY1 0x1A8
#define FUSE_PRIVATE_KEY2 0x1AC
#define FUSE_PRIVATE_KEY3 0x1B0

/*! Fuse commands. */
#define FUSE_READ 0x1
#define FUSE_WRITE 0x2
#define FUSE_SENSE 0x3
#define FUSE_CMD_MASK 0x3

/*! Fuse cache registers. */
#define FUSE_RESERVED_ODMX(x) (0x1C8 + 4 * (x))

void fuse_disable_program();
u32 fuse_read_odm(u32 idx);
void fuse_wait_idle();
int fuse_read_ipatch(void (*ipatch)(u32 offset, u32 value));
int fuse_read_evp_thunk(u32 *iram_evp_thunks, u32 *iram_evp_thunks_len);
void read_raw_ipatch_fuses(u32 *words);

#endif
