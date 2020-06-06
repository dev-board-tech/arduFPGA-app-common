/*
 * Unions utility file for arduFPGA designs
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

#ifndef __UNIONS__
#define __UNIONS__
//Example of usage.
/*
convert32to8 ByteToWrite_;
ByteToWrite_.i32 = 32857632847;
PORTA = ByteToWrite_.Byte0;
PORTB = ByteToWrite_.Byte1;
PORTC = ByteToWrite_.Byte2;
PORTD = ByteToWrite_.Byte3;
*/
/*
convert8to32 LoongToRead_;
LoongToRead_.Byte0 = RamDisk_DataTable[FileAddress];
LoongToRead_.Byte1 = RamDisk_DataTable[FileAddress + 1];
LoongToRead_.Byte2 = RamDisk_DataTable[FileAddress + 2];
LoongToRead_.Byte3 = RamDisk_DataTable[FileAddress + 3];
return LoongToRead_.LoongReturn;
*/
//###########################################################################
typedef union {
unsigned short i16;
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
	};
} convert16to8;
//###########################################################################
typedef union {
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
	};unsigned short ShortReturn;
}convert8to16;
//###########################################################################
typedef union {
unsigned long i32;
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};
}convert32to8;
//###########################################################################
typedef union {
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};unsigned long LongReturn;
}convert8to32;
//###########################################################################
typedef union {
double i48;
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
	};
}convert48to8;
//###########################################################################
typedef union {
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
	};double Return48;
}convert8to48;
//###########################################################################
typedef union {
double i64;
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
		unsigned char Byte6;
		unsigned char Byte7;
	};
}convert64to8;
//###########################################################################
typedef union {
	struct {
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
		unsigned char Byte6;
		unsigned char Byte7;
	};double Return64;
}convert8to64;
//###########################################################################
inline uint16_t uni_8_to_16(uint8_t b1, uint8_t b0) {
	convert8to16 uni;
	uni.Byte0 = b0;
	uni.Byte1 = b1;
	return uni.ShortReturn;
}

inline uint32_t uni_8_to_32(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) {
	convert8to32 uni;
	uni.Byte0 = b0;
	uni.Byte1 = b1;
	uni.Byte2 = b2;
	uni.Byte3 = b3;
	return uni.LongReturn;
}

inline uint64_t uni_8_to_64(uint8_t b7, uint8_t b6, uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) {
	convert8to64 uni;
	uni.Byte0 = b0;
	uni.Byte1 = b1;
	uni.Byte2 = b2;
	uni.Byte3 = b3;
	uni.Byte4 = b4;
	uni.Byte5 = b5;
	uni.Byte6 = b6;
	uni.Byte7 = b7;
	return uni.Return64;
}
//###########################################################################
#endif
