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
//#######################################################################################
void make3DPoint(_3d_points *_point, uint32_t Cell, double x, double y, double z)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->z[Cell] = z;
}
 //#######################################################################################
void make2DPoint(_3d_points *_point, uint32_t Cell, double x, double y, double depth, double scaleFactor)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->depth[Cell] = depth;
	_point->scaleFactor[Cell] = scaleFactor;
}
//#######################################################################################
void Transform3DPointsTo2DPoints(_3d_points *screenPoints, _3d_points *Points, axisRotations AxisRotations)
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

		make2DPoint(screenPoints, i, x, y, -z, scaleFactor);
	}
	screenPoints->length = Points->length;
}
//#######################################################################################
//#######################################################################################
//#######################################################################################
void put_3d_triangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t color)
{
#ifdef DISPLAY_FUNC_DRAW_LINE
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	int32_t X_start = (int32_t)screenPoints.x[1];
	int32_t Y_start = (int32_t)screenPoints.y[1];
	int32_t X_end = (int32_t)screenPoints.x[2];
	int32_t Y_end = (int32_t)screenPoints.y[2];
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int32_t)screenPoints.x[2];
	Y_start = (int32_t)screenPoints.y[2];
	X_end = (int32_t)screenPoints.x[3];
	Y_end = (int32_t)screenPoints.y[3];
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (int32_t)screenPoints.x[3];
	Y_start = (int32_t)screenPoints.y[3];
	X_end = (int32_t)screenPoints.x[1];
	Y_end = (int32_t)screenPoints.y[1];
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
#endif
}
//#######################################################################################
void put_3d_rectangle(spi_t *inst, box_t *box, uint8_t *buf, _3d_points *Points, int32_t X_offset, int32_t Y_offset, double X_Angle, double Y_Angle, double Z_Angle, uint32_t Color)
{
#ifdef DISPLAY_FUNC_DRAW_LINE
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[0] + X_offset, (int32_t)screenPoints.y[0] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);

	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[2] + X_offset, (int32_t)screenPoints.y[2] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);

	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[1] + X_offset, (int32_t)screenPoints.y[1] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[5] + X_offset, (int32_t)screenPoints.y[5] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);

	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[3] + X_offset, (int32_t)screenPoints.y[3] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[4] + X_offset, (int32_t)screenPoints.y[4] + Y_offset, 1, Color);
	DISPLAY_FUNC_DRAW_LINE(inst, box, buf, (int32_t)screenPoints.x[7] + X_offset, (int32_t)screenPoints.y[7] + Y_offset, (int32_t)screenPoints.x[6] + X_offset, (int32_t)screenPoints.y[6] + Y_offset, 1, Color);
#endif
}
//#######################################################################################



