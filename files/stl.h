/*
 * STL file decoder library file for arduFPGA design.
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


#ifndef STL_H_
#define STL_H_

#include "gfx/3d.h"

typedef struct
{
	unsigned char Header[80];
	unsigned long TriangleNr;
}stl_header;

typedef struct
{
	struct
	{
		float x;
		float y;
		float z;
	}NormalVector;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex1;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex2;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex3;
	unsigned short Attribute;
}stl_solid_t;

void stl_get_triangle(_3d_points *triangle_bounds, unsigned char *file, unsigned long triangle_nr);
unsigned long stl_get_nr_of_triangles(unsigned char *file);

#ifdef USE_VISUAL_STUDIO
#include "stl.cpp"
#endif

#endif /* STL_H_ */
