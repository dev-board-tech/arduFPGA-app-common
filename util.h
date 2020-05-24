/*
 * util.h
 *
 * Created: 24.05.2020 13:23:31
 *  Author: MorgothCreator
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

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





#endif /* UTIL_H_ */