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



#endif /* UTIL-FAT_H_ */