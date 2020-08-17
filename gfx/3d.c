/*
 * 3D library file for arduFPGA design.
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


#include <math.h>
#include "3d.h"

#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const int16_t lib_3ds_sin[] PROGMEM =  
#else
			int16_t lib_3ds_sin[] = 
#endif
	{
	    0,   571,  1143,  1714,  2285,  2855,  3425,  3993, 
	 4560,  5125,  5689,  6252,  6812,  7370,  7927,  8480, 
	 9031,  9580, 10125, 10667, 11206, 11742, 12274, 12803, 
	13327, 13847, 14364, 14875, 15383, 15885, 16383, 16876, 
	17363, 17846, 18323, 18794, 19259, 19719, 20173, 20620, 
	21062, 21497, 21925, 22347, 22761, 23169, 23570, 23964, 
	24350, 24729, 25100, 25464, 25820, 26168, 26509, 26841, 
	27165, 27480, 27787, 28086, 28377, 28658, 28931, 29195, 
	29450, 29696, 29934, 30162, 30381, 30590, 30790, 30981, 
	31163, 31335, 31497, 31650, 31793, 31927, 32050, 32164, 
	32269, 32363, 32448, 32522, 32587, 32642, 32687, 32722, 
	32747, 32762, 32767
	};
#if __AVR_MEGA__
#include <avr/pgmspace.h>
static const int16_t lib_3ds_cos[] PROGMEM = 
#else
			int16_t lib_3ds_cos[] = 
#endif
	{
	32767,32762,32747,32722,32687,32642,32587,32522,
	32448,32363,32269,32164,32050,31927,31793,31650,
	31497,31335,31163,30981,30790,30590,30381,30162,
	29934,29696,29450,29195,28931,28658,28377,28086,
	27787,27480,27165,26841,26509,26168,25820,25464,
	25100,24729,24350,23964,23570,23169,22761,22347,
	21925,21497,21062,20620,20173,19719,19259,18794,
	18323,17846,17363,16876,16383,15885,15383,14875,
	14364,13847,13327,12803,12274,11742,11206,10667,
	10125, 9580, 9031, 8480, 7927, 7370, 6812, 6252,
	 5689, 5125, 4560, 3993, 3425, 2855, 2285, 1714,
	 1143,  571,    0
	 };

int16_t get_sin(uint16_t deg)
{
	uint16_t degIn = (uint16_t)(deg % 359);
	if (degIn <= 90)
		return (int16_t)pgm_read_word(&lib_3ds_sin[degIn]);
	else if (degIn <= 180)
		return (int16_t)pgm_read_word(&lib_3ds_sin[180 - degIn]);
	else if (degIn <= 270)
		return -(int16_t)pgm_read_word(&lib_3ds_sin[degIn - 180]);
	else
		return -(int16_t)pgm_read_word(&lib_3ds_sin[360 - degIn]);
}

int16_t get_cos(uint16_t deg)
{
	uint16_t degIn = (uint16_t)(deg % 359);
	if (degIn <= 90)
		return (int16_t)pgm_read_word(&lib_3ds_cos[degIn]);
	else if (degIn <= 180)
		return -(int16_t)pgm_read_word(&lib_3ds_cos[180 - degIn]);
	else if (degIn <= 270)
		return -(int16_t)pgm_read_word(&lib_3ds_cos[degIn - 180]);
	else
		return (int16_t)pgm_read_word(&lib_3ds_cos[360 - degIn]);
}

//#######################################################################################
/*void make3DPoint(_3d_points *_point, uint32_t Cell, double x, double y, double z)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->z[Cell] = z;
}*/
 //#######################################################################################
/*void make2DPoint(_3d_points *_point, uint32_t Cell, double x, double y, double depth, double scaleFactor)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->depth[Cell] = depth;
	_point->scaleFactor[Cell] = scaleFactor;
}*/
//#######################################################################################
void lib_3ds_3DPointsTo2DPoints(_3d_points *screenPoints, _3d_points *Points, axisRotations AxisRotations)
{
	double sx = sin(AxisRotations.x);
	double cx = cos(AxisRotations.x);
	double sy = sin(AxisRotations.y);
	double cy = cos(AxisRotations.y);
	double sz = sin(AxisRotations.z);
	double cz = cos(AxisRotations.z);
	volatile double x,y,z, xy,xz, yx,yz, zx,zy, scaleFactor;

	uint32_t i = Points->length;
	while (i--){
		x = Points->x[i];
		y = Points->y[i];
		z = Points->z[i];

		// rotation around x
		xy = cx*y - sx*z;
		xz = sx*y + cx*z;
		// rotation around y
		yz = cy*xz - sy*x;
		yx = sy*xz + cy*x;
		// rotation around z
		zx = cz*yx - sz*xy;
		zy = sz*yx + cz*xy;

		scaleFactor = Points->focalLength /(Points->focalLength + yz);
		x = (zx*scaleFactor) / Points->depthScale;
		y = (zy*scaleFactor) / Points->depthScale;
		z = yz / Points->depthScale;

		//make2DPoint(screenPoints, i, x, y, -z, scaleFactor);
		screenPoints->x[i] = x;
		screenPoints->y[i] = y;
		screenPoints->depth[i] = -z;
		screenPoints->scaleFactor[i] = scaleFactor;
	}
	screenPoints->length = Points->length;
}
//#######################################################################################
void lib_3ds_3DPointsTo2DPoints_i(_2d_point_i *screenPoints, _3d_points_i *Points, axisRotations_i *AxisRotations)
{
#if __AVR_MEGA__
	int16_t sx = get_sin(AxisRotations->x);
	int16_t cx = get_cos(AxisRotations->x);
	int16_t sy = get_sin(AxisRotations->y);
	int16_t cy = get_cos(AxisRotations->y);
	int16_t sz = get_sin(AxisRotations->z);
	int16_t cz = get_cos(AxisRotations->z);
#else
	int16_t sx = lib_3ds_sin[AxisRotations->x];
	int16_t cx = lib_3ds_cos[AxisRotations->x];
	int16_t sy = lib_3ds_sin[AxisRotations->y];
	int16_t cy = lib_3ds_cos[AxisRotations->y];
	int16_t sz = lib_3ds_sin[AxisRotations->z];
	int16_t cz = lib_3ds_cos[AxisRotations->z];
#endif
	int32_t x,y,z, xy,xz, yx,yz, zx,zy, scaleFactor;

	uint32_t i = Points->length;
	while (i--){
        x = Points->x[i];
        y = Points->y[i];
        z = Points->z[i];

        // rotation around x
        xz = (int32_t)((int32_t)(sx * y) + (int32_t)(cx * z)) / 65536;
        xy = (int32_t)((int32_t)(cx * y) - (int32_t)(sx * z)) / 65536;
        // rotation around y
        yx = (int32_t)((int32_t)(sy * xz) + (int32_t)(cy * x)) / 65536;
        yz = (int32_t)((int32_t)(cy * xz) - (int32_t)(sy * x)) / 65536;
        // rotation around z
        zy = (int32_t)((int32_t)(sz * yx) + (int32_t)(cz * xy)) / 65536;
        zx = (int32_t)((int32_t)(cz * yx) - (int32_t)(sz * xy)) / 65536;

        scaleFactor = (int32_t)((Points->focalLength * 65536) / ((Points->focalLength + yz)));
        x = (int32_t)((int32_t)(zx * scaleFactor) / (Points->depthScale * 65536));
        y = (int32_t)((int32_t)(zy * scaleFactor) / (Points->depthScale * 65536));
        z = (int32_t)((yz * 65536) / (Points->depthScale * 65536));

		//make2DPoint(screenPoints, i, x, y, -z, scaleFactor);
		screenPoints[i].x = (int16_t)x;
		screenPoints[i].y = (int16_t)y;
		screenPoints[i].depth = (int16_t)-z;
		screenPoints[i].scaleFactor = scaleFactor;
	}
}
//#######################################################################################
//#######################################################################################
//#######################################################################################
void lib_3ds_draw_triangle_i(spi_t *inst, box_t *box, uint8_t *buf, _3d_points_i *Points, int16_t X_offset, int16_t Y_offset, uint16_t X_Angle, uint16_t Y_Angle, uint16_t Z_Angle, uint32_t color)
{
	_2d_point_i screenPoints[3];

	axisRotations_i cubeAxisRotations;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 3;
	lib_3ds_3DPointsTo2DPoints_i(screenPoints, Points, &cubeAxisRotations);

	int16_t X_start = (int16_t)screenPoints[0].x;
	int16_t Y_start = (int16_t)screenPoints[0].y;
	int16_t X_end = (int16_t)screenPoints[1].x;
	int16_t Y_end = (int16_t)screenPoints[1].y;
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int16_t)screenPoints[1].x;
	Y_start = (int16_t)screenPoints[1].y;
	X_end = (int16_t)screenPoints[2].x;
	Y_end = (int16_t)screenPoints[2].y;
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int16_t)screenPoints[2].x;
	Y_start = (int16_t)screenPoints[2].y;
	X_end = (int16_t)screenPoints[0].x;
	Y_end = (int16_t)screenPoints[0].y;
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void lib_3ds_draw_cube_i(spi_t *inst, box_t *box, uint8_t *buf, _3d_points_i *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t Color)
{
	_2d_point_i screenPoints[8];

	axisRotations_i cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	lib_3ds_3DPointsTo2DPoints_i(screenPoints, Points, &cubeAxisRotations);
	draw_line(inst, box, buf, (int16_t)screenPoints[0].x + X_offset, (int16_t)screenPoints[0].y + Y_offset, (int16_t)screenPoints[1].x + X_offset, (int16_t)screenPoints[1].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[0].x + X_offset, (int16_t)screenPoints[0].y + Y_offset, (int16_t)screenPoints[3].x + X_offset, (int16_t)screenPoints[3].y+ Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[0].x + X_offset, (int16_t)screenPoints[0].y + Y_offset, (int16_t)screenPoints[4].x + X_offset, (int16_t)screenPoints[4].y + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int16_t)screenPoints[2].x + X_offset, (int16_t)screenPoints[2].y + Y_offset, (int16_t)screenPoints[1].x + X_offset, (int16_t)screenPoints[1].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[2].x + X_offset, (int16_t)screenPoints[2].y + Y_offset, (int16_t)screenPoints[3].x + X_offset, (int16_t)screenPoints[3].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[2].x + X_offset, (int16_t)screenPoints[2].y + Y_offset, (int16_t)screenPoints[6].x + X_offset, (int16_t)screenPoints[6].y + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int16_t)screenPoints[5].x + X_offset, (int16_t)screenPoints[5].y + Y_offset, (int16_t)screenPoints[1].x + X_offset, (int16_t)screenPoints[1].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[5].x + X_offset, (int16_t)screenPoints[5].y + Y_offset, (int16_t)screenPoints[4].x + X_offset, (int16_t)screenPoints[4].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[5].x + X_offset, (int16_t)screenPoints[5].y + Y_offset, (int16_t)screenPoints[6].x + X_offset, (int16_t)screenPoints[6].y + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int16_t)screenPoints[7].x + X_offset, (int16_t)screenPoints[7].y + Y_offset, (int16_t)screenPoints[3].x + X_offset, (int16_t)screenPoints[3].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[7].x + X_offset, (int16_t)screenPoints[7].y + Y_offset, (int16_t)screenPoints[4].x + X_offset, (int16_t)screenPoints[4].y + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int16_t)screenPoints[7].x + X_offset, (int16_t)screenPoints[7].y + Y_offset, (int16_t)screenPoints[6].x + X_offset, (int16_t)screenPoints[6].y + Y_offset, 1, Color);
}
//#######################################################################################
//#######################################################################################
//#######################################################################################
void lib_3ds_draw_triangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t color)
{
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	lib_3ds_3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	int32_t X_start = (int32_t)screenPoints.x[1];
	int32_t Y_start = (int32_t)screenPoints.y[1];
	int32_t X_end = (int32_t)screenPoints.x[2];
	int32_t Y_end = (int32_t)screenPoints.y[2];
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int32_t)screenPoints.x[2];
	Y_start = (int32_t)screenPoints.y[2];
	X_end = (int32_t)screenPoints.x[3];
	Y_end = (int32_t)screenPoints.y[3];
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int32_t)screenPoints.x[3];
	Y_start = (int32_t)screenPoints.y[3];
	X_end = (int32_t)screenPoints.x[1];
	Y_end = (int32_t)screenPoints.y[1];
	draw_line(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void lib_3ds_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t Color)
{
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	lib_3ds_3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);

	draw_line(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);
	draw_line(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);
}
//#######################################################################################



