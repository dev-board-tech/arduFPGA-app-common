/*
 * GUI & Explorer file for arduFPGA designs.
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


#ifndef GUI_H_
#define GUI_H_

#include "def.h"
#include <avr/pgmspace.h>
#include "mmc_sd_spi.h"
#include DISPLAY_DRIVER_FILE

extern bool gui_redirect_up_btn;
extern bool gui_redirect_dn_btn;

inline void gui_print_status(spi_t *spi_screen, uint8_t *screen_buf, const char *text, uint8_t bar_len) {
	char buf[32];
	strcpy_P(buf, text);
#ifdef DISPLAY_FUNC_CLEAR
	DISPLAY_FUNC_CLEAR(spi_screen, screen_buf, 0);
#endif
	if(bar_len) {
#ifdef DISPLAY_FUNC_DRAW_RECTANGLE
		DISPLAY_FUNC_DRAW_RECTANGLE(spi_screen, NULL, screen_buf, 0, 32, bar_len, 8, true, true);
#endif
	}
#ifdef DISPLAY_FUNC_DRAW_STRING
	DISPLAY_FUNC_DRAW_STRING(spi_screen, NULL, screen_buf, buf, 0, 8, false, false, 0, 1);
#endif
}

inline void gui_draw_string(spi_t *spi_screen, uint8_t *screen_buf, const char *text, uint8_t x, uint8_t y) {
	char buf[32];
	strcpy_P(buf, text);
#ifdef DISPLAY_FUNC_DRAW_STRING
	DISPLAY_FUNC_DRAW_STRING(spi_screen, NULL, screen_buf, buf, x, y, false, false, 0, 1);
#endif
}



void gui_init(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf);
void gui_idle(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf);
void gui_paint(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf);

void inline gui_set_redirect_up_btn(bool state) {
	gui_redirect_up_btn = state;
}

bool inline gui_get_redirect_up_btn() {
	return gui_redirect_up_btn;
}

void inline gui_set_redirect_dn_btn(bool state) {
	gui_redirect_dn_btn = state;
}

bool inline gui_get_redirect_dn_btn() {
	return gui_redirect_dn_btn;
}

#endif /* GUI_H_ */