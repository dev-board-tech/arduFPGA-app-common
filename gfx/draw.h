/*
 * Extended draw library file for arduFPGA design.
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


#ifndef DRAW_H_
#define DRAW_H_

#include "def.h"

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
	}
	else {
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
#ifdef Use_FastDrawBar
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
#ifdef Use_FastDrawBar
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



#endif /* DRAW_H_ */