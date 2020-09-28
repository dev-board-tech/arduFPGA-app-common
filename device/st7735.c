/*
 * ST7735 TFT LCD display driver file for arduFPGA designs.
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
#include "st7735.h"
#include "driver/spi.h"

#define DELAY 0x80

#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const unsigned char ST7735_BIT_MASK_TABLE [] PROGMEM = {
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

// based on Adafruit ST7735 library for Arduino
static const uint8_t
init_cmds1[] 
#if __AVR_MEGA__
 PROGMEM
#endif
= {            // Init for 7735R, part 1 (red or green tab)
	15,                       // 15 commands in list:
	ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
	150,                    //     150 ms delay
	ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
	255,                    //     500 ms delay
	ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
	0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
	0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
	0x01, 0x2C, 0x2D,       //     Dot inversion mode
	0x01, 0x2C, 0x2D,       //     Line inversion mode
	ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
	0x07,                   //     No inversion
	ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
	0xA2,
	0x02,                   //     -4.6V
	0x84,                   //     AUTO mode
	ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
	0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
	ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
	0x0A,                   //     Opamp current small
	0x00,                   //     Boost frequency
	ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
	0x8A,                   //     BCLK/2, Opamp current small & Medium low
	0x2A,
	ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
	0x8A, 0xEE,
	ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
	0x0E,
	ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
	ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
	ST7735_ROTATION,        //     row addr/col addr, bottom to top refresh
	ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
	0x05 },                 //     16-bit color

#ifdef ST7735_128x128
init_cmds2[] 
#if __AVR_MEGA__
 PROGMEM
#endif
= {            // Init for 7735R, part 2 (1.44" display)
	2,                        //  2 commands in list:
	ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
	0x00, 0x00,             //     XSTART = 0
	0x00, 0x7F,             //     XEND = 127
	ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
	0x00, 0x00,             //     XSTART = 0
	0x00, 0x7F },           //     XEND = 127
#endif // ST7735_IS_128X128

#ifndef ST7735_128x128
init_cmds2[] 
#if __AVR_MEGA__
 PROGMEM
#endif
= {            // Init for 7735S, part 2 (160x80 display)
	3,                        //  3 commands in list:
	ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
	0x00, 0x00,             //     XSTART = 0
	0x00, 0x4F,             //     XEND = 79
	ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
	0x00, 0x00,             //     XSTART = 0
	0x00, 0x9F ,            //     XEND = 159
	ST7735_INVON, 0 },        //  3: Invert colors
#endif

init_cmds3[] 
#if __AVR_MEGA__
 PROGMEM
#endif
= {            // Init for 7735R, part 3 (red or green tab)
	4,                        //  4 commands in list:
	ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
	0x02, 0x1c, 0x07, 0x12,
	0x37, 0x32, 0x29, 0x2d,
	0x29, 0x25, 0x2B, 0x39,
	0x00, 0x01, 0x03, 0x10,
	ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
	0x03, 0x1d, 0x07, 0x06,
	0x2E, 0x2C, 0x29, 0x2D,
	0x2E, 0x2E, 0x37, 0x3F,
	0x00, 0x00, 0x02, 0x10,
	ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
	10,                     //     10 ms delay
	ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
	100 };                  //     100 ms delay


static void st7735_select() {
	SPI_ST7735_CS_ASSERT();
}

static void st7735_unselect() {
	SPI_ST7735_CS_DEASSERT();
}

static void st7735_reset() {
	ST7735_RST_PORT &= ~ST7735_RST_PIN;
	delay_ms(5);
	ST7735_RST_PORT |= ST7735_RST_PIN;
}

static void st7735_writeCommand(spi_t *inst, uint8_t cmd) {
	ST7735_DC_PORT &= ~ST7735_DC_PIN;
	spi_wrd_byte(inst, cmd);
}

static void st7735_writeData(spi_t *inst, uint8_t* buff, size_t buff_size) {
	ST7735_DC_PORT |= ST7735_DC_PIN;
	//spi_wrd_buf(inst, buff, buff, buff_size);
	for (int cnt = 0; cnt < buff_size; cnt++) {
		spi_wrd_byte(inst, *buff++);
	}
}

static void st7735_executeCommandList(spi_t *inst, const uint8_t *addr) {
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
		st7735_writeCommand(inst, cmd);

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
			st7735_writeData(inst, tmp_buf, numArgs);
#else
			st7735_writeData(inst, (uint8_t*)addr, numArgs);
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

static void st7735_setAddressWindow(spi_t *inst, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	// column address set
	st7735_writeCommand(inst, ST7735_CASET);
	uint8_t data[] = { 0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART };
	st7735_writeData(inst, data, sizeof(data));

	// row address set
	st7735_writeCommand(inst, ST7735_RASET);
	data[1] = y0 + ST7735_YSTART;
	data[3] = y1 + ST7735_YSTART;
	st7735_writeData(inst, data, sizeof(data));

	// write to RAM
	st7735_writeCommand(inst, ST7735_RAMWR);
}


void st7735_init(spi_t *inst, uint8_t *buf) {
	ST7735_DC_DIR |= ST7735_DC_PIN;
	ST7735_RST_DIR |= ST7735_RST_PIN;
    st7735_reset();
    st7735_select();
    st7735_executeCommandList(inst, init_cmds1);
    st7735_executeCommandList(inst, init_cmds2);
    st7735_executeCommandList(inst, init_cmds3);
    st7735_unselect();
}

uint16_t st7735_get_x() {
#ifdef ST7735_128x128
	return 128;
#else
	return 160;
#endif
}

uint16_t st7735_get_y() {
#ifdef ST7735_128x128
	return 128;
#else
	return 80;
#endif
}

void st7735_rfsh(spi_t *inst, uint8_t *buf) {
}

void st7735_on(spi_t *inst, bool state) {
	st7735_select();
	//st7735_writeCommand(inst, state ? ST7735_NORON : ST7735_NOROFF);
	st7735_writeCommand(inst, state ? ST7735_DISPON : ST7735_DISPOFF);
	st7735_unselect();
}

void st7735_set_contrast(spi_t *inst, uint8_t cont) {
	//spi_wrd_byte(inst, 0x81);
	//spi_wrd_byte(inst, cont);
	//SPI_ST7735_CS_DEASSERT();
}

void st7735_draw_pixel(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y,
#ifdef ST7735_BW_MODE
bool
#else
uint16_t
#endif
 color) {
	/* Check if outside the display */
	if(x < 0 || y < 0 || y > st7735_get_y() - 1)
		return;
	/* Check if outside the window */
	if(box) {
		if(x < box->x_min ||
		x >= box->x_max ||
		y < box->y_min ||
		y >= box->y_max)
			return;
	}
    st7735_select();

    st7735_setAddressWindow(inst, x, y, x+1, y+1);
#ifdef ST7735_BW_MODE
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
    st7735_writeData(inst, data, sizeof(data));
    st7735_unselect();
}

void st7735_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill, 
#ifdef ST7735_BW_MODE
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
		box__.x_max = st7735_get_x();
		box__.y_min = 0;
		box__.y_max = st7735_get_y();
	}
	int16_t x_end = x + x_size ,y_end = y + y_size;
	if(x >= box__.x_max ||
	y >= box__.y_max ||
	x_end < box__.x_min ||
	y_end < box__.y_min)
		return;
	register int16_t LineCnt = y;

#ifdef ST7735_BW_MODE
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
		
		st7735_select();
		st7735_setAddressWindow(inst, x, LineCnt, _x_end, y_end);

		uint16_t pix_nr = (LineCnt - y_end) * (x - _x_end);
		ST7735_DC_PORT |= ST7735_DC_PIN;
		for (;pix_nr > 0; pix_nr--) {
			spi_wrd_byte(inst, data[0]);
			spi_wrd_byte(inst, data[1]);
		}
		st7735_unselect();
		return;
	}
	st7735_draw_h_line(inst, box, buf, x, x + x_size, y, 1, color);
	st7735_draw_h_line(inst, box, buf, x, x + x_size, y + y_size, 1, color);
	st7735_draw_v_line(inst, box, buf, y, y + y_size, x, 1, color);
	st7735_draw_v_line(inst, box, buf, y, y + y_size, x + x_size, 1, color);
}

void st7735_draw_h_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t x1, int16_t x2, int16_t y, uint8_t width,
#ifdef ST7735_BW_MODE
 bool
#else
 uint16_t
#endif
 color ) {
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	st7735_draw_rectangle(inst, box, buf, x1, y - Half_width1, x2, y + Half_width2, true, color);
}

void st7735_draw_v_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t y1, int16_t y2, int16_t x, uint8_t width,
#ifdef ST7735_BW_MODE
 bool
#else
 uint16_t
#endif
 color) {
	int16_t Half_width1 = (width>>1);
	int16_t Half_width2 = width-Half_width1;
	st7735_draw_rectangle(inst, box, buf, x - Half_width1, y1, x + Half_width2, y2, true, color);
}

void st7735_clear(spi_t *inst, uint8_t *buf,
#ifdef ST7735_BW_MODE
 bool
#else
 uint16_t
#endif
 color) {
	st7735_draw_rectangle(inst, NULL, buf, 0, 0, st7735_get_x(), st7735_get_y(), true, color);
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

int st7735_draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap,
#ifdef ST7735_BW_MODE
 bool
#else
 uint16_t
#endif
 foreColor,
 #ifdef ST7735_BW_MODE
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
		box__.x_max = st7735_get_x();
		box__.y_min = 0;
		box__.y_max = st7735_get_y();
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
								st7735_draw_pixel(inst, &box__, buf,
								XX + Cursor_X, YY + Cursor_Y, inkColor);
							}
							else {
								if (ulOpaque)
								st7735_draw_pixel(inst, &box__, buf,
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
