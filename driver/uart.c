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

#include <stdint.h>
#include <avr/io.h>
#if defined (__AVR_XMEGA__) || defined (__AVR_MEGA__)
#include <avr/pgmspace.h>
#endif
#include "uart.h"
#include "unions.h"

void uart_init(uint32_t baud) {
	unsigned int ubrr = (F_CPU / (baud * 8)) -1;
	if(ubrr > 4095) {
		ubrr = 4095;
	}
	/* Set baud rate */
#ifdef UCSR1A
	UBRR1H = ubrr >> 8;
	UBRR1L = (uint8_t)ubrr;
	UCSR1A = (1<<U2X1);
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = (0<<USBS1) | (3<<UCSZ10);
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
#elif defined (UCSR0A)
	UBRR0H = ubrr >> 8;
	UBRR0L = (uint8_t)ubrr;
	UCSR0A = (1<<U2X0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0) | (3<<UCSZ00);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
#endif
}

void uart_put_c(uint8_t c) {
#ifdef UCSR1A
	if(! (UCSR1B & (1<<TXEN1)))
		return;
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* Put data into buffer, sends the data */
	UDR1 = c;
#elif defined (UCSR0A)
	if(! (UCSR0B & (1<<TXEN0)))
		return;
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = c;
#endif
}

#if defined (__AVR_XMEGA__) || defined (__AVR_MEGA__)
void uart_put_s_P(const char *s) {
	char c;
	while ((c = pgm_read_byte(s++))) {
		uart_put_c(c);
	}
}
#endif

void uart_put_s(char *s) {
	char c;
	while ((c = *s++)) {
		uart_put_c(c);
	}
}

void uart_print_bin_char(uint8_t c) {
	for (uint8_t cnt = 0; cnt < 8; cnt++) {
		uart_put_c(c & 0x80 ? '1' : '0');
		c = c<< 1;
	}
}

void uart_print_bin_short(uint16_t x) {
	convert16to8 uni;
	uni.i16 = x;
	uart_print_bin_char(uni.Byte1);
	uart_print_bin_char(uni.Byte0);
}

void uart_print_bin_long(uint32_t x) {
	convert32to8 uni;
	uni.i32 = x;
	uart_print_bin_char(uni.Byte3);
	uart_print_bin_char(uni.Byte2);
	uart_print_bin_char(uni.Byte1);
	uart_print_bin_char(uni.Byte0);
}

void uart_print_hex_char(uint8_t c) {
	uint8_t c1 = (c >> 4) & 0x0f, c2 = c & 0x0f;
	uart_put_c(c1 + (c1 > 9 ? ('a' - 10) : '0'));
	uart_put_c(c2 + (c2 > 9 ? ('a' - 10) : '0'));
}

void uart_print_hex_short(uint16_t c) {
	uart_print_hex_char(c >> 8);
	uart_print_hex_char(c);
}

void uart_print_hex_long(uint32_t c) {
	uart_print_hex_char(c >> 24);
	uart_print_hex_char(c >> 16);
	uart_print_hex_char(c >> 8);
	uart_print_hex_char(c);
}

int8_t uart_get_c() {
#ifdef UCSR1A
	if(! (UCSR1B & (1<<RXEN1)))
		return 0;
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	return (int8_t)UDR1;
#elif defined (UCSR0A)
	if(! (UCSR0B & (1<<RXEN0)))
		return 0;
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return (int8_t)UDR0;
#endif
}

bool uart_get_c_nb(uint8_t *c) {
#ifdef UCSR1A
	if(! (UCSR1B & (1<<RXEN1)))
		return false;
	if ( !(UCSR1A & (1<<RXC1)) )
		return false;
	else {
		asm("nop");
		*c = UDR1;
		return true;
	}
#elif defined (UCSR0A)
	if(! (UCSR0B & (1<<RXEN0)))
		return false;
	if ( !(UCSR0A & (1<<RXC0)) )
		return false;
	else {
		asm("nop");
		*c = UDR0;
		return true;
	}
#endif
}

