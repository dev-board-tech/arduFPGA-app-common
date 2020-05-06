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

#include <stdbool.h>
#include <ctype.h>
#include "hex_string.h"

void GetHexChar(char *hex_str, unsigned char data)
{
	char char1 = (data >> 4) & 0x0F;
	char char2 = data & 0x0F;
	if(char1 <= 9)
		hex_str[0] = (char1 + '0');
	else
		hex_str[0] = ((char1 - 10) + 'A');
	if(char2 <= 9)
		hex_str[1] = (char2 + '0');
	else
		hex_str[1] = ((char2 - 10) + 'A');
}

void GetHexBuff(char *hex_str, unsigned char *data, unsigned int data_len)
{
	unsigned int cnt = 0;
	char tmp_str[2];
	for(; cnt < data_len; cnt++)
	{
		GetHexChar(tmp_str, data[cnt]);
		hex_str[cnt * 2] = tmp_str[0];
		hex_str[(cnt * 2) + 1] = tmp_str[1];
	}
	hex_str[cnt * 2] = 0;
}

bool GetBinFromHexChar(unsigned char *dest, char src)
{
	int tmp = tolower((int)src);
	if((tmp < '0' || tmp > '9') && (tmp < 'a' || tmp > 'f'))
		return false;
	if(tmp <= '9')
		*dest = (tmp - '0') & 0x0F;
	else
	{
		*dest = ((tmp - 'a') + 10) & 0x0F;
	}
	return true;
}

unsigned int GetBinFromHexBuff(unsigned char *bin_buff, char *data, unsigned int dest_buff_len)
{
	unsigned int cnt = 0;
	while(*data != 0 && dest_buff_len != 0)
	{
		unsigned char tmp0 = 0;
		unsigned char tmp1 = 0;
		if(!GetBinFromHexChar(&tmp1, *data++))
			return 0;
		if(!GetBinFromHexChar(&tmp0, *data++))
			return 0;
		bin_buff[cnt++] = (tmp1 << 4) + tmp0;
		dest_buff_len--;
	}
	return cnt;
}


