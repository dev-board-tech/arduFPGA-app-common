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


#ifndef _3D_H_
#define _3D_H_

#include <stdlib.h>
#include "def.h"
#include "driver/spi.h"
#include "gfx/draw.h"
#define _depth 1

#define PI (3.14159265358979323846264338327950288)
#define PI2 (3.14159265358979323846264338327950288/2.0)



typedef struct {
	int16_t x;
	int16_t y;
	int16_t depth;
	int16_t scaleFactor;
}_2d_point_i;

typedef struct {
	int32_t *x;
	int32_t *y;
	int32_t *z;
	int32_t *depth;
	int32_t *scaleFactor;
	int32_t focalLength;
	int32_t depthScale;
	uint16_t length;
}_3d_points_i;

typedef struct 
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
}axisRotations_i;

typedef struct
{
	double *x;
	double *y;
	double *z;
	double *depth;
	double *scaleFactor;
	double focalLength;
	double depthScale;
	uint32_t length;
}point;

typedef struct
{
	double x[8];
	double y[8];
	double z[8];
	double depth[8];
	double scaleFactor[8];
	double focalLength;
	double depthScale;
	uint32_t length;
}_3d_points;

typedef struct
{
	double x;
	double y;
	double z;
}axisRotations;

typedef struct
{
	point points[8];
}Transformed_Points_Array;

void lib_3ds_3DPointsTo2DPoints(_3d_points *screenPoints, _3d_points *Points, axisRotations AxisRotations);
void lib_3ds_draw_triangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t Color);
void lib_3ds_draw_triangle_i(spi_t *inst, box_t *box, uint8_t *buf, _3d_points_i *Points, int16_t X_offset, int16_t Y_offset, uint16_t X_Angle, uint16_t Y_Angle, uint16_t Z_Angle, uint32_t color);
void lib_3ds_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t Color);

//#######################################################################################
#ifdef USE_VISUAL_STUDIO
#include "3d.cpp"
#endif
#endif /* 3D_H_ */
