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
#include "mmc_sd_spi.h"
#include "fat_fs/inc/ff.h"

void util_fat_strip_extension(char *ret_path, char *path);
char *util_fat_get_path(char *ret_path, char *ptr);
void util_fat_get_filename(char *ret_path, char *path);
void util_fat_get_extension(char *ret_path, char *path);
void util_fat_change_extension(char *ret_path, char *path, const char *extension);
// Check selected file extension against a table with extensions, if is found will return true.
bool util_fat_is_extension(FILINFO *fInfo, const char ext_table[][4], bool ext_pgm_table);
int16_t util_fat_get_objects_count(DIR *dirObject, const char ext_table[][4], bool ext_pgm_table);
char **util_fat_free_objects_name_table(char **object_table);
/*
 * First character in each string is:
 * 'D' Indicate that is a directory.
 * 'F' Indicate that is a file.
 * This will allow usage of "util_str_sort" function from util.c file that will put directories first and files after..
 */
char **util_fat_get_objects_name(DIR *dirObject, char **object_table, uint16_t *objects_count, const char ext_table[][4], bool ext_pgm_table);
char **util_fat_clone_objects_name_table(char **src_object_table);
bool util_fat_fallow_path(DIR *dirObject, char *tmp_buf, char *path);
#if defined(MAX_ALLOWED_PATH_LEN_BUF) && defined(MAX_ALLOWED_FILE_NAME_LEN_BUF)

extern DIR dirObject;
extern FIL filObject;

/*
 * This function automatically fallow the path written in the 'file_name' file on the root. //   NOT TESTED YET //
 */
bool fallow_txt_file_directory(mmc_sd_t *uSD, char *file_name, bool pgm);
/*
 * Each application can have a settings file that store the last path that has opened that application, this file is on the root. //   NOT TESTED YET //
 */
bool save_current_working_path(mmc_sd_t *uSD, char *file_name, bool file_name_pgm, char *stored_path, bool stored_path_pgm);
#endif

void loadAppPath();
uint16_t moveString(char *from, char *to);

#endif /* UTIL-FAT_H_ */