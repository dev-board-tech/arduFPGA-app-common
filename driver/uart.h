/*
 * UART driver file for arduFPGA design.
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


#ifndef UART_H_
#define UART_H_

#include "def.h"

#include "stdbool.h"

void uart_init(uint32_t baud);
void uart_put_c(uint8_t c);
void uart_put_s(char *s);
#if defined (__AVR_XMEGA__) || defined (__AVR_MEGA__)
void uart_put_s_P(const char *s);
#endif
void uart_print_bin_char(uint8_t c);
void uart_print_bin_short(uint16_t x);
void uart_print_bin_long(uint32_t x);
void uart_print_hex_char(uint8_t c);
void uart_print_hex_short(uint16_t c);
void uart_print_hex_long(uint32_t c);
int8_t uart_get_c();
bool uart_get_c_nb(uint8_t *c);

#endif /* UART_H_ */
