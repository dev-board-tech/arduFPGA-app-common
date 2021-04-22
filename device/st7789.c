/*
 * ST7789 TFT LCD display driver file for arduFPGA designs.
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

#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "delay.h"
#include "def.h"
#include "st7789.h"
#include "driver/spi.h"

#define DELAY 0x80

#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const unsigned char ST7789_BIT_MASK_TABLE [] PROGMEM = {
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000
};
#endif

// based on Adafruit ST7789 library for Arduino
static const uint8_t
cmd_240x240[] 
#if __AVR_MEGA__
 PROGMEM
#endif
= {                 		// Initialization commands for 7789 screens
	10,                       				// 9 commands in list:
	ST7789_SWRESET,   DELAY,  		// 1: Software reset, no args, w/delay
	150,                     				// 150 ms delay
	ST7789_SLPOUT ,   DELAY,  		// 2: Out of sleep mode, no args, w/delay
	255,                    				// 255 = 500 ms delay
	ST7789_COLMOD , 1+DELAY,  		// 3: Set color mode, 1 arg + delay:
	0x55,                   				// 16-bit color
	10,                     				// 10 ms delay
	ST7789_MADCTL , 1,  					// 4: Memory access ctrl (directions), 1 arg:
	0x00,                   				// Row addr/col addr, bottom to top refresh
	ST7789_CASET  , 4,  					// 5: Column addr set, 4 args, no delay:
	0x00, ST7789_240x240_XSTART,          // XSTART = 0
	(ST7789_TFTWIDTH+ST7789_240x240_XSTART) >> 8,
	(ST7789_TFTWIDTH+ST7789_240x240_XSTART) & 0xFF,   // XEND = 240
	ST7789_RASET  , 4,  					// 6: Row addr set, 4 args, no delay:
	0x00, ST7789_240x240_YSTART,          // YSTART = 0
	(ST7789_TFTHEIGHT+ST7789_240x240_YSTART) >> 8,
	(ST7789_TFTHEIGHT+ST7789_240x240_YSTART) & 0xFF,	// YEND = 240
	ST7789_INVON ,   DELAY,  		// 7: Inversion ON
	10,
	ST7789_NORON  ,   DELAY,  		// 8: Normal display on, no args, w/delay
	10,                     				// 10 ms delay
	ST7789_DISPON ,   DELAY,  		// 9: Main screen turn on, no args, w/delay
	255 };                  				// 255 = 500 ms delay


static void st7789_select() {
	//SPI_ST7789_CS_ASSERT();
}

static void st7789_unselect() {
	//SPI_ST7789_CS_DEASSERT();
}

static void st7789_reset() {
	ST7789_RST_PORT &= ~ST7789_RST_PIN;
	delay_ms(5);
	ST7789_RST_PORT |= ST7789_RST_PIN;
}

static void st7789_writeCommand(spi_t *inst, uint8_t cmd) {
	ST7789_DC_PORT &= ~ST7789_DC_PIN;
	spi_wrd_byte(inst, cmd);
}

static void st7789_writeData(spi_t *inst, uint8_t* buff, size_t buff_size) {
	ST7789_DC_PORT |= ST7789_DC_PIN;
	//spi_wrd_buf(inst, buff, buff, buff_size);
	for (int cnt = 0; cnt < buff_size; cnt++) {
		spi_wrd_byte(inst, *buff++);
	}
}

static void st7789_executeCommandList(spi_t *inst, const uint8_t *addr) {
	uint8_t numCommands, numArgs;
	uint16_t ms;

#ifdef __AVR_MEGA__
	numCommands = pgm_read_byte(addr++);
#else
	numCommands = *addr++;
#endif
	while(numCommands--) {
#ifdef __AVR_MEGA__
		uint8_t cmd = pgm_read_byte(addr++);
#else
		uint8_t cmd = *addr++;
#endif
		st7789_writeCommand(inst, cmd);

#ifdef __AVR_MEGA__
		numArgs = pgm_read_byte(addr++);
#else
		numArgs = *addr++;
#endif
		// If high bit set, delay follows args
		ms = numArgs & DELAY;
		numArgs &= ~DELAY;
		if(numArgs) {
#ifdef __AVR_MEGA__
			uint8_t tmp_buf[16];
			memcpy_P(tmp_buf, addr, numArgs);
			st7789_writeData(inst, tmp_buf, numArgs);
#else
			st7789_writeData(inst, (uint8_t*)addr, numArgs);
#endif
			addr += numArgs;
		}

		if(ms) {
#ifdef __AVR_MEGA__
			ms = pgm_read_byte(addr++);
#else
			ms = *addr++;
#endif
			//ms = *addr++;
			if(ms == 255) 
				ms = 500;
			delay_ms(ms);
		}
	}
}

static void st7789_setAddressWindow(spi_t *inst, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	// column address set
	st7789_writeCommand(inst, ST7789_CASET);
	uint8_t data[] = { 0x00, x0, 0x00, x1 };
	st7789_writeData(inst, data, sizeof(data));

	// row address set
	st7789_writeCommand(inst, ST7789_RASET);
	data[1] = y0;
	data[3] = y1;
	st7789_writeData(inst, data, sizeof(data));

	// write to RAM
	st7789_writeCommand(inst, ST7789_RAMWR);
}


void st7789_init(spi_t *inst, uint8_t *buf) {
	ST7789_DC_DIR |= ST7789_DC_PIN;
	ST7789_RST_DIR |= ST7789_RST_PIN;
    st7789_reset();
    st7789_select();
    st7789_executeCommandList(inst, cmd_240x240);
	st7789_set_rotation(inst, 0);
    st7789_unselect();
}

uint16_t st7789_get_x() {
	return 240;
}

uint16_t st7789_get_y() {
	return 240;
}

void st7789_set_rotation(spi_t *inst, uint16_t rotation) {
    st7789_select();
	st7789_writeCommand(inst, ST7789_MADCTL);
	uint8_t data = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
	switch (rotation) {
		case 90:
			data = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			break;
		case 180:
			data = ST7789_MADCTL_RGB;
			break;
		case 270:
			data = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
			break;
	}
    st7789_writeData(inst, &data, 1);
	st7789_unselect();
}

void st7789_rfsh(spi_t *inst, uint8_t *buf) {
}

void st7789_on(spi_t *inst, bool state) {
	st7789_select();
	//st7789_writeCommand(inst, state ? ST7789_NORON : ST7789_NOROFF);
	st7789_writeCommand(inst, state ? ST7789_DISPON : ST7789_DISPOFF);
	st7789_unselect();
}

void st7789_set_contrast(spi_t *inst, uint8_t cont) {
	//spi_wrd_byte(inst, 0x81);
	//spi_wrd_byte(inst, cont);
	//SPI_ST7789_CS_DEASSERT();
}

void st7789_draw_pixel(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y,
#ifdef ST7789_BW_MODE
bool
#else
uint16_t
#endif
 color) {
	/* Check if outside the display */
	if(x < 0 || y < 0 || y > st7789_get_y() - 1)
		return;
	/* Check if outside the window */
	if(box) {
		if(x < box->x_min ||
		x >= box->x_max ||
		y < box->y_min ||
		y >= box->y_max)
			return;
	}
    st7789_select();

    st7789_setAddressWindow(inst, x, y, x+1, y+1);
#ifdef ST7789_BW_MODE
	uint8_t data[2];
	if(color) {
		data[0] = 0xFF;
		data[1] = 0xFF;
	} else {
		data[0] = 0x00;
		data[1] = 0x00;
	}
#else
    uint8_t data[] = { color >> 8, color };
#endif
    st7789_writeData(inst, data, sizeof(data));
    st7789_unselect();
}

void st7789_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill, 
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = st7789_get_x();
		box__.y_min = 0;
		box__.y_max = st7789_get_y();
	}
	int16_t x_end = x + x_size ,y_end = y + y_size;
	if(x >= box__.x_max ||
	y >= box__.y_max ||
	x_end < box__.x_min ||
	y_end < box__.y_min)
		return;
	register int16_t LineCnt = y;

#ifdef ST7789_BW_MODE
	uint8_t data[2];
	if(color) {
		data[0] = 0xFF;
		data[1] = 0xFF;
	} else {
		data[0] = 0x00;
		data[1] = 0x00;
	}
#else
	uint8_t data[] = { color >> 8, color };
#endif

	if(fill) {
		if(LineCnt < box__.y_min)
			LineCnt = box__.y_min;
		int16_t _x_start = x;
		if(_x_start < box__.x_min)
			_x_start = box__.x_min;
		int16_t _x_end = x_end;
		if(_x_end > box__.x_max)
			_x_end = box__.x_max;
		int16_t width_to_refresh = (_x_end - _x_start);
		if((width_to_refresh + _x_start) > box__.x_max)
			width_to_refresh = (box__.x_max - _x_start);
		
		st7789_select();
		st7789_setAddressWindow(inst, x, LineCnt, _x_end, y_end);

		uint16_t pix_nr = (LineCnt - y_end) * (x - _x_end);
		ST7789_DC_PORT |= ST7789_DC_PIN;
		for (;pix_nr > 0; pix_nr--) {
			spi_wrd_byte(inst, data[0]);
			spi_wrd_byte(inst, data[1]);
		}
		st7789_unselect();
		return;
	}
	st7789_draw_h_line(inst, box, buf, x, x + x_size, y, 1, color);
	st7789_draw_h_line(inst, box, buf, x, x + x_size, y + y_size, 1, color);
	st7789_draw_v_line(inst, box, buf, y, y + y_size, x, 1, color);
	st7789_draw_v_line(inst, box, buf, y, y + y_size, x + x_size, 1, color);
}

void st7789_draw_h_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t x1, int16_t x2, int16_t y, uint8_t width,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color ) {
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	st7789_draw_rectangle(inst, box, buf, x1, y - Half_width1, x2, y + Half_width2, true, color);
}

void st7789_draw_v_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t y1, int16_t y2, int16_t x, uint8_t width,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color) {
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	st7789_draw_rectangle(inst, box, buf, x - Half_width1, y1, x + Half_width2, y2, true, color);
}

void st7789_clear(spi_t *inst, uint8_t *buf,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color) {
	st7789_draw_rectangle(inst, NULL, buf, 0, 0, st7789_get_x(), st7789_get_y(), true, color);
}

/*#####################################################*/
#ifdef __AVR_MEGA__
static const uint8_t CharTable6x8[] PROGMEM =
#else
static const uint8_t CharTable6x8[] =
#endif
{
	6                          ,0          ,6          ,8          ,32            ,128,
	/*  OffsetOfBeginingCharTable  ,0=Y-X|1=X-X,X-Dimension,Y-Dimension,BeginAsciiChar,EndAsciiChar*/
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x5F,0x00,0x00,0x00,0x00,0x00,//   !		32,33
	0x07,0x00,0x07,0x00,0x00,0x00,
	0x14,0x7F,0x14,0x7F,0x14,0x00,// " #		34,35
	0x24,0x2A,0x7F,0x2A,0x12,0x00,
	0x23,0x13,0x08,0x64,0x62,0x00,// 0x %		36,37
	0x36,0x49,0x55,0x22,0x50,0x00,
	0x05,0x03,0x00,0x00,0x00,0x00,// & '		38,39
	0x1C,0x22,0x41,0x00,0x00,0x00,
	0x41,0x22,0x1C,0x00,0x00,0x00,// ( )		40,41
	0x08,0x2A,0x1C,0x2A,0x08,0x00,
	0x08,0x08,0x3E,0x08,0x08,0x00,// * +		42,43
	0x50,0x30,0x00,0x00,0x00,0x00,
	0x08,0x08,0x08,0x00,0x00,0x00,// , -		44,45
	0x30,0x30,0x00,0x00,0x00,0x00,
	0x20,0x10,0x08,0x04,0x02,0x00,// . /		46,47
	0x3E,0x51,0x49,0x45,0x3E,0x00,
	0x42,0x7F,0x40,0x00,0x00,0x00,// 0 1		48,49
	0x42,0x61,0x51,0x49,0x46,0x00,
	0x21,0x41,0x45,0x4B,0x31,0x00,// 2 3		50,51
	0x18,0x14,0x12,0x7F,0x10,0x00,
	0x27,0x45,0x45,0x45,0x39,0x00,// 4 5		52,53
	0x3C,0x4A,0x49,0x49,0x30,0x00,
	0x01,0x71,0x09,0x05,0x03,0x00,// 6 7		54,55
	0x36,0x49,0x49,0x49,0x36,0x00,
	0x06,0x49,0x49,0x29,0x1E,0x00,// 8 9		56,57
	0x36,0x00,0x00,0x00,0x00,0x00,
	0x56,0x36,0x00,0x00,0x00,0x00,// : ;		58,59
	0x08,0x14,0x22,0x41,0x00,0x00,
	0x14,0x14,0x14,0x00,0x00,0x00,// < =		60,61
	0x41,0x22,0x14,0x08,0x00,0x00,
	0x02,0x01,0x51,0x09,0x06,0x00,// > ?		62,63
	0x32,0x49,0x79,0x41,0x3E,0x00,
	0x7E,0x11,0x11,0x7E,0x00,0x00,// @ A		64,65
	0x7F,0x49,0x49,0x36,0x00,0x00,
	0x3E,0x41,0x41,0x22,0x00,0x00,// B C		66,67
	0x7F,0x41,0x22,0x1C,0x00,0x00,
	0x7F,0x49,0x49,0x41,0x00,0x00,// D E		68,69
	0x7F,0x09,0x09,0x01,0x00,0x00,
	0x3E,0x41,0x51,0x32,0x00,0x00,// F G		70,71
	0x7F,0x08,0x08,0x7F,0x00,0x00,
	0x41,0x7F,0x41,0x00,0x00,0x00,// H I		72,73
	0x20,0x40,0x41,0x3F,0x01,0x00,
	0x7F,0x08,0x14,0x22,0x41,0x00,// J K		74,75
	0x7F,0x40,0x40,0x00,0x00,0x00,
	0x7F,0x02,0x04,0x02,0x7F,0x00,// L M		76,77
	0x7F,0x04,0x08,0x10,0x7F,0x00,
	0x3E,0x41,0x41,0x3E,0x00,0x00,// N O		78,79
	0x7F,0x09,0x09,0x06,0x00,0x00,
	0x3E,0x41,0x51,0x21,0x5E,0x00,// P Q		80,81
	0x7F,0x19,0x29,0x46,0x00,0x00,
	0x46,0x49,0x49,0x31,0x00,0x00,// R S		82,83
	0x01,0x7F,0x01,0x00,0x00,0x00,
	0x3F,0x40,0x40,0x3F,0x00,0x00,// T U		84,85
	0x1F,0x20,0x40,0x20,0x1F,0x00,
	0x7F,0x20,0x18,0x20,0x7F,0x00,// V W		86,87
	0x63,0x14,0x08,0x14,0x63,0x00,
	0x03,0x04,0x78,0x04,0x03,0x00,// X Y		88,89
	0x61,0x51,0x49,0x45,0x43,0x00,
	0x7F,0x41,0x41,0x00,0x00,0x00,// Z [		90,91
	0x02,0x04,0x08,0x10,0x20,0x00,
	0x41,0x41,0x7F,0x00,0x00,0x00,// \ ]		92,93
	0x04,0x02,0x01,0x02,0x04,0x00,
	0x40,0x40,0x40,0x00,0x00,0x00,// ^ _		94,95
	0x01,0x02,0x04,0x00,0x00,0x00,
	0x20,0x54,0x54,0x78,0x00,0x00,// ` a		96,97
	0x7F,0x48,0x44,0x38,0x00,0x00,
	0x38,0x44,0x44,0x00,0x00,0x00,// b c		98,99
	0x38,0x44,0x48,0x7F,0x00,0x00,
	0x38,0x54,0x54,0x18,0x00,0x00,// d e		100,101
	0x08,0x7E,0x09,0x01,0x00,0x00,
	0x08,0x14,0x54,0x3C,0x00,0x00,// f g		102,103
	0x7F,0x08,0x04,0x78,0x00,0x00,
	0x44,0x7D,0x40,0x00,0x00,0x00,// h i		104,105
	0x20,0x40,0x44,0x3D,0x00,0x00,
	0x7F,0x10,0x28,0x44,0x00,0x00,// j k		106,107
	0x41,0x7F,0x40,0x00,0x00,0x00,
	0x7C,0x04,0x18,0x04,0x78,0x00,// l m		108,109
	0x7C,0x08,0x04,0x78,0x00,0x00,
	0x38,0x44,0x44,0x38,0x00,0x00,// n o		110,111
	0x7C,0x14,0x14,0x08,0x00,0x00,
	0x08,0x14,0x18,0x7C,0x00,0x00,// p q		112,113
	0x7C,0x08,0x04,0x08,0x00,0x00,
	0x48,0x54,0x54,0x20,0x00,0x00,// r s		114,115
	0x04,0x3F,0x44,0x40,0x00,0x00,
	0x3C,0x40,0x20,0x7C,0x00,0x00,// t u		116,117
	0x1C,0x20,0x40,0x20,0x1C,0x00,
	0x3C,0x40,0x30,0x40,0x3C,0x00,// v w		118,119
	0x44,0x28,0x10,0x28,0x44,0x00,
	0x0C,0x50,0x50,0x3C,0x00,0x00,// x y		120,121
	0x44,0x64,0x54,0x4C,0x44,0x00,
	0x08,0x36,0x41,0x00,0x00,0x00,// z {		122,123
	0x7F,0x00,0x00,0x00,0x00,0x00,
	0x41,0x36,0x08,0x00,0x00,0x00,// | }		124,125
	0x08,0x2A,0x1C,0x08,0x00,0x00,
	0x08,0x1C,0x2A,0x08,0x00,0x00,// -> <-		126,127
	0x14,0x36,0x77,0x36,0x14,0x00 };//			128

int st7789_draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 foreColor,
 #ifdef ST7789_BW_MODE
  bool
 #else
  uint16_t
 #endif
 inkColor) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = st7789_get_x();
		box__.y_min = 0;
		box__.y_max = st7789_get_y();
	}
	char *pcString = string;
	bool WordWrap = wordWrap;
	//s32 _SelStart = properties->_SelStart;
	//s32 _SelLen = properties->_SelLen;

	int8_t chWidth = 0;
	int8_t chHeight = 0;
	int16_t CharPtr;
	int8_t Tmp = 0;
	int16_t Cursor_X = x;
	int16_t Cursor_Y = y;
	bool ulVisible = true;
	int16_t CharCnt = 0;
	bool ulOpaque = false;
#ifdef __AVR_MEGA__
	chWidth = pgm_read_byte(&CharTable6x8[2]);
	chHeight = pgm_read_byte(&CharTable6x8[3]);
#else
	chWidth = CharTable6x8[2];
	chHeight = CharTable6x8[3];
#endif
	do {
		int8_t Char = *pcString;
		if (Char == 0) {
			return CharCnt - 1;
		}
#ifdef __AVR_MEGA__
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if (Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
			} else {
			int8_t Temp;
			/* if CompactWriting is true search the character for free cols from right to left and clear them */
			if (!terminalMode) {
				for (Tmp = 1; Tmp < chWidth; Tmp++) {
#ifdef __AVR_MEGA__
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if (Temp == 0)
						break;
				}
				Tmp++;
			}
			else {
				Tmp = chWidth;
			}
			if (Cursor_X + Tmp >= box__.x_min
			&& Cursor_X < box__.x_max + Tmp
			&& Cursor_Y + chHeight >= box__.y_min
			&& Cursor_Y < box__.y_max + chHeight) {
				if (ulVisible) {
					int16_t XX = 0;
					int16_t YY = 0;
					for (XX = 0; XX < Tmp; XX++) {
#ifdef __AVR_MEGA__
						Temp = pgm_read_byte(&CharTable6x8[XX + CharPtr]);
#else
						Temp = CharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++) {
							if (Temp & 0x1) {
								st7789_draw_pixel(inst, &box__, buf,
								XX + Cursor_X, YY + Cursor_Y, inkColor);
							}
							else {
								if (ulOpaque)
								st7789_draw_pixel(inst, &box__, buf,
								XX + Cursor_X, YY + Cursor_Y, foreColor);
							}
							Temp = Temp >> 1;
						}
					}
				}
			}
		}
		switch (Char) {
			case '\r':
				Cursor_X = x;
				pcString++;
				break;
			case '\n':
				Cursor_Y += chHeight;
				pcString++;
				break;
			default:
				Cursor_X += Tmp;
				if ((Cursor_X + chWidth > box__.x_max)
				&& WordWrap == true) {
					Cursor_Y += chHeight;
					Cursor_X = x;
				}
				pcString++;
		}
		CharCnt++;
	} while (1);
}
//#######################################################################################
