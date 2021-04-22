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


#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdbool.h>

bool util_is_hex(char src);
void util_char_to_hex(char *ptr, uint8_t c);
void util_short_to_hex(char *ptr, uint16_t c);
void util_long_to_hex(char *ptr, uint32_t c);
void util_get_hex_buf(char *hex_str, unsigned char *data, unsigned int data_len);
bool util_get_bin_from_hex_char(unsigned char *dest, char src);
unsigned int util_get_bin_from_hex_buf(unsigned char *bin_buff, char *data, unsigned int dest_buff_len);
void util_bin_to_hex_string(char *dest_buf, unsigned char *src_buf, unsigned int str_buf_len);
uint8_t util_dec_to_uchar(char *buf);
/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
int32_t util_int_to_percent(int32_t maxPercentageValue, int32_t minValue, int32_t maxValue, int32_t value);
/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
uint32_t util_uint_to_percent(uint32_t maxPercentageValue, uint32_t minValue, uint32_t maxValue, uint32_t value);
/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue.
 */
double util_double_to_percent(double maxPercentageValue, double minValue, double maxValue, double value);

typedef struct ToPercentageWithDecimals_s
{
	int32_t Value;
	int8_t Decimal;
}ToPercentageWithDecimals_t;

/*
 * This function convert a Value between MinValue and MaxValue to a fit into a value from 0 to MaxPercentageValue with decimals.
 * At this moment does not return decimals :)
 */
ToPercentageWithDecimals_t util_int_to_percent_decimal(int32_t maxPercentageValue, int32_t minValue, int32_t maxValue, int32_t value);
/*
 * This function convert a Value between 0 and MaxPercentageValue to a value between MinValue and MaxValue.
 */
int32_t util_percent_to_int(int32_t minValue, int32_t maxValue, int32_t maxPercentageValue, int32_t value);
/*
 * This function convert a Value between 0 and MaxPercentageValue to a value between MinValue and MaxValue.
 */
double util_percent_to_double(double minValue, double maxValue, double maxPercentageValue, double value);

void util_str_sort(int n, char **x);

typedef struct {
	uint16_t size;
	char *str;
}string;


#endif /* UTIL_H_ */