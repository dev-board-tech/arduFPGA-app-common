/*
 * Unions utility for BOOT-LOADRER of ARDUFPGA soft core design.
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
//convert32to8 ByteToWrite_;
//ByteToWrite_.i32 = 32857632847;
//PORTA = ByteToWrite_.Byte0;
//PORTB = ByteToWrite_.Byte1;
//PORTC = ByteToWrite_.Byte2;
//PORTD = ByteToWrite_.Byte3;


//convert8to32 LoongToRead_;
//LoongToRead_.Byte0 = RamDisk_DataTable[FileAddress];
//LoongToRead_.Byte1 = RamDisk_DataTable[FileAddress + 1];
//LoongToRead_.Byte2 = RamDisk_DataTable[FileAddress + 2];
//LoongToRead_.Byte3 = RamDisk_DataTable[FileAddress + 3];
//return LoongToRead_.LoongReturn;
//###########################################################################
typedef union 
{
unsigned short i16;
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
	};
} convert16to8;
//###########################################################################
typedef union 
{
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
	};unsigned short ShortReturn;
}convert8to16;
//###########################################################################
typedef union 
{
unsigned long i32;
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};
}convert32to8;
//###########################################################################
typedef union 
{
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
	};unsigned long LongReturn;
}convert8to32;
//###########################################################################
typedef union 
{
double i48;
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
	};
}convert48to8;
//###########################################################################
typedef union 
{
	struct 
	{
		unsigned char Byte0;
		unsigned char Byte1;
		unsigned char Byte2;
		unsigned char Byte3;
		unsigned char Byte4;
		unsigned char Byte5;
	};double Return48;
}convert8to48;
//###########################################################################
typedef union 
{
double i64;
	struct 
	{
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
typedef union 
{
	struct 
	{
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
//###########################################################################
#endif
