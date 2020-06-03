/*
 * SSD1331 OLED display driverfile for arduFPGA designs
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
#include "ssd1331.h"
#include "spi.h"

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

void ssd1331_init(spi_t *inst) {
	SSD1331_DC_DIR |= SSD1331_DC_PIN;
	SSD1331_RST_DIR |= SSD1331_RST_PIN;
	SSD1331_VCCEN_DIR |= SSD1331_VCCEN_PIN;
	SSD1331_PMODEN_DIR |= SSD1331_PMODEN_PIN;
	delay_ms(5);
	SSD1331_RST_PORT &= ~SSD1331_RST_PIN;
	delay_ms(2);
	SSD1331_DC_PORT &= ~SSD1331_DC_PIN;
	SSD1331_RST_PORT |= SSD1331_RST_PIN;
	SSD1331_VCCEN_PORT &= ~SSD1331_VCCEN_PIN;
	SSD1331_PMODEN_PORT |= SSD1331_PMODEN_PIN;
	delay_ms(20);
	SSD1331_RST_PORT &= ~SSD1331_RST_PIN;
	delay_ms(2);
	SSD1331_RST_PORT |= SSD1331_RST_PIN;
	delay_ms(2);
	
	unsigned char cmds[5];
	/* The (un)lock commands register and the byte value to set the register to unlock command mode */
	cmds[0] = 0xFD;
	cmds[1] = 0x12;
	SPI_SSD1331_CS_ASSERT();
	spi_wr_buf(inst, cmds, 2);
	/* Turn off the display with a one byte command */
	cmds[0] = SSD1331_CMD_DISPLAYOFF;
	spi_wr_buf(inst, cmds, 1);
	/* The driver remap and color depth command and the single byte value */
	cmds[0] = SSD1331_CMD_SETREMAP;
	cmds[1] = 0x72;//RGB = 0x72, BGR = 0x76
	spi_wr_buf(inst, cmds, 2);
	/* The set display start line command and the single byte value for the top line */
	cmds[0] = SSD1331_CMD_SETDISPLAYSTARTLINE;
	cmds[1] = 0x00;
	spi_wr_buf(inst, cmds, 2);
	/* The  command and the single byte value for no vertical offset */
	cmds[0] = SSD1331_CMD_SETDISPLAYOFFSET;
	cmds[1] = 0x00;
	spi_wr_buf(inst, cmds, 2);
	/* A single byte value for a normal display */
	cmds[0] = SSD1331_CMD_NORMALDISPLAY;
	spi_wr_buf(inst, cmds, 1);
	/* The multiplex ratio command and the single byte value */
	cmds[0] = SSD1331_CMD_SETMULTIPLEXRATIO;
	cmds[1] = 0x3F;
	spi_wr_buf(inst, cmds, 2);
	/* The master configuration command and the required single byte value of 0x8E */
	cmds[0] = SSD1331_CMD_SETMASTERCONFIGURE;
	cmds[1] = 0x8E;
	spi_wr_buf(inst, cmds, 2);
	/* The power saving mode command and the single byte value */
	cmds[0] = SSD1331_CMD_POWERSAVEMODE;
	cmds[1] = 0x0B;
	spi_wr_buf(inst, cmds, 2);
	/* The set phase length command and the single byte value */
	cmds[0] = SSD1331_CMD_PHASEPERIODADJUSTMENT;
	cmds[1] = 0x31;
	spi_wr_buf(inst, cmds, 2);
	/* The Clock ratio and oscillator frequency command and the single byte value */
	cmds[0] = SSD1331_CMD_DISPLAYCLOCKDIV;
	cmds[1] = 0xF0;
	spi_wr_buf(inst, cmds, 2);
	/* The color A 2nd precharge speed command and the single byte value*/
	cmds[0] = SSD1331_CMD_SETPRECHARGESPEEDA;
	cmds[1] = 0x64;
	spi_wr_buf(inst, cmds, 2);
	/* The color B 2nd precharge speed command and the single byte value*/
	cmds[0] = SSD1331_CMD_SETPRECHARGESPEEDB;
	cmds[1] = 0x78;
	spi_wr_buf(inst, cmds, 2);
	/* The color C 2nd precharge speed command and the single byte value */
	cmds[0] = SSD1331_CMD_SETPRECHARGESPEEDC;
	cmds[1] = 0x64;
	spi_wr_buf(inst, cmds, 2);
	/* The set precharge voltage command and the single byte value */
	cmds[0] = SSD1331_CMD_SETPRECHARGEVOLTAGE;
	cmds[1] = 0x3A;
	spi_wr_buf(inst, cmds, 2);
	/* The VCOMH Deselect level command and the single byte value */
	cmds[0] = SSD1331_CMD_SETVVOLTAGE;
	cmds[1] = 0x3E;
	spi_wr_buf(inst, cmds, 2);
	/* The set master current attenuation factor command and the single byte value */
	cmds[0] = SSD1331_CMD_MASTERCURRENTCONTROL;
	cmds[1] = 0x06;
	spi_wr_buf(inst, cmds, 2);
	/* The Color A contrast command and the single byte value */
	cmds[0] = SSD1331_CMD_SETCONTRASTA;
	cmds[1] = 0x91;
	spi_wr_buf(inst, cmds, 2);
	/* The Color B contrast command and the single byte value */
	cmds[0] = SSD1331_CMD_SETCONTRASTB;
	cmds[1] = 0x50;
	spi_wr_buf(inst, cmds, 2);
	/* The Color C contrast command and the single byte value */
	cmds[0] = SSD1331_CMD_SETCONTRASTC;
	cmds[1] = 0x7D;
	spi_wr_buf(inst, cmds, 2);
	/* The disable scrolling command */
	cmds[0] = SSD1331_CMD_CLEARWINDOW;
	spi_wr_buf(inst, cmds, 1);
	delay_ms(2);
	cmds[0] = SSD1331_CMD_CLEARWINDOW;
	cmds[1] = 0x00;
	cmds[2] = 0x00;
	cmds[3] = 95;
	cmds[4] = 63;
	spi_wr_buf(inst, cmds, 5);
	SSD1331_VCCEN_PORT |= SSD1331_VCCEN_PIN;
	delay_ms(100);
	/* Turn on the display with a one byte command */
	cmds[0] = SSD1331_CMD_DISPLAYON;
	spi_wr_buf(inst, cmds, 1);
	delay_ms(2);
	/* The clear command and the five bytes representing the area to clear */
	cmds[0] = SSD1331_CMD_CLEARWINDOW; 		// Enter the “clear mode”
	cmds[1] = 0x00;					// Set the starting column coordinates
	cmds[2] = 0x00;					// Set the starting row coordinates
	cmds[3] = 95;	// Set the finishing column coordinates;
	cmds[4] = 63;	// Set the finishing row coordinates;
	spi_wr_buf(inst, cmds, 5);
	SPI_SSD1331_CS_DEASSERT();
	delay_ms(5);
	ssd1331_set_contrast(inst, 32);
	ssd1331_clear(inst, false);
}

void ssd1331_wr_cmd(spi_t *inst, uint8_t cmd) {
	SSD1331_DC_PORT &= ~SSD1331_DC_PIN;
	SPI_SSD1331_CS_ASSERT();
	spi_wrd_byte(inst, cmd);
	SPI_SSD1331_CS_DEASSERT();
}

void ssd1331_wr_data(spi_t *inst, uint8_t cmd) {
	SSD1331_DC_PORT |= SSD1331_DC_PIN;
	SPI_SSD1331_CS_ASSERT();
	spi_wrd_byte(inst, cmd);
	SPI_SSD1331_CS_DEASSERT();
}

void ssd1331_on(spi_t *inst, bool state) {
	SPI_SSD1331_CS_ASSERT();
	ssd1331_wr_cmd(inst, state ? 0xAF : 0xAE);
	SPI_SSD1331_CS_DEASSERT();
}

void ssd1331_set_contrast(spi_t *inst, uint8_t cont) {
	SPI_SSD1331_CS_ASSERT();
	ssd1331_wr_cmd(inst, SSD1331_CMD_SETCONTRASTA);
	ssd1331_wr_cmd(inst, cont);
	SPI_SSD1331_CS_DEASSERT();
	SPI_SSD1331_CS_ASSERT();
	ssd1331_wr_cmd(inst, SSD1331_CMD_SETCONTRASTB);
	ssd1331_wr_cmd(inst, cont);
	SPI_SSD1331_CS_DEASSERT();
	SPI_SSD1331_CS_ASSERT();
	ssd1331_wr_cmd(inst, SSD1331_CMD_SETCONTRASTC);
	ssd1331_wr_cmd(inst, cont);
	SPI_SSD1331_CS_DEASSERT();
	delay_ms(5);
}

uint16_t ssd1331_get_x(void) {
	return 96;
}

uint16_t ssd1331_get_y(void) {
	return 64;
}

void ssd1331_draw_pixel(spi_t *inst, box_t *box, int16_t x, int16_t y, bool state) {
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
	uint8_t cmds[8];
	cmds[0] = SSD1331_CMD_DRAWLINE;	//draw rectangle
	cmds[1] = x;					// start column
	cmds[2] = y;					// start row
	cmds[3] = x;					// end column
	cmds[4] = y;					//end row

	cmds[5] = state ? 255 : 0;	//R
	cmds[6] = state ? 255 : 0;	//G
	cmds[7] = state ? 255 : 0;	//B
	SPI_SSD1331_CS_ASSERT();
	spi_wr_buf(inst, cmds, 8);
	SPI_SSD1331_CS_DEASSERT();
}

void ssd1331_draw_rectangle(spi_t *inst, box_t *box, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill, bool state) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 96;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t x_end = x + x_size - 1 ,y_end = y + y_size - 1;
	if(x >= box__.x_max &&
		y >= box__.y_max &&
			x_end < box__.x_min &&
				y_end < box__.y_min)
		return;

	uint8_t cmds[13];
	cmds[0] = SSD1331_CMD_FILLWINDOW;	//draw rectangle
	if(fill)
		cmds[1] = SSD1331_ENABLE_FILL;	//draw rectangle
	else
		cmds[1] = SSD1331_DISABLE_FILL;	//draw rectangle
	SPI_SSD1331_CS_ASSERT();
	spi_wr_buf(inst, cmds, 2);
	SPI_SSD1331_CS_DEASSERT();

	cmds[0] = SSD1331_CMD_DRAWRECTANGLE;	//draw rectangle
	cmds[1] = x;					// start column
	cmds[2] = y;					// start row
	cmds[3] = x_end;					// end column
	cmds[4] = y_end;					//end row

	cmds[5] = state ? 255 : 0;	//R
	cmds[6] = state ? 255 : 0;	//G
	cmds[7] = state ? 255 : 0;	//B
	cmds[8] = state ? 255 : 0;	//R
	cmds[9] = state ? 255 : 0;	//G
	cmds[10] = state ? 255 : 0;	//B
	SPI_SSD1331_CS_ASSERT();
	spi_wr_buf(inst, cmds, 11);
	SPI_SSD1331_CS_DEASSERT();
	delay_ms(3);
}

void ssd1331_draw_h_line(spi_t *inst, box_t *box, int16_t x1, int16_t x2, int16_t y, uint8_t width, bool state) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 96;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t X1_Tmp = x1, X2_Tmp = x1 + x2 - 1;
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
			ssd1331_draw_pixel(inst, &box__, (int16_t)(X1_Tmp), (int16_t)(_Y_), state);
	}
}

void ssd1331_draw_v_line(spi_t *inst, box_t *box, int16_t y1, int16_t y2, int16_t x, uint8_t width, bool state) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 96;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	int16_t Y1_Tmp = y1, Y2_Tmp = y1 + y2 - 1;
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
			ssd1331_draw_pixel(inst, &box__, (int16_t)(_X_), (int16_t)(Y1_Tmp), state);
	}
}

void ssd1331_clear(spi_t *inst, bool state) {
	
	ssd1331_draw_rectangle(inst, NULL, 0, 0, 96, 64, true, state);
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

int ssd1331_draw_string(spi_t *inst, box_t *box, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap, bool foreColor, bool inkColor)
{
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = 96;
		box__.y_min = 0;
		box__.y_max = 64;
	}
	char *pcString = string;
	bool WordWrap = wordWrap;
	//s32 _SelStart = properties->_SelStart;
	//s32 _SelLen = properties->_SelLen;

	uint8_t chWidth = 0;
	uint8_t chHeight = 0;
	int16_t CharPtr;
	uint16_t Tmp = 0;
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
	do
	{
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
		}
		else
		{
			uint8_t Temp;
			/* if CompactWriting is true search the character for free cols from right to left and clear them */
			if (!terminalMode)
			{
				for (Tmp = 1; Tmp < chWidth; Tmp++)
				{
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
			else
			{
				Tmp = chWidth;
			}
			if ((Cursor_X + Tmp >= box__.x_min)
				&& (Cursor_X < box__.x_max + Tmp)
					&& (Cursor_Y + chHeight >= box__.y_min)
						&& (Cursor_Y < box__.y_max + chHeight))
			{
				if (ulVisible)
				{
					uint8_t XX = 0;
					uint8_t YY = 0;
					for (XX = 0; XX < Tmp; XX++)
					{
#ifdef __AVR_MEGA__
						Temp = pgm_read_byte(&CharTable6x8[XX + CharPtr]);
#else
						Temp = CharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++)
						{
							if (Temp & 0x1)
							{
								ssd1331_draw_pixel(inst, &box__, 
									XX + Cursor_X, YY + Cursor_Y, inkColor);
							}
							else
							{
								if (ulOpaque)
									ssd1331_draw_pixel(inst, &box__,
										XX + Cursor_X, YY + Cursor_Y, foreColor);
							}
							Temp = Temp >> 1;
						}
					}
				}
			}
		}
		switch (Char)
		{
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
					&& WordWrap == true)
			{
				Cursor_Y += chHeight;
				Cursor_X = x;
			}
			pcString++;
		}
		CharCnt++;
	} while (1);
}
//#######################################################################################
