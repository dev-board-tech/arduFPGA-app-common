/*
 * Delay library file for arduFPGA designs.
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

#include "delay.h"
#include "def.h"

volatile uint32_t timer_cnt = 0;

void delay_us(uint32_t us) {
	volatile int32_t us_ = (us * (F_CPU / 1000000 / 37)) - 50;
	if(us_ < 0)
		return;
	while(us_--);
}

void delay_ms(uint16_t ms) {
	volatile int32_t ms_ = (ms * (F_CPU / 1000 / 37)) - 50;
	if(ms_ < 0)
		return;
	while(ms_--);
}

void delay_s(uint16_t s) {
	volatile int32_t s_ = (s * (F_CPU / 37)) - 50;
	if(s_ < 0)
		return;
	while(s_--);
}

void timer_service() {
	timer_cnt++;
}

void timer_enable(timer_t *inst, bool enable) {
	inst->tik = timer_cnt + inst->value;
	inst->enabled = enable;
}

bool timer_tik(timer_t *inst) {
	bool tick = inst->tik < timer_cnt ? inst->enabled : false;
	if(tick) {
		inst->tik = timer_cnt + inst->value;
	}
	return tick;
}
