/*
 * SSD1306 OLED display driver file for arduFPGA designs.
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
#include <string.h>
#include "gfx/draw.h"
#include "delay.h"
#include "driver/spi.h"
#include "ssd1306.h"

#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const unsigned char SSD1306_BIT_MASK_TABLE [] PROGMEM = {
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

void ssd1306_init(spi_t *inst, uint8_t *buf) {
	SSD1306_DC_DIR |= SSD1306_DC_PIN;
	SSD1306_RST_DIR |= SSD1306_RST_PIN;
	SSD1306_RST_PORT &= ~SSD1306_RST_PIN;
	delay_ms(2);
	SSD1306_RST_PORT |= SSD1306_RST_PIN;
	delay_ms(10);

	ssd1306_wr_cmd(inst, SSD1306_DISPLAYOFF);
	ssd1306_wr_cmd(inst, SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_wr_cmd(inst, 0xF0);
	ssd1306_wr_cmd(inst, SSD1306_CHARGEPUMP);
	ssd1306_wr_cmd(inst, 0x14);
	ssd1306_wr_cmd(inst, SSD1306_SEGREMAP | 0x1);
	ssd1306_wr_cmd(inst, SSD1306_COMSCANDEC);
	ssd1306_wr_cmd(inst, SSD1306_SETCONTRAST);
	ssd1306_wr_cmd(inst, 0x8F);
	ssd1306_wr_cmd(inst, SSD1306_SETPRECHARGE);
	ssd1306_wr_cmd(inst, 0xF1);
	ssd1306_wr_cmd(inst, SSD1306_MEMORYMODE);
	ssd1306_wr_cmd(inst, 0x00);
	
	delay_ms(2);
	ssd1306_wr_cmd(inst, SSD1306_DISPLAYON);
	
#ifdef SSD1306_USE_NO_BUF
	ssd1306_wr_cmd(inst, 0x21);
	ssd1306_wr_cmd(inst, 0x00);
	ssd1306_wr_cmd(inst, 0x7F);
	ssd1306_wr_cmd(inst, 0x22);
	ssd1306_wr_cmd(inst, 0x00);
	ssd1306_wr_cmd(inst, 0x07);
	SSD1306_DC_PORT |= SSD1306_DC_PIN;
#endif
	uint8_t *_buf = buf;
	for(uint16_t cnt = 0; cnt < 1024; cnt++)
	{
#ifndef SSD1306_USE_NO_BUF
		*_buf++ = 0x00;
#else
		spi_wrd_byte(inst, 0x00);
#endif
	}
	SPI_SSD1306_CS_DEASSERT();
	ssd1306_rfsh(inst, buf);
}

void ssd1306_wr_cmd(spi_t *inst, uint8_t cmd) {
	SSD1306_DC_PORT &= ~SSD1306_DC_PIN;
	SPI_SSD1306_CS_ASSERT();
	spi_wrd_byte(inst, cmd);
}

uint16_t ssd1306_get_x(void) {
	return 128;
}

uint16_t ssd1306_get_y(void) {
	return 64;
}

void ssd1306_rfsh(spi_t *inst, uint8_t *buf) {
	/* Set start line */
	//ssd1306_wr_cmd(0x40);

#ifndef SSD1306_USE_NO_BUF
	SPI_SSD1306_CS_ASSERT();
	ssd1306_wr_cmd(inst, 0x21);
	ssd1306_wr_cmd(inst, 0x00);
	ssd1306_wr_cmd(inst, 0x7F);
	ssd1306_wr_cmd(inst, 0x22);
	ssd1306_wr_cmd(inst, 0x00);
	ssd1306_wr_cmd(inst, 0x07);
	SSD1306_DC_PORT |= SSD1306_DC_PIN;
	spi_wr_buf(inst, buf, 1024);
	SPI_SSD1306_CS_DEASSERT();
#endif
	ssd1306_on(inst, true);
}

void ssd1306_on(spi_t *inst, bool state) {
	ssd1306_wr_cmd(inst, state ? 0xAF : 0xAE);
	SPI_SSD1306_CS_DEASSERT();
}

void ssd1306_set_contrast(spi_t *inst, uint8_t cont) {
	ssd1306_wr_cmd(inst, 0x81);
	ssd1306_wr_cmd(inst, cont);
	SPI_SSD1306_CS_DEASSERT();
}

void ssd1306_draw_pixel(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y, bool state) {
#ifndef SSD1306_USE_NO_BUF
	/* Check if outside the display */
	if(x < 0 || y < 0 || y > 63)
		return;
	/* Check if outside the window */
	if(box) {
		if(x < box->x_min ||
			x >= box->x_max ||
				y < box->y_min ||
					y >= box->y_max)
			return;
	}
	/* Calculate the byte where the bit will be written. */
	unsigned char *tmp_buff = buf + (((y >> 3) * 128) + x);
	/* Create the mask of the bit that will be edited inside the selected byte. */
	uint8_t mask;
#if __AVR_XMEGA__
		mask = pgm_read_byte(&SSD1306_BIT_MASK_TABLE[y & 0x07]);
#else
		mask = 1 << (y & 0x07);
#endif
	if (state)
		*tmp_buff |= mask;
	else
		*tmp_buff &= ~mask;
#endif
}

void ssd1306_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill, bool state) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 128;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t x_end = x + x_size ,y_end = y + y_size;
	if(x >= box__.x_max ||
		y >= box__.y_max ||
			x_end < box__.x_min ||
				y_end < box__.y_min)
		return;
	register int16_t LineCnt = y;
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
#ifndef SSD1306_USE_NO_BUF
		for( ; LineCnt < y_end; LineCnt++) {
			if(LineCnt >= box__.y_max)
				return;
			register int16_t x = _x_start;
			for( ; x < _x_end ; x++) {
				ssd1306_draw_pixel(inst, &box__, buf, x, LineCnt, state);
			}
		}
#else
		for( ; LineCnt < y_end; LineCnt+=8) {
			if(LineCnt >= box__.y_max)
				return;
			register int16_t x = _x_start;
			for( ; x < _x_end ; x++) {
				ssd1306_wr_cmd(inst, 0x21);
				ssd1306_wr_cmd(inst, x);
				ssd1306_wr_cmd(inst, x);
				ssd1306_wr_cmd(inst, 0x22);
				ssd1306_wr_cmd(inst, LineCnt >> 3);
				ssd1306_wr_cmd(inst, LineCnt >> 3);
				SSD1306_DC_PORT |= SSD1306_DC_PIN;
				spi_wrd_byte(inst, state ? 0xFF : 0x00);
				SPI_SSD1306_CS_DEASSERT();
			}
		}
#endif
		return;
	}
#ifndef SSD1306_USE_NO_BUF
	int16_t _x_end = x_end;
	int16_t _x_start = x;
	if(_x_end > box__.x_max) 
		_x_end = box__.x_max;
	if(_x_start < box__.x_min) 
		_x_start = box__.x_min;
	if(y >= box__.y_min) {
		for(LineCnt = _x_start ; LineCnt < _x_end ; LineCnt++) {
			ssd1306_draw_pixel(inst, &box__, buf, LineCnt, y, state);
		}
	}

	if(y_end <= box__.y_max) {
		for(LineCnt = _x_start ; LineCnt < _x_end ; LineCnt++) {
			ssd1306_draw_pixel(inst, &box__, buf, LineCnt, y_end - 1, state);
		}
	}

	int16_t _y_end = y_end;
	if(_y_end > box__.y_max) 
		_y_end = box__.y_max;
	int16_t _y_start = y;
	if(_y_start < box__.y_min) 
		_y_start = box__.y_min;
	if(x >= box__.x_min) {
		for(LineCnt = _y_start ; LineCnt < _y_end ; LineCnt++) {
			ssd1306_draw_pixel(inst, &box__, buf, x, LineCnt, state);
		}
	}

	if(x_end <= box__.x_max) {
		for(LineCnt = _y_start ; LineCnt < _y_end ; LineCnt++) {
			ssd1306_draw_pixel(inst, &box__, buf, (x_end - 1), LineCnt, state);
		}
	}
#endif
}

void ssd1306_draw_h_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t x1, int16_t x2, int16_t y, uint8_t width, bool state) {
#ifndef SSD1306_USE_NO_BUF
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 128;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t X1_Tmp = x1, X2_Tmp = x1 + x2;
	if(X1_Tmp < (int16_t)box__.x_min)
		X1_Tmp = (int16_t)box__.x_min;
	if(X1_Tmp >= (int16_t)box__.x_max)
		X1_Tmp = (int16_t)box__.x_max;
	if(X2_Tmp < (int16_t)box__.x_min)
		X2_Tmp = (int16_t)box__.x_min;
	if(X2_Tmp >= (int16_t)box__.x_max)
		X2_Tmp = (int16_t)box__.x_max;
	if(y < (int16_t)box__.y_min)
		y = (int16_t)box__.y_min;
	if(y >= (int16_t)box__.y_max)
		y = (int16_t)box__.y_max;
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	for(;X1_Tmp < X2_Tmp; X1_Tmp++) {
		int16_t _Y_ = y - Half_width1;
		for(; _Y_ < y + Half_width2; _Y_++)
		ssd1306_draw_pixel(inst, &box__, buf, (int16_t)(X1_Tmp), (int16_t)(_Y_), state);
	}
#endif
}

void ssd1306_draw_v_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t y1, int16_t y2, int16_t x, uint8_t width, bool state) {
#ifndef SSD1306_USE_NO_BUF
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 128;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t Y1_Tmp = y1, Y2_Tmp = y1 + y2;
	if(x < (int16_t)box__.x_min)
		x = (int16_t)box__.x_min;
	if(x >= (int16_t)box__.x_max)
		x = (int16_t)box__.x_max;
	if(Y1_Tmp < (int16_t)box__.y_min)
		Y1_Tmp = (int16_t)box__.y_min;
	if(Y1_Tmp >= (int16_t)box__.y_max)
		Y1_Tmp = (int16_t)box__.y_max;
	if(Y2_Tmp < (int16_t)box__.y_min)
		Y2_Tmp = (int16_t)box__.y_min;
	if(Y2_Tmp >= (int16_t)box__.y_max)
		Y2_Tmp = (int16_t)box__.y_max;
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	for(;Y1_Tmp < Y2_Tmp; Y1_Tmp++) {
		int16_t _X_ = x - Half_width1;
		for(; _X_ < x + Half_width2; _X_++)
			ssd1306_draw_pixel(inst, &box__, buf, (int16_t)(_X_), (int16_t)(Y1_Tmp), state);
	}
#endif
}

void ssd1306_clear(spi_t *inst, uint8_t *buf, bool state) {
	//uint8_t *_buf = buf;
#ifndef SSD1306_USE_NO_BUF
	/*for(uint16_t cnt = 0; cnt < 1024; cnt++) {
		*_buf++ = state ? 0xFF : 0x00;
	}*/
	memset(buf, state ? 0xFF : 0x00, 1024);
#else
	ssd1306_draw_rectangle(inst, NULL, buf, 0, 0, 128, 64, 1, state);
#endif
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

int ssd1306_draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap, bool foreColor, bool inkColor)
{
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 128;
		box__.y_min = 0;
		box__.y_max = 64;
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
#ifndef SSD1306_USE_NO_BUF
						for (YY = 0; YY < chHeight; YY++) {
							if (Temp & 0x1)
							{
								ssd1306_draw_pixel(inst, &box__, buf, 
									XX + Cursor_X, YY + Cursor_Y, inkColor);
							}
							else
							{
								if (ulOpaque)
									ssd1306_draw_pixel(inst, &box__, buf,
										XX + Cursor_X, YY + Cursor_Y, foreColor);
							}
							Temp = Temp >> 1;
						}
#else
						if(XX + Cursor_X < box__.x_max) {
							SPI_SSD1306_CS_ASSERT();
							ssd1306_wr_cmd(inst, 0x21);
							ssd1306_wr_cmd(inst, XX + Cursor_X);
							ssd1306_wr_cmd(inst, XX + Cursor_X);
							ssd1306_wr_cmd(inst, 0x22);
							ssd1306_wr_cmd(inst, Cursor_Y >> 3);
							ssd1306_wr_cmd(inst, Cursor_Y >> 3);
							SSD1306_DC_PORT |= SSD1306_DC_PIN;
							spi_wrd_byte(inst, inkColor ? Temp : ~Temp);
							SPI_SSD1306_CS_DEASSERT();
						}
#endif
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
