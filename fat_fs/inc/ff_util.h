/*
 * FAT filesystem utility for BOOT-LOADRER of ARDUFPGA soft core design.
 * 
 * Copyright (C) 2016  Iulian Gheorghiu (morgoth@devboard.tech)
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

#ifndef LIB_LIB_FAT_FS_INC_FF_UTIL_H_
#define LIB_LIB_FAT_FS_INC_FF_UTIL_H_

#include "../inc/ff.h"

void ff_util_reset_chk();
unsigned long ff_util_get_chk();

FRESULT ff_util_write (
	FIL* fp,			/* Pointer to the file object */
	const void* buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT* bw			/* Pointer to number of bytes written */
);

int ff_util_gets (
	TCHAR* buff,	/* Pointer to the string buffer to read */
	int len,		/* Size of string buffer (characters) */
	FIL* fp			/* Pointer to the file object */
);

int ff_util_putc (
	TCHAR c,	/* A character to be output */
	FIL* fp		/* Pointer to the file object */
);

int ff_util_puts (
	const TCHAR* str,	/* Pointer to the string to be output */
	FIL* fp				/* Pointer to the file object */
);

int ff_util_appendhexs (
	const unsigned char* buff,	/* Pointer to the buffer to be output */
	const unsigned int buff_len,/* Number of bytes to be output */
	FIL* fp				/* Pointer to the file object */
);

int ff_util_put_nextline (
	FIL* fp				/* Pointer to the file object */
);

int ff_util_printf(
		FIL* fp,			/* Pointer to the file object */
		const TCHAR* str,	/* Pointer to the format string */
		...					/* Optional arguments... */
);

FRESULT ff_util_seek (
	FIL* fp,				/* Pointer to the file object */
	unsigned int location
);

FRESULT ff_util_seek_eof (
	FIL* fp				/* Pointer to the file object */
);

FRESULT ff_util_file_exist (
		const TCHAR* str	/* Pointer to the string to be output */
);

#endif /* LIB_LIB_FAT_FS_INC_FF_UTIL_H_ */
