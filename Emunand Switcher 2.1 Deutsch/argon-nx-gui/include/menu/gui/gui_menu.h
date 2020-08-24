/*  
 * Copyright (c) 2018 Guillem96
 * Copyright (c) 2019 Storm
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
#ifndef _MENU_H_
#define _MENU_H_

#include "utils/types.h"
#include "core/custom-gui.h"
#include "core/argon-ctxt.h"

/* Handle all menu related stuff */
void gui_menu_open(argon_ctxt_t* argon_ctxt);

/* Handle all menu related stuff */
void gui_menu_draw(argon_ctxt_t* argon_ctxt);

/* Deallocate a menu from heap */
void gui_menu_destroy(argon_ctxt_t* argon_ctxt);

//void manual_system_maintenance(bool refresh);//hinzugef�gt


#endif