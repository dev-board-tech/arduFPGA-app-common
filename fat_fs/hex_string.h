/*
 * HEX string utility for BOOT-LOADRER of ARDUFPGA soft core design.
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

#ifndef LIB_LIB_UTIL_HEX_STRING_H_
#define LIB_LIB_UTIL_HEX_STRING_H_

void GetHexChar(char *hex_str, unsigned char data);
void GetHexBuff(char *hex_str, unsigned char *data, unsigned int data_len);
bool GetBinFromHexChar(unsigned char *dest, char src);
unsigned int GetBinFromHexBuff(unsigned char *bin_buff, char *data, unsigned int dest_buff_len);


#endif /* LIB_LIB_UTIL_HEX_STRING_H_ */
