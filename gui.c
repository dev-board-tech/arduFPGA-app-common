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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include "gui.h"
#include "driver/spi.h"
#include "delay.h"
//#include "25flash.h"
#include "fat_fs/inc/ff.h"
#include "ffconf.h"
#include "kbd.h"
#include GUI_APP_FILE_INCLUDE

uint8_t disp_up_limit = GUI_UPPER_LIMIT_ROW;
uint8_t disp_dn_limit = GUI_LOWER_LIMIT_ROW;
uint16_t menu_pos = 0;
uint16_t menu_sel = 0;
uint16_t file_checked = -1;
uint16_t path_cnt = 0;
uint16_t path_sel_cnt = 0;
uint16_t items_scanned = 0;
static const char* const str[] = {_VOLUME_STRS};
FRESULT res = FR_OK;
uint8_t fattrib = 0;
#ifdef MAX_ALLOWED_FILE_NAME_LEN_BUF
char nameBuff[MAX_ALLOWED_FILE_NAME_LEN_BUF];
#else
char nameBuff[1];
#endif
extern DIR dirObject;
extern FILINFO fInfo;
extern FIL filObject;
bool fs_mounted = false;
bool gui_redirect_up_btn = false;
bool gui_redirect_dn_btn = false;

bool gui_initialized = false;

const char file_ext_filter[][4] PROGMEM = GUI_LIST_OF_DISPLAYED_EXTENSIONS;

void gui_init(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
	kbd_init();
	gui_idle(uSD, spi_screen, screen_buf);
	gui_paint(uSD, spi_screen, screen_buf);
#ifdef GUI_ACT_FUNC_ON_INIT
	GUI_ACT_FUNC_ON_INIT(uSD, spi_screen, screen_buf);
#endif
}

void gui_idle(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
// Check if the FS is mounted, if it is, open the root directory.
	if(uSD->fs_mounted != fs_mounted && uSD->fs_mounted) {
		fs_mounted = true;
#ifdef GUI_ACT_FUNC_ON_FS_MOUNTED
		GUI_ACT_FUNC_ON_FS_MOUNTED(uSD, spi_screen, screen_buf);
#endif
	} else {
		if(uSD->fs_mounted != fs_mounted && ~uSD->fs_mounted) {
			fs_mounted = false;
		}
	}
#ifdef GUI_ACT_FUNC_ON_IDLE
	GUI_ACT_FUNC_ON_IDLE(uSD, fs_mounted, spi_screen, screen_buf);
#endif
	kbd_idle();
	if(kbd_changed()) {
		uint8_t kbd_state = kbd_get();
// Navigate the menu UP.
		if(kbd_state & KBD_U_KEY) {
#ifdef GUI_ACT_FUNC_ON_UP_BTN_PRESS
			if(gui_redirect_up_btn) {
				GUI_ACT_FUNC_ON_UP_BTN_PRESS(uSD, spi_screen, screen_buf);
			} else {
#endif
				if(menu_sel > 0) {
					menu_sel--;
				}
				if(menu_sel < menu_pos) {
					menu_pos = menu_sel;
				}
				gui_paint(uSD, spi_screen, screen_buf);
#ifdef GUI_ACT_FUNC_ON_UP_BTN_PRESS
			}
#endif
		} else
// Navigate the menu DOWN.
		if(kbd_state & KBD_D_KEY) {
#ifdef GUI_ACT_FUNC_ON_DN_BTN_PRESS
			if(gui_redirect_dn_btn) {
				GUI_ACT_FUNC_ON_DN_BTN_PRESS(uSD, spi_screen, screen_buf);
			} else {
#endif
				if((menu_sel < (menu_pos + (disp_dn_limit + 1) - disp_up_limit)) && menu_sel < items_scanned - 1) {
					menu_sel++;
				}
				if(menu_sel >= (menu_pos + (disp_dn_limit + 1) - disp_up_limit) && res == FR_OK) {
					menu_pos++;
				}
				gui_paint(uSD, spi_screen, screen_buf);
#ifdef GUI_ACT_FUNC_ON_UP_BTN_PRESS
			}
#endif
		} else
// Open the selected directory/file.
		if(kbd_state & KBD_A_KEY) {
			if(fattrib & AM_DIR) {
// Open directory.
				if(f_opendir(&dirObject, nameBuff) == FR_OK) {
					f_chdir(nameBuff);
					menu_pos = 0;
					menu_sel = 0;
					path_cnt++;
					gui_paint(uSD, spi_screen, screen_buf);
				}
			} else {
//Open selected file.
				path_sel_cnt = path_cnt;
#ifdef GUI_ACT_FUNC_ON_FILE_SELECT
				GUI_ACT_FUNC_ON_FILE_SELECT(uSD, spi_screen, screen_buf);
#endif
				gui_paint(uSD, spi_screen, screen_buf);
			}
		} else if(kbd_state & KBD_B_KEY) {
// Go to parent directory.
			f_opendir(&dirObject, "..");
			f_chdir("..");
			menu_pos = 0;
			menu_sel = 0;
			path_cnt--;
			gui_paint(uSD, spi_screen, screen_buf);
		} else if(kbd_state & KBD_L_KEY) {
#ifdef GUI_ACT_FUNC_ON_LEFT_BTN_PRESS
			GUI_ACT_FUNC_ON_LEFT_BTN_PRESS(uSD, spi_screen, screen_buf);
#endif
		} else if(kbd_state & KBD_R_KEY) {
#ifdef GUI_ACT_FUNC_ON_RIGHT_BTN_PRESS
			GUI_ACT_FUNC_ON_RIGHT_BTN_PRESS(uSD, spi_screen, screen_buf);
#endif
		}
	}
}
// Check selected file extension against a table with extensions, if is found will return true.
bool gui_is_extension(char *ext) {
	uint8_t cnt = 0;
	for(cnt = 0; cnt < sizeof(file_ext_filter) / sizeof(file_ext_filter[0]); cnt++) {
		if(!strcmp_P(fInfo.altname + strlen(fInfo.altname) - 3, file_ext_filter[cnt])) {
			return true;
		}
	}
	return false;
}

void gui_paint(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
	if(fs_mounted) {
// Only the FS is mounted.
		char tmpNameBuff[27];
		if(f_rewinddir(&dirObject) == FR_OK) {
			uint16_t menu_scan = 0;
			while(1) {
// Read each item in current directory.
				res = f_readdir(&dirObject, &fInfo);
// If there are more lines to display than the space on the display, exit the loop.
				if(menu_scan == menu_pos + (disp_dn_limit + 1) - disp_up_limit || res != FR_OK) {
					items_scanned = menu_scan + (res == FR_OK ? 1 : 0);
					break;
				}
// Filter, display only files with .app extension and the sub directory's.
				if(res == FR_OK && (gui_is_extension(fInfo.altname + strlen(fInfo.altname) - 3) || fInfo.fattrib & AM_DIR)/*  && (fInfo.fattrib & AM_ARC)*/ ) {
// Print only what is inside display window.
					if(menu_scan >= menu_pos && menu_scan < menu_pos +  (disp_dn_limit + 1) - disp_up_limit) {
						if(menu_scan == menu_sel) {
							fattrib = fInfo.fattrib;
#ifdef MAX_ALLOWED_FILE_NAME_LEN_BUF
							strncpy(nameBuff, fInfo.fname, MAX_ALLOWED_FILE_NAME_LEN_BUF);
#endif
						}
// Check and print the selected line.
						if(file_checked != -1)
// This is utile for players, will display an '*' on selected/current playing file.
							strcpy(tmpNameBuff, menu_scan == menu_sel ? ">":(menu_scan == file_checked && path_sel_cnt == path_cnt ? "*":"  "));
						else {
							strcpy(tmpNameBuff, menu_scan == menu_sel ? ">":"  ");
						}
#ifdef GUI_ACT_FUNC_ON_IDLE
						GUI_ACT_FUNC_ON_IDLE(uSD, fs_mounted, spi_screen, screen_buf);
#endif
// Concatenate the file/directory name.
						strcat(tmpNameBuff, fInfo.fname);
						DISPLAY_FUNC_DRAW_RECTANGLE(spi_screen, NULL, screen_buf, 0, ((menu_scan - menu_pos) + disp_up_limit) << 3, DISPLAY_FUNC_GET_X(), 8, true, true);
						DISPLAY_FUNC_DRAW_STRING(spi_screen, NULL, screen_buf, tmpNameBuff, 0, ((menu_scan - menu_pos) + disp_up_limit) << 3, false, false, 1, 0);
					}
					menu_scan++;
				}
			}
// If there are less that eight lines to print on the display, fill the rest of the display with the background color.
			for (uint16_t tmp_cnt = ((menu_scan - menu_pos) + disp_up_limit); tmp_cnt < (disp_dn_limit + 1); tmp_cnt++) {
				DISPLAY_FUNC_DRAW_RECTANGLE(spi_screen, NULL, screen_buf, 0, tmp_cnt << 3, DISPLAY_FUNC_GET_X(), 8, true, true);
#ifdef GUI_ACT_FUNC_ON_IDLE
				GUI_ACT_FUNC_ON_IDLE(uSD, fs_mounted, spi_screen, screen_buf);
#endif
			}
		}
		else {
// If there is an error when will rewind the directory, clear the screen and print an error.
			menu_pos = 0;
			menu_sel = 0;
			gui_print_status(spi_screen, screen_buf, PSTR("ERR reading uSD."), 0);
		}
	} else {
// Clear the screen if the uSD was pull from the socket. 
		menu_pos = 0;
		menu_sel = 0;
		gui_print_status(spi_screen, screen_buf, PSTR("uSD not inserted..."), 0);
	}
}




