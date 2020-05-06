/*
 * GUI & Explorer for BOOT-LOADRER of ARDUFPGA soft core design.
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
#include "spi.h"
#include "delay.h"
#include "25flash.h"
#include "fat_fs/inc/ff.h"
#include "ffconf.h"
#include "kbd.h"
#include "util-exp.h"

uint16_t menu_pos = 0;
uint16_t menu_sel = 0;
uint16_t items_scanned = 0;
static const char* const str[] = {_VOLUME_STRS};
FRESULT res = FR_OK;
uint8_t fattrib = 0;
char nameBuff[27];
DIR dirObject;
FILINFO fInfo;
FIL filObject;
bool fs_mounted = false;

const char design_update_err_message[] PROGMEM = "Err reading design update...\r\nThe design need to be loaded\r\n with a programmer";

extern _25flash_t flash_des;
//extern _25flash_t flash_app;

bool gui_initialized = false;

void gui_init(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
	kbd_init();
	gui_idle(uSD, spi_screen, screen_buf);
	gui_paint(uSD, spi_screen, screen_buf);
}

void gui_idle(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
// Check if the FS is mounted, if it is, open the root directory.
	if(uSD->fs_mounted != fs_mounted && uSD->fs_mounted) {
		fs_mounted = true;
		if(f_opendir(&dirObject, "/") == FR_OK) {
			f_chdir("/");
			if(!gui_initialized) {
				gui_initialized = true;
				uint8_t buf[64];
#ifdef USE_DESIGN_UPGRADE
/*****************************************/
// Check if a design update has been found.
/*****************************************/
				util_design_app_update(uSD, spi_screen, screen_buf, true);
/*****************************************/
// !Check if a design update has been found.
/*****************************************/
#endif
/*****************************************/
// Read the last opened application path.
/*****************************************/
//Check for current.txt file at startup.
				if(f_open(&filObject, "current.txt", FA_READ) == FR_OK) {
					UINT b_read = 0x00;
					f_rewind(&filObject);
					if(f_read(&filObject, buf, f_size(&filObject), &b_read) == FR_OK) {
						if(f_size(&filObject) != b_read) {
							return;
						}
						f_close(&filObject);
						char tmp_buf[16];
						char filename[27];
						bool fallow_ok = util_fallow_path(tmp_buf, (char *)buf);
// Extract the name with extension of the application.
						util_get_filename(filename, (char *)buf);
/*****************************************/
// Check if EEPROM was edited.
/*****************************************/
// If EEPROM was edited then the explorer was oppened after a user application was interrupted, if EEPROM was not edited then the explorer is opened after a user application was interrupted or after a power up (no need to save EEPROM to uSD).
						if(BOOT_STAT & BOOT_STAT_EEP_EDITED) {
							BOOT_STAT &= ~BOOT_STAT_EEP_EDITED;
// Check if the path was successfully fallow.
							if(fallow_ok) {
								util_change_extension(filename, filename, PSTR("eep"));
								bool eep_different = false;
// Try to open the EEPROM file.
								if(f_open(&filObject, filename, FA_READ) == FR_OK) { 
// File exist, compare it, we do not want to wear out the uSD with the same data.
									for (uint16_t cnt = 0; cnt < EEP_SIZE; cnt += 0x20) {
										eeprom_read_block(buf + 0x20, (void *)cnt, 0x20);
										if(f_read(&filObject, buf, 0x20, &b_read) == FR_OK) {
											if(memcmp(buf + 0x20, buf, 0x20)) {
												eep_different = true;
												break;
											}
										}
									}
									f_close(&filObject);
								} else { 
// File does not exist.
									eep_different = true;
								}
// File does not exist or is not the same as in EEPROM, save it to uSD.
								if(eep_different) {
									if(f_open(&filObject, filename, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
										UINT b_write = 0x00;
										for (uint16_t cnt = 0; cnt < EEP_SIZE; cnt += 0x40) {
											eeprom_read_block(buf, (void *)cnt, 0x40);
											if(f_write(&filObject, buf, 0x40, &b_write) == FR_OK) {
												if(b_write != 0x40) {
													f_unlink(filename);
													return;
												}
											}
										}
										f_truncate(&filObject);
										f_close(&filObject);
// There is an ERROR in write function, the uSD remain locked after the file is write and closed when you try to reinitialize the uSD.
// I add a open statement this seems to unlock the uSD.
										f_open(&filObject, filename, FA_READ);
									}
								}
							}
/*****************************************/
// !Check if EEPROM was edited.
/*****************************************/
						}
					}
/*****************************************/
// !Read the last opened application path.
/*****************************************/
				}
			}
		}
	} else {
		if(uSD->fs_mounted != fs_mounted && ~uSD->fs_mounted) {
			fs_mounted = false;
		}
	}
	kbd_idle();
	if(kbd_changed()) {
		uint8_t kbd_state = kbd_get();
// Navigate the menu UP.
		if(kbd_state & KBD_U_KEY) {
			if(menu_sel > 0) {
				menu_sel--;
			}
			if(menu_sel < menu_pos) {
				menu_pos = menu_sel;
			}
			gui_paint(uSD, spi_screen, screen_buf);
		} else
// Navigate the menu DOWN.
		if(kbd_state & KBD_D_KEY) {
			if((menu_sel < (menu_pos + 8)) && menu_sel < items_scanned - 1) {
				menu_sel++;
			}
			if(menu_sel >= (menu_pos + 8) && res == FR_OK) {
				menu_pos = (menu_sel - 7);
			}
			gui_paint(uSD, spi_screen, screen_buf);
		} else
// Open the selected directory/file.
		if(kbd_state & KBD_A_KEY) {
			if(fattrib & AM_DIR) {
// Open directory.
				if(f_opendir(&dirObject, nameBuff) == FR_OK) {
					f_chdir(nameBuff);
					menu_pos = 0;
					menu_sel = 0;
					gui_paint(uSD, spi_screen, screen_buf);
				}
			} else {
//Open selected file and load it.
				util_app_load(uSD, spi_screen, screen_buf);
			}
		} else
		if(kbd_state & KBD_B_KEY) {
			f_opendir(&dirObject, "..");
			f_chdir("..");
			menu_pos = 0;
			menu_sel = 0;
			gui_paint(uSD, spi_screen, screen_buf);
		}
	}
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
				if(menu_scan == menu_pos + 8 || res != FR_OK) {
					items_scanned = menu_scan + (res == FR_OK ? 1 : 0);
					break;
				}
// Filter, display only files with .app extension and the sub directory's.
				if(res == FR_OK && (!strcmp(fInfo.altname + strlen(fInfo.altname) - 3, "APP") || fInfo.fattrib & AM_DIR)/*  && (fInfo.fattrib & AM_ARC)*/ ) {
// Print only what is inside display window.
					if(menu_scan >= menu_pos && menu_scan < menu_pos + 8) {
						if(menu_scan == menu_sel) {
							fattrib = fInfo.fattrib;
							strcpy(nameBuff, fInfo.fname);
						}
// Check and print the selected line.
						strcpy(tmpNameBuff, menu_scan == menu_sel ? ">":"  ");
// Concatenate the file/directory name.
						strcat(tmpNameBuff, fInfo.fname);
						ssd1306_put_rectangle(spi_screen, NULL, screen_buf, 0, (menu_scan - menu_pos) << 3, ssd1306_get_x(), 8, true, true);
						ssd1306_draw_string(spi_screen, NULL, screen_buf, tmpNameBuff, 0, (menu_scan - menu_pos) << 3, false, false, 1, 0);
					}
				menu_scan++;
				}
			}
// If there are less that eight lines to print on the display, fill the rest of the display with the background color.
			for (uint16_t tmp_cnt = (menu_scan - menu_pos); tmp_cnt < 8; tmp_cnt++) {
				ssd1306_put_rectangle(spi_screen, NULL, screen_buf, 0, tmp_cnt << 3, ssd1306_get_x(), 8, true, true);
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