/*
 * Utility file for arduFPGA designs.
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


#include <ctype.h>
#include <stdint.h>

#include <string.h>
#include "util.h"

bool util_is_hex(char src) {
	int tmp = tolower((int)src);
	if((tmp < '0' || tmp > '9') && (tmp < 'a' || tmp > 'f'))
		return false;
	else
		return true;
}

void util_char_to_hex(char *ptr, uint8_t c) {
	uint8_t c1 = (c >> 4) & 0x0f, c2 = c & 0x0f;
	ptr[0] = (c1 + (c1 > 9 ? ('a' - 10) : '0'));
	ptr[1] = (c2 + (c2 > 9 ? ('a' - 10) : '0'));
	ptr[2] = 0;
}

void util_short_to_hex(char *ptr, uint16_t c) {
	util_char_to_hex(ptr, c);
	util_char_to_hex(ptr + 2, c >> 8);
	ptr[4] = 0;
}

void util_long_to_hex(char *ptr, uint32_t c) {
	util_char_to_hex(ptr, c);
	util_char_to_hex(ptr + 2, c >> 8);
	util_char_to_hex(ptr + 4, c >> 16);
	util_char_to_hex(ptr + 6, c >> 24);
	ptr[8] = 0;
}

void util_get_hex_buf(char *hex_str, unsigned char *data, unsigned int data_len) {
	unsigned int cnt = 0;
	char tmp_str[2];
	for(; cnt < data_len; cnt++) {
		util_char_to_hex(tmp_str, data[cnt]);
		hex_str[cnt * 2] = tmp_str[0];
		hex_str[(cnt * 2) + 1] = tmp_str[1];
	}
	hex_str[cnt * 2] = 0;
}

bool util_get_bin_from_hex_char(unsigned char *dest, char src) {
	int tmp = tolower((int)src);
	if((tmp < '0' || tmp > '9') && (tmp < 'a' || tmp > 'f'))
		return false;
	if(tmp <= '9')
		*dest = (tmp - '0') & 0x0F;
	else {
		*dest = ((tmp - 'a') + 10) & 0x0F;
	}
	return true;
}

unsigned int util_get_bin_from_hex_buf(unsigned char *bin_buff, char *data, unsigned int dest_buff_len) {
	unsigned int cnt = 0;
	while(*data != 0 && dest_buff_len != 0) {
		unsigned char tmp0 = 0;
		unsigned char tmp1 = 0;
		if(!util_get_bin_from_hex_char(&tmp1, *data++))
			return 0;
		if(!util_get_bin_from_hex_char(&tmp0, *data++))
			return 0;
		bin_buff[cnt++] = (tmp1 << 4) + tmp0;
		dest_buff_len--;
	}
	return cnt;
}

void util_bin_to_hex_string(char *dest_buf, unsigned char *src_buf, unsigned int str_buf_len) {
	for(unsigned int cnt = 0; cnt < str_buf_len; cnt++) {
		util_char_to_hex(dest_buf, *src_buf++);
		dest_buf += 2;
	}
}

uint8_t util_dec_to_uchar(char *buf) {
	return 0;
}
/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
int32_t util_int_to_percent(int32_t maxPercentageValue, int32_t minValue, int32_t maxValue, int32_t value) {
	int32_t ReturnedValue = 0;
	if(value > maxValue)
		value = maxValue;
	else if(value < minValue)
		value = minValue;
	if (maxValue < 65536)
		ReturnedValue = ((value - minValue) * 0x10000) / (((maxValue - minValue) * 0x10000) / maxPercentageValue);
	else
		ReturnedValue = (value - minValue) / ((maxValue - minValue) / maxPercentageValue);
	if(ReturnedValue > maxPercentageValue)
		ReturnedValue = maxPercentageValue;
	else if(ReturnedValue < 0)
		ReturnedValue = 0;
	return ReturnedValue;
}

/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
uint32_t util_uint_to_percent(uint32_t maxPercentageValue, uint32_t minValue, uint32_t maxValue, uint32_t value) {
	uint32_t ReturnedValue = 0;
	if (maxValue < 65536)
		ReturnedValue = ((value - minValue) * 0x10000) / (((maxValue - minValue) * 0x10000) / maxPercentageValue);
	else
		ReturnedValue = (value - minValue) / ((maxValue-minValue) / maxPercentageValue);
	if(ReturnedValue > maxPercentageValue)
		ReturnedValue = maxPercentageValue;
	else if(ReturnedValue < 0)
		ReturnedValue = 0;
	return ReturnedValue;
}

/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
double util_double_to_percent(double maxPercentageValue, double minValue, double maxValue, double value) {
	double ReturnedValue = 0;
	ReturnedValue = (value- minValue) / ((maxValue-minValue) / maxPercentageValue);
	if(ReturnedValue > maxPercentageValue)
		ReturnedValue = maxPercentageValue;
	else if(ReturnedValue < 0)
		ReturnedValue = 0;
	return ReturnedValue;
}


/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue with decimals.
 * At this moment does not return decimals :)
 */
ToPercentageWithDecimals_t util_int_to_percent_decimal(int32_t maxPercentageValue, int32_t minValue, int32_t maxValue, int32_t value) {
	ToPercentageWithDecimals_t Return;
	Return.Decimal = 0;
	if (maxValue < 65536) { // Increase the precision of the result.
		Return.Value = (value * 0x10000) / (((maxValue - minValue) * 0x10000) / maxPercentageValue);
	} else {
		Return.Value = value / ((maxValue-minValue) / maxPercentageValue);
	}
	return Return;
}

/*
 * This function convert a Value between 0 and MaxPercentageValue to a value between MinValue and MaxValue.
 */
int32_t util_percent_to_int(int32_t minValue, int32_t maxValue, int32_t maxPercentageValue, int32_t value) {
	int32_t result = (signed long long)((value * (maxValue - minValue)) / maxPercentageValue) + minValue;
	if(result > maxValue)
		result = maxValue;
	else if(result < minValue)
		result = minValue;
	return result;
}

/*
 * This function convert a Value between 0 and MaxPercentageValue to a value between MinValue and MaxValue.
 */
double util_percent_to_double(double minValue, double maxValue, double maxPercentageValue, double value) {
	double result = ((value * (maxValue - minValue)) / maxPercentageValue) + minValue;
	if(result > maxValue)
		result = maxValue;
	else if(result < minValue)
		result = minValue;
	return result;
}

void util_str_sort(int n, char **x) {
	char *temp; // Pointer declaration
	int i,str;
	for(str = 0; str < n-1; ++str) {
		for(i = str+1; i < n; ++i) {
			if(strcmp(x[str],x[i]) > 0) {
				temp = x[str]; // compared string being stored in temp
				x[str] = x[i];
				x[i] = temp;
			}
		}
	}
	return;
}