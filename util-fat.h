/*
 * FAT FS utility file for arduFPGA designs.
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


#ifndef UTIL_FAT_H_
#define UTIL_FAT_H_

#include <string.h>
#include <avr/pgmspace.h>
#include "def.h"
#include "util.h"

void util_fat_strip_extension(char *ret_path, char *path) {
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

char *util_fat_get_path(char *ret_path, char *ptr) {
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

void util_fat_get_filename(char *ret_path, char *path) {
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

void util_fat_get_extension(char *ret_path, char *path) {
	char *path_int = path + strlen(path);
	do
	{
		path_int--;
	} while (*path_int != '.' && path_int != path);
	if(path_int != path) {
		strcpy(ret_path, path_int + 1);
	}
}

void util_fat_change_extension(char *ret_path, char *path, const char *extension) {
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

// Check selected file extension against a table with extensions, if is found will return true.
bool util_fat_is_extension(FILINFO *fInfo, const char ext_table[][4], bool ext_pgm_table) {
	uint8_t cnt = 0;
	while(1) {
		if(!ext_table[cnt][0]) {
			return false;
		}
		if(ext_pgm_table == true && !strcmp_P(fInfo->altname + strlen(fInfo->altname) - 3, ext_table[cnt])) {
			return true;
		} else if(ext_pgm_table == false && !strcmp(fInfo->altname + strlen(fInfo->altname) - 3, ext_table[cnt])) {
			return true;
		}
		cnt++;
	}
	return false;
}

int16_t util_fat_get_objects_count(DIR *dirObject, const char ext_table[][4], bool ext_pgm_table) {
	FILINFO fInfo;
	f_rewinddir(dirObject);
	int16_t count = 0;
	if(f_rewinddir(dirObject) == FR_OK) {
		while(1) {
			// Read each item in current directory.
			FRESULT res = f_readdir(dirObject, &fInfo);
			// Filter, display only files with .app extension and the sub directory's.
			if(res == FR_OK) {
				if(((fInfo.fattrib & AM_DIR) ? true : (util_fat_is_extension(&fInfo, ext_table, ext_pgm_table) && fInfo.fattrib & AM_ARC)) && strlen(fInfo.altname) < (MAX_ALLOWED_FILE_NAME_LEN_BUF - 1)) {
					count++;
				}
				} else {
				return count;
			}
		}
		} else {
		return 0;
	}
}

char **util_fat_free_objects_name_table(char **object_table) {
	uint16_t objects = 0;
	if(object_table) {
		while(object_table[objects]) {
			free(object_table[objects]);
			objects++;
		}
		free(object_table);
	}
	return NULL;
}
/*
 * First character in each string is:
 * 'D' Indicate that is a directory.
 * 'F' Indicate that is a file.
 * This will allow usage of "util_str_sort" function from util.c file that will put directories first and files after..
 */
char **util_fat_get_objects_name(DIR *dirObject, char **object_table, uint16_t *objects_count, const char ext_table[][4], bool ext_pgm_table) {
	FILINFO fInfo;
	uint16_t objects_new = util_fat_get_objects_count(dirObject, ext_table, ext_pgm_table);
	uint16_t objects_already = 0;
	if(objects_new != 0) {
		if(object_table) {
			while(object_table[objects_already]) {
				objects_already++;
			}
		}			
		object_table = realloc(object_table, (objects_already + objects_new + 1) * sizeof(object_table[0]));

		if(object_table) {
			uint16_t cnt = objects_already;
			for(; cnt < objects_already + objects_new; cnt++) {
				// Read each item in current directory.
				if(f_readdir(dirObject, &fInfo) == FR_OK) {
					// Filter, get only the directories.
					if(((fInfo.fattrib & AM_DIR) ? true : (fInfo.fattrib & AM_ARC && util_fat_is_extension(&fInfo, ext_table, ext_pgm_table))) && strlen(fInfo.altname) < (MAX_ALLOWED_FILE_NAME_LEN_BUF - 1)) {
						object_table[cnt] = malloc(strlen(fInfo.altname) + 2);
						// If out of memory, return what was able to read.
						if(!object_table[cnt]) {
							*objects_count = cnt;
							return object_table;
						}
						*object_table[cnt] = fInfo.fattrib & AM_DIR ? 'D' : 'F';
						strcpy(object_table[cnt] + 1, fInfo.altname);
					}
				} else {
					util_fat_free_objects_name_table(object_table);
				}
			}
		}
	}
	*objects_count = objects_already + objects_new;
	return object_table;
}

char **util_fat_clone_objects_name_table(char **src_object_table) {
	uint16_t objects = 0;
	char **dest_table_ptr = NULL;
	if(src_object_table) {
		while(src_object_table[objects]) {
			objects++;
		}
		dest_table_ptr = calloc(1, (objects + 1) * sizeof(dest_table_ptr[0]));
		if(dest_table_ptr) {
			int cnt = 0;
			for(; cnt < objects; cnt++) {
				dest_table_ptr[cnt] = malloc(strlen(src_object_table[cnt]) + 1);
				if(dest_table_ptr[cnt]) {
					strcpy(dest_table_ptr[cnt], src_object_table[cnt]);
					} else {
					break;
				}
			}
		}
	}
	return dest_table_ptr;
}

bool util_fat_fallow_path(DIR *dirObject, char *tmp_buf, char *path) {
	// Extract the path to the application.
	char *ptr = (char*)path;
	// Navigate to the APP directory.
	bool fallow_ok = true;
	f_closedir(dirObject);
	while ((ptr = util_fat_get_path(tmp_buf, ptr))) {
		if(f_opendir(dirObject, tmp_buf) == FR_OK){
			f_chdir(tmp_buf);
			} else {
			if(f_opendir(dirObject, "/") == FR_OK) {
				f_chdir("/");
				fallow_ok = false;
			}
			break;
		}
	}
	return fallow_ok;
}

#if defined(MAX_ALLOWED_PATH_LEN_BUF) && defined(MAX_ALLOWED_FILE_NAME_LEN_BUF)

extern DIR dirObject;
extern FIL filObject;

/*
 * This function automatically fallow the path written in the 'file_name' file on the root. //   NOT TESTED YET //
 */
bool fallow_txt_file_directory(mmc_sd_t *uSD, char *file_name, bool pgm) {
	if(f_opendir(&dirObject, "/") != FR_OK) {
		return false;
	} else {
		f_chdir("/");
		//Check for path direction file name on the root.
		char buf[MAX_ALLOWED_PATH_LEN_BUF];
		if(pgm) {
			strcpy_P(buf, file_name);
		} else {
			strcpy(buf, file_name);
		}
		if(f_open(&filObject, buf, FA_READ) != FR_OK) {
			return false;
		} else {
			UINT b_read = 0x00;
			f_rewind(&filObject);
			if(f_read(&filObject, buf, f_size(&filObject), &b_read) != FR_OK) {
				return false;
			} else {
				if(f_size(&filObject) != b_read) {
					return false;
				}
				f_close(&filObject);
				char tmp_buf[MAX_ALLOWED_PATH_LEN_BUF];
				bool status = util_fat_fallow_path(&dirObject, tmp_buf, (char *)buf);
				util_fat_get_path(tmp_buf, buf);
				f_chdir(tmp_buf);
				return status;
			}
		}
	}
}
/*
 * Each application can have a settings file that store the last path that has opened that application, this file is on the root. //   NOT TESTED YET //
 */
bool save_current_working_path(mmc_sd_t *uSD, char *file_name, bool file_name_pgm, char *stored_path, bool stored_path_pgm) {
	if(f_opendir(&dirObject, "/") != FR_OK) {
		return false;
	} else {
		f_chdir("/");
		char cp_path[MAX_ALLOWED_PATH_LEN_BUF];
		char stored_path_s[MAX_ALLOWED_PATH_LEN_BUF];
		if(f_open(&filObject, file_name, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
			UINT b_read = 0x00;
			if(stored_path_pgm) {
				strcpy_P(stored_path_s, stored_path);
			} else {
				strcpy(stored_path_s, stored_path);
			}
			f_read(&filObject, cp_path, f_size(&filObject), &b_read);
			if(strcmp(cp_path, (char*)stored_path_s)) {
				f_rewind(&filObject);
				f_write(&filObject, (TCHAR*)stored_path_s, strlen((char*)stored_path_s) + 1, &b_read);
				f_truncate(&filObject);
			}
			f_close(&filObject);
		}
		// Reopen the saved path.
		bool status = util_fat_fallow_path(&dirObject, cp_path, (char *)stored_path_s);
		util_fat_get_path(cp_path, stored_path_s);
		f_chdir(cp_path);
		return status;
	}
}
#endif



#endif /* UTIL-FAT_H_ */