/*
 * Utility's for BOOT-LOADRER of ARDUFPGA soft core design.
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


#ifndef __UTIL_EXP_H__
#define __UTIL_EXP_H__

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "spi.h"
#include "mmc_sd_spi.h"
#include "def.h"
#include "fat_fs/inc/ff.h"
#include "25flash.h"
#include "kbd.h"
#include "gui.h"
#include "ssd1306.h"
#include "delay.h"

extern DIR dirObject;
extern FILINFO fInfo;
extern FIL filObject;
extern _25flash_t flash_des;
//extern _25flash_t flash_app;
extern const char design_update_err_message[];
extern char nameBuff[];
extern bool gui_initialized;

const char design_update_err_message[] PROGMEM = "Err reading design update...\r\nThe design need to be loaded\r\n with a programmer";

inline void app_strip_extension(char *ret_path, char *path) {
	char *path_int = path + strlen(path);
	do
	{
		path_int--;
	} while (*path_int != '.' && path_int != path);
	if(path_int != path) {
		if(ret_path != path) {
			strncpy(ret_path, path, (path_int - path) + 1);
			} else {
			*path_int = '\0';
		}
	}
}

inline char *app_get_path(char *ret_path, char *ptr) {
// Check if a separator exists.
	char *start_ptr = strchr(ptr, '/');
// If a separator does not exists signify that is not a path.
	if(!start_ptr)
		return NULL;
// If a separator is found on other location that the first character, signify that is a relative path.
	if (start_ptr - ptr != 0) {
		start_ptr = ptr - 1;
	}
// Check for the end separator, every path need to end with a separator.
	char *end_ptr = strchr(start_ptr + 1, '/');
// If the end separator is not found, signify that there is no path.
	if(!end_ptr)
		return NULL;
	strncpy(ret_path, start_ptr + 1, end_ptr - start_ptr - 1);
// Put the end of string.
	ret_path[end_ptr - start_ptr - 1] = 0;
	return end_ptr; 
}

inline void app_get_filename(char *ret_path, char *path) {
	char *path_int = path + strlen(path);
// Scan for the first separator from the right to left.
	do {
		path_int--;
	} while (*path_int != '/' && path_int != path);
// If the path string differ from the scanned one, copy it to the return string.
	if(path_int != path) {
		strcpy(ret_path, path_int + 1);
	}
}

inline void app_get_extension(char *ret_path, char *path) {
	char *path_int = path + strlen(path);
	do
	{
		path_int--;
	} while (*path_int != '.' && path_int != path);
	if(path_int != path) {
		strcpy(ret_path, path_int + 1);
	}
}

inline void app_change_extension(char *ret_path, char *path, const char *extension) {
	char *path_int = path + strlen(path);
// Scan for the first dot from right to left.
	do {
		path_int--;
	} while (*path_int != '.' && path_int != path);
// If the path string differ from the scanned one, copy it to the return string.
	if(path_int != path) {
// If the destination string pointer differs from the source string pointer, copy the string without extension.
		if(ret_path != path) {
			strncpy(ret_path, path, (path_int - path) + 1);
			} else {
// If the destination string pointer is the same as the source string pointer put the end of string delimiter.
			*path_int = '\0';
		}
// Append the new extension to the destination string.
		strcat(ret_path, ".");
		strcat_P(ret_path, extension);
	}
}

inline bool app_fallow_path(char *tmp_buf, char *path) {
// Extract the path to the application.
	char *ptr = (char*)path;
// Navigate to the APP directory.
	bool fallow_ok = true;
	f_closedir(&dirObject);
	while ((ptr = app_get_path(tmp_buf, ptr))) {
		if(f_opendir(&dirObject, tmp_buf) == FR_OK){
			f_chdir(tmp_buf);
			} else {
			if(f_opendir(&dirObject, "/") == FR_OK) {
				f_chdir("/");
				fallow_ok = false;
			}
			break;
		}
	}
	return fallow_ok;
}
// If des.bin and app.bin are found in the root, it will ask for update, these files will be deleted.
// If des.bin and app.bin are found in the platform directory, will update the design and explorer without asking for update, is considered a change in used platform, these files will not be deleted.
inline bool app_design_app_update(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf, bool ask_update) {
	bool des_need_update = false;
	bool boot_need_update = false;
	if(ask_update) {
		gui_draw_string(spi_screen, screen_buf, PSTR("Check for update..."), 0, 8);
		delay_ms(1000);
	}
	FRESULT f2 = f_open(&filObject, "exp.bin", FA_READ);
	if(f2 == FR_OK) {
		f_close(&filObject);
	}
	FRESULT f1 = f_open(&filObject, "des.bin", FA_READ);
	if(f1 == FR_OK && f2 == FR_OK) {
		// Design update has been found.
		// If the design update has been found, there is mandatory to be an application update file.
		//gui_print_status(spi_screen, screen_buf, PSTR("Design update file found\r\nDo you want to update\r\n the design?\r\n   OK=Update it.\r\n   BACK=Ignore and delete from uSD."), 0);
		uint8_t kbd_state = 0;
		if(ask_update) {
			gui_draw_string(spi_screen, screen_buf, PSTR("Design update file found"), 0, 8);
			gui_draw_string(spi_screen, screen_buf, PSTR("Do you want to update"), 0, 16);
			gui_draw_string(spi_screen, screen_buf, PSTR("the design?"), 6, 24);
			gui_draw_string(spi_screen, screen_buf, PSTR("OK=Update it."), 12, 32);
			gui_draw_string(spi_screen, screen_buf, PSTR("BACK=Ignore and delete"), 12, 40);
			gui_draw_string(spi_screen, screen_buf, PSTR("update files from uSD."), 18, 48);
			do {
				kbd_idle();
			} while(!kbd_changed());
			kbd_state = kbd_get();
		}
		if((kbd_state & KBD_A_KEY) || ask_update == false) {
			UINT b_read = 0x00;
			uint8_t buf[64];
			uint32_t cnt = 0;
/*****************************************/
/* Verify the design if is the same.*/
/* We do not need to check more than 64 bytes at the beginning of the image,
    because the image include the date, hour and minute when was compiled. */
/*****************************************/
			//gui_print_status(spi_screen, screen_buf, PSTR("Verify design..."), 0);
			for (cnt = 0; cnt < 0x40; cnt += 0x20) {
				if(f_read(&filObject, buf, 0x20, &b_read) == FR_OK) {
					_25flash_read(&flash_des, cnt, buf + 0x20, b_read);
					//ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt) >> 10, 32, 1, 8, true, true);
					if(memcmp(buf, buf + 0x20, b_read)) {
						des_need_update = true;
						break;
					}
				}
			}
/*****************************************/
// !Verify the design if is the same.
/*****************************************/
/*****************************************/
// Update the design FLASH.
/*****************************************/
			if(des_need_update) {
				f_rewind(&filObject);
				gui_print_status(spi_screen, screen_buf, PSTR("Erasing design..."), 0);
				_24flash_write_status(&flash_des, 0x80);
				for (cnt = 0; cnt < f_size(&filObject) + 0x1000; cnt += 0x1000) {
					_25flash_erase(&flash_des, cnt);
					ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt) >> 10, 32, 4, 8, true, true);
				}
// Write the updated design to the FLASH.
				gui_print_status(spi_screen, screen_buf, PSTR("Write design..."), 0);
				for (cnt = 0; cnt < f_size(&filObject); cnt += 0x40) {
					if(f_read(&filObject, buf, 0x40, &b_read) == FR_OK) {
						_25flash_write(&flash_des, cnt, buf, b_read);
						ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt) >> 10, 32, 1, 8, true, true);
					} else {
						gui_print_status(spi_screen, screen_buf, design_update_err_message, 0);
						_24flash_write_status(&flash_des, 0xBC);
						while(1);
					}
				}
				_24flash_write_status(&flash_des, 0xBC);
				f_close(&filObject);
				if(ask_update) {
					f_unlink("des.bin");
				}
			}
/*****************************************/
// !Update the design FLASH.
/*****************************************/
/*****************************************/
// Update the bootloader/explorer FLASH.
/*****************************************/
			if(f_open(&filObject, "exp.bin", FA_READ) == FR_OK) {
/*****************************************/
// Verify the bootloader/explorer FLASH if is the same.
/*****************************************/
				gui_print_status(spi_screen, screen_buf, PSTR("Verify explorer..."), 0);
				for (cnt = FLASH_APP_EXPLORER_START_ADDR; cnt < f_size(&filObject) + FLASH_APP_EXPLORER_START_ADDR; cnt += 0x20) {
					if(f_read(&filObject, buf, 0x20, &b_read) == FR_OK) {
						_25flash_read(&flash_des, cnt, buf + 0x20, b_read);
						ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt - FLASH_APP_EXPLORER_START_ADDR) >> (f_size(&filObject) > 0x10000 ? 10 : (f_size(&filObject) > 0x8000 ? 9 : 8)), 32, 1, 8, true, true);
						if(memcmp(buf, buf + 0x20, b_read)) {
							boot_need_update = true;
							break;
						}
					}
				}
/*****************************************/
// !Verify the bootloader/explorer FLASH if is the same.
/*****************************************/
				if(boot_need_update) {
					f_rewind(&filObject);
					gui_print_status(spi_screen, screen_buf, PSTR("Erasing explorer..."), 0);
					_24flash_write_status(&flash_des, 0x80);
					for (cnt = FLASH_APP_EXPLORER_START_ADDR; cnt < f_size(&filObject) + 0x1000 + FLASH_APP_EXPLORER_START_ADDR; cnt += 0x1000) {
						_25flash_erase(&flash_des, cnt);
						ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt - FLASH_APP_EXPLORER_START_ADDR) >> (f_size(&filObject) > 0x10000 ? 10 : (f_size(&filObject) > 0x8000 ? 9 : 8)), 32, 16, 8, true, true);
					}
// Write the updated explorer to the FLASH.
					gui_print_status(spi_screen, screen_buf, PSTR("Updating explorer..."), 0);
					for (cnt = FLASH_APP_EXPLORER_START_ADDR; cnt < f_size(&filObject) + FLASH_APP_EXPLORER_START_ADDR; cnt += 0x40) {
						if(f_read(&filObject, buf, 0x40, &b_read) == FR_OK) {
							_25flash_write(&flash_des, cnt, buf, b_read);
							ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt - FLASH_APP_EXPLORER_START_ADDR) >> (f_size(&filObject) > 0x10000 ? 10 : (f_size(&filObject) > 0x8000 ? 9 : 8)), 32, 1, 8, true, true);
						} else {
							gui_print_status(spi_screen, screen_buf, design_update_err_message, 0);
							_24flash_write_status(&flash_des, 0xBC);
							while(1);
						}
					}
					_24flash_write_status(&flash_des, 0xBC);
					f_close(&filObject);
					if(ask_update) {
						f_unlink("exp.bin");
					}
				}
			}
/*****************************************/
// !Update the bootloader/explorer FLASH.
/*****************************************/
				ssd1306_clear(spi_screen, screen_buf, 0);
				gui_draw_string(spi_screen, screen_buf, PSTR("Design FLASH written successfully."), 0, 16);
				gui_draw_string(spi_screen, screen_buf, PSTR("Now press the RESET button."), 6, 24);
// There is an ERROR in write function, the uSD remain locked after the file is write and closed when you try to reinitialize the uSD.
				//f_open(&filObject, "current.txt", FA_READ); // Dummy open file.
				// TODO hard RESSET.
				//while(1);
				return des_need_update;
			} else if(kbd_state & KBD_B_KEY) {
			if(f1 == FR_OK) {
				f_close(&filObject);
				f_unlink("des.bin");
			}
			if(f2 == FR_OK) {
				f_unlink("exp.bin");
			}
			return false;
		}
	} else if(f1 == FR_OK) {
		f_close(&filObject);
		return false;
	}
	return false;
}

inline void app_app_load(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
	uint8_t flash_buf[64];
	UINT b_read = 0x40;
	bool des_updated = app_design_app_update(uSD, spi_screen, screen_buf, false);
	if(f_open(&filObject, nameBuff, FA_READ) == FR_OK && !des_updated) {
		bool different = false;
		uint32_t cnt = 0;
		gui_print_status(spi_screen, screen_buf, PSTR("Verifying FLASH APP..."), 0);
		// Check if the APP in the flash is the same as the selected for load one, we do not want to wear out the FLASH with the same data.
		do {
			if(f_read(&filObject, flash_buf, 0x20, &b_read) != FR_OK) {
				f_close(&filObject);
				gui_print_status(spi_screen, screen_buf, PSTR("ERR reading file."), 0);
				delay_ms(2000);
				return;
			}
			_25flash_read(&flash_des, FLASH_APP_USER_START_ADDR + cnt, flash_buf + 0x20, b_read);
			if(memcmp(flash_buf, flash_buf + 0x20, b_read)) {
				different = true;
				break;
			}
			cnt += b_read;
			ssd1306_put_rectangle(spi_screen, NULL, screen_buf, cnt >> f_size(&filObject) > 0x10000 ? 10 : (f_size(&filObject) > 0x8000 ? 9 : 8), 32, 4, 8, true, true);
		} while(cnt < f_size(&filObject));
		if(!different) {
			// The application written on the FLASH is the same, load the EEPROM content from uSD if exists.
			f_close(&filObject);
			gui_print_status(spi_screen, screen_buf, PSTR("APP is the same."), 128);
			// Check if a EEPROM file is on the uSD, if it is copy the data into internal emulated EEPROM.
			app_change_extension(nameBuff, nameBuff, PSTR("eep"));
			if(f_open(&filObject, nameBuff, FA_READ) == FR_OK) {
				for (uint16_t cnt = 0; cnt < EEP_SIZE; cnt+= 0x40) {
					if(f_read(&filObject, flash_buf, 0x40, &b_read) == FR_OK) {
						eeprom_write_block(flash_buf, (void *)cnt, 0x40);
						} else {
						break;
					}
				}
				f_close(&filObject);
			}
			// Wait 1000ms to be able to read the message on the display.
			delay_ms(1000);
			ssd1306_on(spi_screen, false);
			BOOT_STAT |= BOOT_STAT_USR_APP_RUNNING;
			BOOT_STAT |= BOOT_STAT_FLASH_APP_NR;
			// Jump to the first stage boot loader, to launch the loaded APP.
			asm("jmp 0x1f804");
		}
		// The selected APP is not the same as the one written on the FLASH.
		// Erase the APP section in the FLASH.
		gui_print_status(spi_screen, screen_buf, PSTR("Erasing FLASH APP..."), 0);
		_24flash_write_status(&flash_des, 0x80);
		for(cnt = FLASH_APP_USER_START_ADDR; cnt < FLASH_APP_USER_START_ADDR + FLASH_APP_RAM_OFFSET; cnt += 0x1000) {
			_25flash_erase(&flash_des, cnt);
			ssd1306_put_rectangle(spi_screen, NULL, screen_buf, (cnt - FLASH_APP_USER_START_ADDR) >> 10, 32, 4, 8, true, true);
		}
		// Write the selected APP from uSD to the FLASH.
		f_rewind(&filObject);
		gui_print_status(spi_screen, screen_buf, PSTR("Writing FLASH APP..."), 0);
		for(cnt = 0; cnt < f_size(&filObject); cnt += 0x40) {
			if(f_read(&filObject, flash_buf, 0x40, &b_read) != FR_OK) {
				gui_print_status(spi_screen, screen_buf, PSTR("ERR reading file."), 128);
				f_close(&filObject);
				return;
			}
			if(b_read != 0x40) {
				_25flash_write(&flash_des, FLASH_APP_USER_START_ADDR + cnt, flash_buf, b_read);
				ssd1306_put_rectangle(spi_screen, NULL, screen_buf, cnt >> 8, 32, 2, 8, true, true);
				break;
			}
			_25flash_write(&flash_des, FLASH_APP_USER_START_ADDR + cnt, flash_buf, 0x40);
			ssd1306_put_rectangle(spi_screen, NULL, screen_buf, cnt >> 8, 32, 2, 8, true, true);
		}
		_24flash_write_status(&flash_des, 0xBC);
		if(cnt + b_read != f_size(&filObject)) {
			gui_print_status(spi_screen, screen_buf, PSTR("ERR reading APP file."), 128);
		}
		f_close(&filObject);
		// Check if a EEPROM file is on the uSD, if it is copy the data into internal emulated EEPROM.
		app_change_extension(nameBuff, nameBuff, PSTR("eep"));
		if(f_open(&filObject, nameBuff, FA_READ) == FR_OK) {
			for (uint16_t cnt = 0; cnt < EEP_SIZE; cnt+= 0x40) {
				if(f_read(&filObject, flash_buf, 0x40, &b_read) == FR_OK) {
					eeprom_write_block(flash_buf, (void *)cnt, 0x40);
					} else {
					break;
				}
			}
			f_close(&filObject);
		}
		// Save the current loaded APP path, including APP name and extension, into the root directory "current.txt" file.
		// This way will know at returning from the user APP, where to save the EEPROM and in future releases the RAM content.
		app_change_extension(nameBuff, nameBuff, PSTR("app"));
		f_closedir(&dirObject);
		f_getcwd((TCHAR*)flash_buf, 0x40);
		strcat((char*)flash_buf, "/");
		strcat((char*)flash_buf, nameBuff);
		if(f_opendir(&dirObject, "/") == FR_OK) {
			f_chdir("/");
			strcpy_P(nameBuff, PSTR("current.txt"));
			if(f_open(&filObject, nameBuff, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
				char cp_path[64];
				f_read(&filObject, cp_path, f_size(&filObject), &b_read);
				if(strcpy(cp_path, (char*)flash_buf)) {
					f_rewind(&filObject);
					f_write(&filObject, (TCHAR*)flash_buf, strlen((char*)flash_buf) + 1, &b_read);
					f_truncate(&filObject);
				}
				f_close(&filObject);
				// There is an ERROR in write function, the uSD remain locked after the file is write and closed when you try to reinitialize the uSD.
				// I add a open statement this seems to unlock the uSD.
				f_open(&filObject, nameBuff, FA_READ);
			}
			f_closedir(&dirObject);
		}
		gui_print_status(spi_screen, screen_buf, PSTR("DONE"), 128);
		// Wait 1000ms to be able to read the message on the display.
		delay_ms(1000);
		ssd1306_on(spi_screen, false);
		BOOT_STAT |= BOOT_STAT_USR_APP_RUNNING;
		BOOT_STAT |= BOOT_STAT_FLASH_APP_NR;
		// Jump to the first stage bootloader.
		asm("jmp 0x1f804");
	} else if(des_updated){
		// Save the current loaded APP path, including APP name and extension, into the root directory "current.txt" file.
		// This way will know at returning from the user APP, where to save the EEPROM and in future releases the RAM content.
		app_change_extension(nameBuff, nameBuff, PSTR("app"));
		f_closedir(&dirObject);
		f_getcwd((TCHAR*)flash_buf, 0x40);
		strcat((char*)flash_buf, "/");
		strcat((char*)flash_buf, nameBuff);
		if(f_opendir(&dirObject, "/") == FR_OK) {
			f_chdir("/");
			strcpy_P(nameBuff, PSTR("current.txt"));
			if(f_open(&filObject, nameBuff, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
				char cp_path[64];
				f_read(&filObject, cp_path, f_size(&filObject), &b_read);
				if(strcpy(cp_path, (char*)flash_buf)) {
					f_rewind(&filObject);
					f_write(&filObject, (TCHAR*)flash_buf, strlen((char*)flash_buf) + 1, &b_read);
					f_truncate(&filObject);
				}
				f_close(&filObject);
				// There is an ERROR in write function, the uSD remain locked after the file is write and closed when you try to reinitialize the uSD.
				// I add a open statement this seems to unlock the uSD.
				f_open(&filObject, nameBuff, FA_READ);
			}
			f_closedir(&dirObject);
		}
		while(1);		
	}
}

inline void app_design_update(mmc_sd_t *uSD, spi_t *spi_screen, uint8_t *screen_buf) {
	if(f_opendir(&dirObject, "/") == FR_OK) {
		f_chdir("/");
		if(!gui_initialized) {
			gui_initialized = true;
			uint8_t buf[64];
			#ifdef USE_DESIGN_UPGRADE
			/*****************************************/
			// Check if a design update has been found.
			/*****************************************/
			if(app_design_app_update(uSD, spi_screen, screen_buf, true)) {
				while(1);
			}
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
					bool fallow_ok = app_fallow_path(tmp_buf, (char *)buf);
					// Extract the name with extension of the application.
					app_get_filename(filename, (char *)buf);
					/*****************************************/
					// Check if EEPROM was edited.
					/*****************************************/
					// If EEPROM was edited then the explorer was oppened after a user application was interrupted, if EEPROM was not edited then the explorer is opened after a user application was interrupted or after a power up (no need to save EEPROM to uSD).
					if(BOOT_STAT & BOOT_STAT_EEP_EDITED) {
						BOOT_STAT &= ~BOOT_STAT_EEP_EDITED;
						// Check if the path was successfully fallow.
						if(fallow_ok) {
							app_change_extension(filename, filename, PSTR("eep"));
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
}
#endif /* __UTIL_EXP_H__ */