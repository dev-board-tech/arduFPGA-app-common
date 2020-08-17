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

#include "draw.h"
#include DISPLAY_DRIVER_FILE


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

