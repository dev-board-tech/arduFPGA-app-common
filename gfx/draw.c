/*
 * draw library file for arduFPGA design.
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

#include <string.h>
#include <stdlib.h>
#include "draw.h"
#include "util.h"
#include DISPLAY_DRIVER_FILE

#define FIRST_CHAR_IDX      32

/*#####################################################*/
#ifdef __AVR_MEGA__
#include <avr/pgmspace.h>
static const uint8_t drawCharTable6x8[] PROGMEM =
#else
static const uint8_t drawCharTable6x8[] =
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

int draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap, uint16_t foreColor, uint16_t inkColor) {
	box_t box__;
	if(box) {
		box__.x_min = box->x_min;
		box__.x_max = box->x_max;
		box__.y_min = box->y_min;
		box__.y_max = box->y_max;
	} else {
		box__.x_min = 0;
		box__.x_max = DISPLAY_FUNC_GET_X();
		box__.y_min = 0;
		box__.y_max = DISPLAY_FUNC_GET_Y();
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
	chWidth = pgm_read_byte(&drawCharTable6x8[2]);
	chHeight = pgm_read_byte(&drawCharTable6x8[3]);
#else
	chWidth = drawCharTable6x8[2];
	chHeight = drawCharTable6x8[3];
#endif
	do {
		int8_t Char = *pcString;
		if (Char == 0) {
			return CharCnt - 1;
		}
#ifdef __AVR_MEGA__
		CharPtr = ((Char - pgm_read_byte(&drawCharTable6x8[4])) * chWidth) + pgm_read_byte(&drawCharTable6x8[0]);
		if(Char < pgm_read_byte(&drawCharTable6x8[4]) || Char > pgm_read_byte(&drawCharTable6x8[5]))
#else
		CharPtr = ((Char - drawCharTable6x8[4]) * chWidth) + drawCharTable6x8[0];
		if (Char < drawCharTable6x8[4] || Char > drawCharTable6x8[5])
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
					Temp = pgm_read_byte(&drawCharTable6x8[Tmp + CharPtr]);
#else
					Temp = drawCharTable6x8[Tmp + CharPtr];
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
						Temp = pgm_read_byte(&drawCharTable6x8[XX + CharPtr]);
#else
						Temp = drawCharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++) {
							if (Temp & 0x1) {
								DISPLAY_FUNC_DRAW_PIXEL(inst, &box__, buf,
								XX + Cursor_X, YY + Cursor_Y, inkColor);
							}
							else {
								if (ulOpaque)
								DISPLAY_FUNC_DRAW_PIXEL(inst, &box__, buf,
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
void draw_circle(spi_t *inst, box_t *box, uint8_t *buf, signed int x, signed int y, unsigned int _radius, bool fill, uint32_t color) {
	unsigned int radius = _radius;
	if (radius < 0)
		radius = ~radius;
	signed int a, b, P;

	a = 0;
	b = radius;
	P = 1 - radius;
	signed int Tmp1;
	signed int Tmp2;
	signed int Tmp3;
	signed int Tmp4;
	signed int Tmp5;
	signed int Tmp6;
	signed int Tmp7;
	signed int Tmp8;
	signed int _Tmp5 = 5, _Tmp7 = 0;

	do {
		Tmp1 = x + a;
		Tmp2 = x - a;
		Tmp3 = x + b;
		Tmp4 = x - b;
		Tmp5 = y + a;
		Tmp6 = y - a;
		Tmp7 = y + b;
		Tmp8 = y - b;
		if (fill)
		{

			if (_Tmp7 != Tmp7)
			{
				DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp2, Tmp1 - Tmp2, Tmp7, 1, color);
				DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp2, Tmp1 - Tmp2, Tmp8, 1, color);
			}
			if (_Tmp5 != Tmp5)
			{
				DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp4, Tmp3 - Tmp4, Tmp5, 1, color);
				DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp4, Tmp3 - Tmp4, Tmp6, 1, color);
			}
			_Tmp5 = Tmp5;
			_Tmp7 = Tmp7;
		}
		else {
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp1, Tmp7, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp3, Tmp5, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp2, Tmp7, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp4, Tmp5, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp3, Tmp6, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp1, Tmp8, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp2, Tmp8, color);
			DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, Tmp4, Tmp6, color);
		}

		if (P < 0)
			P += 3 + 2 * a++;
		else
			P += 5 + 2 * (a++ - b--);
	} while (a <= b);
	//refresh();
}
//#######################################################################################
void draw_line(spi_t *inst, box_t *box, uint8_t *buf, signed int X1, signed int Y1, signed int X2, signed int Y2, uint8_t width, uint32_t color) {
	if (width == 1) {
		signed int currentX, currentY, Xinc, Yinc, Dx, Dy, TwoDx, TwoDy,
				twoDxAccumulatedError, twoDyAccumulatedError;
		Dx = (X2 - X1);
		Dy = (Y2 - Y1);
		TwoDx = Dx + Dx;
		TwoDy = Dy + Dy;
		currentX = X1;
		currentY = Y1;
		Xinc = 1;
		Yinc = 1;
		if (Dx < 0) {
			Xinc = -1;
			Dx = -Dx;
			TwoDx = -TwoDx;
		}
		if (Dy < 0) {
			Yinc = -1;
			Dy = -Dy;
			TwoDy = -TwoDy;
		}
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, X1, Y1, color);
		if ((Dx != 0) || (Dy != 0)) {
			if (Dy <= Dx) {
				twoDxAccumulatedError = 0;
				do {
					currentX += Xinc;
					twoDxAccumulatedError += TwoDy;
					if (twoDxAccumulatedError > Dx)
					{
						currentY += Yinc;
						twoDxAccumulatedError -= TwoDx;
					}
					DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, currentX, currentY, color);
				} while (currentX != X2);
			}
			else {
				twoDyAccumulatedError = 0;
				do {
					currentY += Yinc;
					twoDyAccumulatedError += TwoDx;
					if (twoDyAccumulatedError > Dy) {
						currentX += Xinc;
						twoDyAccumulatedError -= TwoDy;
					}
					DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, currentX, currentY, color);
				} while (currentY != Y2);
			}
		}
	} else {
		int32_t half_width;
		int32_t half_width_square;
		signed int dy, dx;
		signed int addx = 1, addy = 1, j;
		signed int P, diff;
		int32_t c1, c2;

		signed int i = 0;
		half_width = width >> 1;
		half_width_square = half_width;
		dx = X2 - X1;
		dy = Y2 - Y1;
		if(dx < 0) {
			dx = -dx;
			half_width_square = half_width * half_width;
		}
		if(dy < 0) {
			dy = -dy;
			half_width_square = half_width * half_width;
		}

		c1 = -(dx * X1 + dy * Y1);
		c2 = -(dx * X2 + dy * Y2);

		if (X1 > X2) {
			int32_t temp;
			temp = c1;
			c1 = c2;
			c2 = temp;
			addx = -1;
		}
		if (Y1 > Y2) {
			int32_t temp;
			temp = c1;
			c1 = c2;
			c2 = temp;
			addy = -1;
		}

		if (dx >= dy) {
			P = 2 * dy - dx;
			diff = P - dx;

			for (i = 0; i <= dx; ++i) {
#ifdef USE_FAST_DRAW_H_LINE
				DISPLAY_FUNC_DRAW_V_LINE(inst, box, buf, Y1+ (-half_width), Y1+ (half_width+width % 2), X1, 1);
#else
				for (j = -half_width; j < half_width + width % 2; ++j) {
					int32_t temp;

					temp = dx * X1 + dy * (Y1 + j); // Use more RAM to increase speed
					if (temp + c1 >= -half_width_square && temp + c2 <= half_width_square)
						DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, X1, Y1 + j, color);
				}
#endif
				if (P < 0) {
					P += 2 * dy;
					X1 += addx;
				}
				else {
					P += diff;
					X1 += addx;
					Y1 += addy;
				}
			}
		}
		else {
			P = 2 * dx - dy;
			diff = P - dy;

			for (i = 0; i <= dy; ++i) {
				if (P < 0) {
					P += 2 * dx;
					Y1 += addy;
				}
				else {
					P += diff;
					X1 += addx;
					Y1 += addy;
				}
#ifdef USE_FAST_DRAW_H_LINE
				DISPLAY_FUNC_DRAW_V_LINE(inst, box, buf, (X1+(-half_width)), (X1+(half_width+width % 2)), Y1, 1);
#else
				for (j = -half_width; j < half_width + width % 2; ++j) {
					signed int temp;

					temp = dx * X1 + dy * (Y1 + j); // Use more RAM to increase speed
					if (temp + c1 >= 0 && temp + c2 <= 0)
						DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, X1 + j, Y1, color);
				}
#endif
			}
		}
	}
}

//#######################################################################################
static void elipseplot(spi_t *inst, box_t *box, uint8_t *buf, signed int xc, signed int yc,
		signed int x, signed int y, bool fill, uint32_t color) {
	signed int Tmp1 = xc + x;
	signed int Tmp2 = xc - x;
	signed int Tmp3 = yc + y;
	signed int Tmp4 = yc - y;

	if (fill) {
		DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp2, Tmp1 - Tmp2, Tmp3, 1, color);
		DISPLAY_FUNC_DRAW_H_LINE(inst, box, buf, Tmp2, Tmp1 - Tmp2, Tmp4, 1, color);
	}
	else {
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, (uint32_t) (Tmp1), (uint32_t) (Tmp3), color);
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, (uint32_t) (Tmp2), (uint32_t) (Tmp3), color);
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, (uint32_t) (Tmp1), (uint32_t) (Tmp4), color);
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, buf, (uint32_t) (Tmp2), (uint32_t) (Tmp4), color);
	}
}
//----------------------------------------------------------------------------------------
void draw_elipse(spi_t *inst, box_t *box, uint8_t *buf, signed int x,signed int y,unsigned int rx,unsigned int ry, bool fill, uint32_t color) {
	unsigned int _rx = rx;
	if (_rx < 0)
		_rx = 0xFFFFFFFF - _rx;
	unsigned int _ry = ry;
	if (_ry < 0)
		_ry = 0xFFFFFFFF - _ry;
	signed int rx2 = _rx * _rx;
	signed int ry2 = _ry * _ry;
	signed int tory2 = 2 * ry2;
	signed int torx2 = 2 * rx2;
	signed int p;
	signed int _x = 0;
	signed int _y = _ry;
	signed int py = torx2 * _y;
	signed int px = 0;
	elipseplot(inst, box, buf, x, y, _x, _y, fill, color);
	p = /*round(*/ry2 - (rx2 * _ry) + (0.25 * rx2)/*)*/;
	while (px < py) {
		_x++;
		px += tory2;
		if (p < 0)
			p += ry2 + px;
		else {
			_y--;
			py -= torx2;
			p += ry2 + px - py;
		}
		elipseplot(inst, box, buf, x, y, _x, _y, fill, color);
	}
	p = /*round(*/ry2 * (_x + 0.5) * (_x + 0.5) + rx2 * (_y - 1) * (_y - 1)
			- rx2 * ry2/*)*/;
	while (_y > 0) {
		_y--;
		py -= torx2;
		if (p > 0)
			p += rx2 - py;
		else {
			_x++;
			px += tory2;
			p += rx2 - py + px;
		}
		elipseplot(inst, box, buf, x, y, _x, _y, fill, color);
	}
}

//#######################################################################################
/*
 *  the coordinates of vertices are (A.x,A.y), (B.x,B.y), (C.x,C.y); we assume that A.y<=B.y<=C.y (you should sort them first)
 * dx1,dx2,dx3 are deltas used in interpolation
 * horizline draws horizontal segment with coordinates (S.x,Y), (E.x,Y)
 * S.x, E.x are left and right x-coordinates of the segment we have to draw
 * S=A means that S.x=A.x; S.y=A.y;
 */

static void triangle_swap_nibble(signed int* a, signed int *b) {
	signed int t = *a;
	*a = *b;
	*b = t;
}

void draw_triangle(spi_t *inst, box_t *box, uint8_t *buf, signed int  ax,signed int  ay,signed int  bx,signed int  by,signed int  cx,signed int  cy, bool fill, uint32_t color)
{
	if(!fill) {
		draw_line(inst, box, buf, ax, ay, bx, by, 1, color);
		draw_line(inst, box, buf, ax, ay, cx, cy, 1, color);
		draw_line(inst, box, buf, bx, by, cx, cy, 1, color);
		return;
	}
	int32_t dx1, dx2, dx3;
	int32_t sx, ex;
	int32_t sy, ey;

	if (ay > by) {
		triangle_swap_nibble(&ay, &by);
		triangle_swap_nibble(&ax, &bx);
	}
	if (ay > cy) {
		triangle_swap_nibble(&ay, &cy);
		triangle_swap_nibble(&ax, &cx);
	}
	if (by > cy) {
		triangle_swap_nibble(&by, &cy);
		triangle_swap_nibble(&bx, &cx);
	}
	if (by - ay > 0)
		dx1 = ((int32_t) (bx - ax) << 16) / (by - ay);
	else
		dx1 = 0;
	if (cy - ay > 0)
		dx2 = ((int32_t) (cx - ax) << 16) / (cy - ay);
	else
		dx2 = 0;
	if (cy - by > 0)
		dx3 = ((int32_t) (cx - bx) << 16) / (cy - by);
	else
		dx3 = 0;

	ex = sx = (int32_t) ax << 16;
	ey = sy = ay;

	if (dx1 > dx2) {
		while (sy <= by) {
			draw_line(inst, box, buf, sx >> 16, sy++, ex >> 16, ey++, 1, color);
			sx += dx2;
			ex += dx1;
		}
		ex = (int32_t) bx << 16;
		ey = by;
		while (sy <= cy) {
			draw_line(inst, box, buf, sx >> 16, sy++, ex >> 16, ey++, 1, color);
			sx += dx2;
			ex += dx3;
		}
	}
	else {
		while (sy <= by) {
			draw_line(inst, box, buf, sx >> 16, sy++, ex >> 16, ey++, 1, color);
			sx += dx1;
			ex += dx2;
		}
		sx = (int32_t) bx << 16;
		sy = by;
		while (sy <= cy) {
			draw_line(inst, box, buf, sx >> 16, sy++, ex >> 16, ey++, 1, color);
			sx += dx3;
			ex += dx2;
		}
	}
}

//#######################################################################################
void draw_msg_and_progress(spi_t *spi_screen, uint8_t *screen_buf, const char *text_P, char *text_R, int32_t progress_min, int32_t progress_max, int32_t progress_value) {
	
	char *buf = NULL;
	if(text_P) {
		buf = (char *)malloc(strlen_P(text_P) + 1);
		if(!buf) {
			return;
		}
		strcpy_P(buf, text_P);
	} else if(text_R) {
		buf = text_R;
	} else {
		return;
	}
	
	int32_t bar_len = util_int_to_percent(DISPLAY_FUNC_GET_X(), progress_min, progress_max, progress_value);
#ifdef DISPLAY_FUNC_CLEAR
	DISPLAY_FUNC_CLEAR(spi_screen, screen_buf, 0);
#endif
	if(bar_len) {
#ifdef DISPLAY_FUNC_DRAW_RECTANGLE
		DISPLAY_FUNC_DRAW_RECTANGLE(spi_screen, NULL, screen_buf, 0, 32, bar_len, 8, true, true);
#endif
	}
#ifdef DISPLAY_FUNC_DRAW_STRING
	DISPLAY_FUNC_DRAW_STRING(spi_screen, NULL, screen_buf, buf, 0, 8, false, false, 0, 1);
#endif
}

//#######################################################################################
void draw_msg(spi_t *spi_screen, uint8_t *screen_buf, const char *text_P, char *text_R, uint8_t x, uint8_t y) {
	char *buf = NULL;
	if(text_P) {
		buf = (char *)malloc(strlen_P(text_P) + 1);
		if(!buf) {
			return;
		}
		strcpy_P(buf, text_P);
	} else if(text_R) {
		buf = text_R;
	} else {
		return;
	}
#ifdef DISPLAY_FUNC_CLEAR
	DISPLAY_FUNC_CLEAR(spi_screen, screen_buf, 0);
#endif
#ifdef DISPLAY_FUNC_DRAW_STRING
	DISPLAY_FUNC_DRAW_STRING(spi_screen, NULL, screen_buf, buf, x, y, false, false, 0, 1);
#endif
}


//#######################################################################################


// Font Definition
#ifdef __AVR_MEGA__
const uint8_t font4x6[97][2] PROGMEM = {
#else
const uint8_t font4x6[97][2] = {
	#endif
	{ 0x00  ,  0x00 },   /*SPACE*/
	{ 0x49  ,  0x08 },   /*'!'*/
	{ 0xb4  ,  0x00 },   /*'"'*/
	{ 0xbe  ,  0xf6 },   /*'#'*/
	{ 0x7b  ,  0x7a },   /*'$'*/
	{ 0xa5  ,  0x94 },   /*'%'*/
	{ 0x55  ,  0xb8 },   /*'&'*/
	{ 0x48  ,  0x00 },   /*'''*/
	{ 0x29  ,  0x44 },   /*'('*/
	{ 0x44  ,  0x2a },   /*')'*/
	{ 0x15  ,  0xa0 },   /*'*'*/
	{ 0x0b  ,  0x42 },   /*'+'*/
	{ 0x00  ,  0x50 },   /*','*/
	{ 0x03  ,  0x02 },   /*'-'*/
	{ 0x00  ,  0x08 },   /*'.'*/
	{ 0x25  ,  0x90 },   /*'/'*/
	{ 0x76  ,  0xba },   /*'0'*/
	{ 0x59  ,  0x5c },   /*'1'*/
	{ 0xc5  ,  0x9e },   /*'2'*/
	{ 0xc5  ,  0x38 },   /*'3'*/
	{ 0x92  ,  0xe6 },   /*'4'*/
	{ 0xf3  ,  0x3a },   /*'5'*/
	{ 0x73  ,  0xba },   /*'6'*/
	{ 0xe5  ,  0x90 },   /*'7'*/
	{ 0x77  ,  0xba },   /*'8'*/
	{ 0x77  ,  0x3a },   /*'9'*/
	{ 0x08  ,  0x40 },   /*':'*/
	{ 0x08  ,  0x50 },   /*';'*/
	{ 0x2a  ,  0x44 },   /*'<'*/
	{ 0x1c  ,  0xe0 },   /*'='*/
	{ 0x88  ,  0x52 },   /*'>'*/
	{ 0xe5  ,  0x08 },   /*'?'*/
	{ 0x56  ,  0x8e },   /*'@'*/
	{ 0x77  ,  0xb6 },   /*'A'*/
	{ 0x77  ,  0xb8 },   /*'B'*/
	{ 0x72  ,  0x8c },   /*'C'*/
	{ 0xd6  ,  0xba },   /*'D'*/
	{ 0x73  ,  0x9e },   /*'E'*/
	{ 0x73  ,  0x92 },   /*'F'*/
	{ 0x72  ,  0xae },   /*'G'*/
	{ 0xb7  ,  0xb6 },   /*'H'*/
	{ 0xe9  ,  0x5c },   /*'I'*/
	{ 0x64  ,  0xaa },   /*'J'*/
	{ 0xb7  ,  0xb4 },   /*'K'*/
	{ 0x92  ,  0x9c },   /*'L'*/
	{ 0xbe  ,  0xb6 },   /*'M'*/
	{ 0xd6  ,  0xb6 },   /*'N'*/
	{ 0x56  ,  0xaa },   /*'O'*/
	{ 0xd7  ,  0x92 },   /*'P'*/
	{ 0x76  ,  0xee },   /*'Q'*/
	{ 0x77  ,  0xb4 },   /*'R'*/
	{ 0x71  ,  0x38 },   /*'S'*/
	{ 0xe9  ,  0x48 },   /*'T'*/
	{ 0xb6  ,  0xae },   /*'U'*/
	{ 0xb6  ,  0xaa },   /*'V'*/
	{ 0xb6  ,  0xf6 },   /*'W'*/
	{ 0xb5  ,  0xb4 },   /*'X'*/
	{ 0xb5  ,  0x48 },   /*'Y'*/
	{ 0xe5  ,  0x9c },   /*'Z'*/
	{ 0x69  ,  0x4c },   /*'['*/
	{ 0x91  ,  0x24 },   /*'\'*/
	{ 0x64  ,  0x2e },   /*']'*/
	{ 0x54  ,  0x00 },   /*'^'*/
	{ 0x00  ,  0x1c },   /*'_'*/
	{ 0x44  ,  0x00 },   /*'`'*/
	{ 0x0e  ,  0xae },   /*'a'*/
	{ 0x9a  ,  0xba },   /*'b'*/
	{ 0x0e  ,  0x8c },   /*'c'*/
	{ 0x2e  ,  0xae },   /*'d'*/
	{ 0x0e  ,  0xce },   /*'e'*/
	{ 0x56  ,  0xd0 },   /*'f'*/
	{ 0x55  ,  0x3B },   /*'g'*/
	{ 0x93  ,  0xb4 },   /*'h'*/
	{ 0x41  ,  0x44 },   /*'i'*/
	{ 0x41  ,  0x51 },   /*'j'*/
	{ 0x97  ,  0xb4 },   /*'k'*/
	{ 0x49  ,  0x44 },   /*'l'*/
	{ 0x17  ,  0xb6 },   /*'m'*/
	{ 0x1a  ,  0xb6 },   /*'n'*/
	{ 0x0a  ,  0xaa },   /*'o'*/
	{ 0xd6  ,  0xd3 },   /*'p'*/
	{ 0x76  ,  0x67 },   /*'q'*/
	{ 0x17  ,  0x90 },   /*'r'*/
	{ 0x0f  ,  0x38 },   /*'s'*/
	{ 0x9a  ,  0x8c },   /*'t'*/
	{ 0x16  ,  0xae },   /*'u'*/
	{ 0x16  ,  0xba },   /*'v'*/
	{ 0x16  ,  0xf6 },   /*'w'*/
	{ 0x15  ,  0xb4 },   /*'x'*/
	{ 0xb5  ,  0x2b },   /*'y'*/
	{ 0x1c  ,  0x5e },   /*'z'*/
	{ 0x6b  ,  0x4c },   /*'{'*/
	{ 0x49  ,  0x48 },   /*'|'*/
	{ 0xc9  ,  0x5a },   /*'}'*/
	{ 0x54  ,  0x00 },   /*'~'*/
	{ 0x56  ,  0xe2 },   /*''*/
	{ 0xFF  ,  0xFE }    /*''*/
};


void drawChar4x6(spi_t *inst, box_t *box, uint8_t *vram, int16_t x, int16_t y,  char c, DISPLAY_COLOR_VAR colour)
{
	const uint8_t index = ((unsigned char)(c)) - FIRST_CHAR_IDX;

	if (index > 97)
	return;

	#ifdef __AVR_MEGA__
	uint8_t data1 = pgm_read_byte(&font4x6[index][0]);
	uint8_t data2 = pgm_read_byte(&font4x6[index][1]);
	#else
	uint8_t data1 = font4x6[index][0];
	uint8_t data2 = font4x6[index][1];
	#endif
	DISPLAY_COLOR_VAR fg = colour;
	#if 0
	DISPLAY_COLOR_VAR bg = !colour;
	if (data2 & 1)	// Descender e.g. j, g
	{
		Platform::DrawPixel(cursorX, y, bg);
		Platform::DrawPixel(cursorX + 1, y, bg);
		Platform::DrawPixel(cursorX + 2, y, bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data1 & 0x80) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x40) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data1 & 0x20) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data1 & 0x10) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x8) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data1 & 0x4) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data1 & 0x2) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x1) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x2) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data2 & 0x80) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data2 & 0x40) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x20) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data2 & 0x10) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data2 & 0x8) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x4) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;
	}
	else
	{
		Platform::DrawPixel(cursorX, y, (data1 & 0x80) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x40) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data1 & 0x20) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data1 & 0x10) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x8) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data1 & 0x4) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data1 & 0x2) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data1 & 0x1) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x2) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data2 & 0x80) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data2 & 0x40) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x20) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, (data2 & 0x10) ? fg : bg);
		Platform::DrawPixel(cursorX + 1, y, (data2 & 0x8) ? fg : bg);
		Platform::DrawPixel(cursorX + 2, y, (data2 & 0x4) ? fg : bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
		y++;

		Platform::DrawPixel(cursorX, y, bg);
		Platform::DrawPixel(cursorX + 1, y, bg);
		Platform::DrawPixel(cursorX + 2, y, bg);
		Platform::DrawPixel(cursorX + 3, y, bg);
	}
	#endif
	if (data2 & 1)	// Descender e.g. j, g
	{
		y++;

		if(data1 & 0x80)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x40)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data1 & 0x20)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data1 & 0x10)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x8)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data1 & 0x4)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data1 & 0x2)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x1)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x2)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data2 & 0x80)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data2 & 0x40)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x20)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data2 & 0x10)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data2 & 0x8)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x4)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;
	}
	else
	{
		if (data1 & 0x80)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x40)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data1 & 0x20)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data1 & 0x10)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x8)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data1 & 0x4)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data1 & 0x2)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data1 & 0x1)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x2)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data2 & 0x80)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data2 & 0x40)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x20)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;

		if (data2 & 0x10)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x, y, fg);
		if (data2 & 0x8)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 1, y, fg);
		if (data2 & 0x4)
		DISPLAY_FUNC_DRAW_PIXEL(inst, box, vram,x + 2, y, fg);
		y++;
	}
}

uint8_t getCharHeight4x6() {
	return 6;
}

uint8_t getCharWidth4x6() {
	return 4;
}

int getRowsInBox4x6(gfxString_t *inst, box_t *box) {
	int cnt = 1;
	int t = box->y_min + 6;
	for(; t < box->y_max; t += 6)
		cnt++;
	return cnt;
}

int getColsInBox4x6(gfxString_t *inst, box_t *box) {
	int cnt = 1;
	int t = box->x_min + 4;
	for(; t < box->x_max; t += 4)
		cnt++;
	return cnt;
}

int drawStringWindowed4x6(gfxString_t *inst, box_t *box, char *string, int16_t x, int16_t y, int16_t cursorPos, bool cursorState) {
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
    int16_t cX = x;
    int16_t cY = y;
    int16_t cCnt = 0;
    inst->edgeTouch = 0;
    uint16_t cLineCnt = 0;;
    inst->rowCnt = 0;
    do {
	    if(cCnt == inst->maxLen && inst->maxLen) {
		    return cCnt;
	    }
	    if(cursorPos == cCnt) {
		    if(cX < box__.x_min) {
			    inst->edgeTouch |= EDGE_OUTSIDE_LEFT;
		    }
		    if(cY <= box__.y_min) {
			    inst->edgeTouch |= EDGE_OUTSIDE_UP;
		    }
		    if(cX >= box__.x_max) {
			    inst->edgeTouch |= EDGE_OUTSIDE_RIGHT;
		    }
		    if(cY >= box__.y_max - 6) {
			    inst->edgeTouch |= EDGE_OUTSIDE_DOWN;
		    }
	    }

	    char C, c = C = *pcString;
	    if(cursorPos == cCnt) {
		    if((cursorState) || (cursorState && C == 0))
		    C = (uint8_t)(96+32);
	    }
	    if(!inst->transparent)
	    drawChar4x6(inst->spi, box, inst->vram, cX, cY, C, inst->inkColor);
	    if (c == 0) {
		    return cCnt - 1;
	    }
	    switch (c) {
		    case '\r':
		    case '\n':
		    if(cY > box__.y_max)
		    return cCnt;
		    cX = x;
		    cY += 6;
		    inst->rowCnt++;
		    break;
		    case 0x09:
		    cX = (4 * inst->tabSpaces) + ((cX / (4 * inst->tabSpaces)) * (4 * inst->tabSpaces));
		    break;
		    default:
		    cX += 4;
		    if ((cX + 4 > box__.x_max)
		    && inst->wordWrap == true) {
			    cY += 6;
			    cX = x;
		    }
		    break;
	    }
	    cLineCnt++;
	    if(cLineCnt > inst->maxLineLen && inst->maxLineLen) {
		    cLineCnt = 0;
		    cX = x;
		    cY += 6;
		    inst->rowCnt++;
	    }
	    pcString++;
	    cCnt++;
    } while (1);
}
