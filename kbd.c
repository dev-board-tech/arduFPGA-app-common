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

#include "def.h"
#include "kbd.h"
#include "delay.h"

uint8_t state = 0;
uint8_t last_state = 0;
timer_t kbd_u_timer;
timer_t kbd_d_timer;
timer_t kbd_l_timer;
timer_t kbd_r_timer;

void kbd_init() {
	kbd_u_timer.value = 300;
	kbd_d_timer.value = 300;
	kbd_l_timer.value = 300;
	kbd_r_timer.value = 300;
//We do not use KBD PORT setup because is PORTA and this PORT has outputs with different functions than inputs, so we need to let PORTA how was configured in the design at reset time.
/*#ifdef KBD_L_IN
	KBD_L_DIR &= ~KBD_L_PIN;
	KBD_L_PORT |= KBD_L_PIN;
#endif
#ifdef KBD_R_IN
	KBD_R_DIR &= ~KBD_R_PIN;
	KBD_R_PORT |= KBD_R_PIN;
#endif
#ifdef KBD_U_IN
	KBD_U_DIR &= ~KBD_U_PIN;
	KBD_U_PORT |= KBD_U_PIN;
#endif
#ifdef KBD_D_IN
	KBD_D_DIR &= ~KBD_D_PIN;
	KBD_D_PORT |= KBD_D_PIN;
#endif
#ifdef KBD_A_IN
	KBD_A_DIR &= ~KBD_A_PIN;
	KBD_A_PORT |= KBD_A_PIN;
#endif
#ifdef KBD_B_IN
	KBD_B_DIR &= ~KBD_B_PIN;
	KBD_B_PORT |= KBD_B_PIN;
#endif*/
}

void kbd_idle() {
	state = 0;
#ifdef KBD_L_IN
	if((~KBD_L_IN) & KBD_L_PIN)
		state |= KBD_L_KEY;
#endif
#ifdef KBD_U_IN
	if((~KBD_U_IN) & KBD_U_PIN)
		state |= KBD_U_KEY;
#endif
#ifdef KBD_D_IN
	if((~KBD_D_IN) & KBD_D_PIN)
		state |= KBD_D_KEY;
#endif
#ifdef KBD_R_IN
	if((~KBD_R_IN) & KBD_R_PIN)
		state |= KBD_R_KEY;
#endif
#ifdef KBD_A_IN
	if((~KBD_A_IN) & KBD_A_PIN)
		state |= KBD_A_KEY;
#endif
#ifdef KBD_B_IN
	if((~KBD_B_IN) & KBD_B_PIN)
		state |= KBD_B_KEY;
#endif
}

bool kbd_changed() {
	bool changed = false;
#ifdef KBD_USE_TICK
	if((state & KBD_U_KEY) != (~last_state & KBD_U_KEY)) {
		last_state |= KBD_U_KEY;
		kbd_u_timer.value = 500;
		timer_enable(&kbd_u_timer, true);
		changed = true;
	} else if((~state & KBD_U_KEY) != (last_state & KBD_U_KEY)) {
		last_state &= ~KBD_U_KEY;
		timer_enable(&kbd_u_timer, false);
	}
	if((state & KBD_D_KEY) != (~last_state & KBD_D_KEY)) {
		last_state |= KBD_D_KEY;
		kbd_d_timer.value = 500;
		timer_enable(&kbd_d_timer, true);
		changed = true;
	} else if((~state & KBD_D_KEY) != (last_state & KBD_D_KEY)) {
		last_state &= ~KBD_D_KEY;
		timer_enable(&kbd_d_timer, false);
	}
	if((state & KBD_L_KEY) != (~last_state & KBD_L_KEY)) {
		last_state |= KBD_L_KEY;
		kbd_l_timer.value = 500;
		timer_enable(&kbd_l_timer, true);
		changed = true;
	} else if((~state & KBD_L_KEY) != (last_state & KBD_L_KEY)) {
		last_state &= ~KBD_L_KEY;
		timer_enable(&kbd_l_timer, false);
	}
	if((state & KBD_R_KEY) != (~last_state & KBD_R_KEY)) {
		last_state |= KBD_R_KEY;
		kbd_r_timer.value = 500;
		timer_enable(&kbd_r_timer, true);
		changed = true;
	} else if((~state & KBD_R_KEY) != (last_state & KBD_R_KEY)) {
		last_state &= ~KBD_R_KEY;
		timer_enable(&kbd_r_timer, false);
	}
	if(timer_tik(&kbd_u_timer)) {
		kbd_u_timer.value = 250;
		changed = true;
	}
	if(timer_tik(&kbd_d_timer)) {
		kbd_d_timer.value = 250;
		changed = true;
	}
	if(timer_tik(&kbd_l_timer)) {
		kbd_l_timer.value = 250;
		changed = true;
	}
	if(timer_tik(&kbd_r_timer)) {
		kbd_r_timer.value = 250;
		changed = true;
	}
#else
	if(last_state ^ state) {
		changed = true;
		delay_ms(10);
	}
	last_state = state;
#endif
	return changed;
}

uint8_t kbd_get() {
	return last_state;
}
