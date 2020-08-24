/*
 * Copyright (c) 2018 naehrwert
 * Copyright (C) 2018 CTCaer
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

#ifndef _BTN_H_
#define _BTN_H_

#include "utils/types.h"

#define BTN_POWER 0x1
#define BTN_VOL_DOWN 0x2
#define BTN_VOL_UP 0x4

u32 btn_read();
u32 btn_wait();
u32 btn_wait_timeout(u32 time_ms, u32 mask);

#endif
