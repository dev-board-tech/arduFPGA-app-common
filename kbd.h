/*
 * Keyboard driver file for arduFPGA designs.
 * 
 * Copyright (C) 2020  Iulian Gheorghiu (morgoth@devboard.tech)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef KBD_H_
#define KBD_H_

#include <stdbool.h>

#define KBD_L_KEY	(1<<5)
#define KBD_U_KEY	(1<<4)
#define KBD_D_KEY	(1<<3)
#define KBD_R_KEY	(1<<2)
#define KBD_A_KEY	(1<<1)
#define KBD_B_KEY	(1<<0)

void kbd_init();
void kbd_idle();
bool kbd_changed();
uint8_t kbd_get();

#endif /* KBD_H_ */