/*
 * Draw library file for arduFPGA design.
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

#include <stdbool.h>
#include "def.h"
#include "driver/spi.h"

#define COLOR_CONVERSION_32_TO_16(COLOR_32) \
	(uint16_t)(((((uint32_t)COLOR_32 >> 19) & 0x1F) << 11) | (((COLOR_32 >> 10) & 0x3F) << 5) | ((COLOR_32 >> 3) & 0x1F))

#define Clr16AliceBlue            COLOR_CONVERSION_32_TO_16(0x00F0F8FF)
#define Clr16AntiqueWhite         COLOR_CONVERSION_32_TO_16(0x00FAEBD7)
#define Clr16Aqua                 COLOR_CONVERSION_32_TO_16(0x0000FFFF)
#define Clr16Aquamarine           COLOR_CONVERSION_32_TO_16(0x007FFFD4)
#define Clr16Azure                COLOR_CONVERSION_32_TO_16(0x00F0FFFF)
#define Clr16Beige                COLOR_CONVERSION_32_TO_16(0x00F5F5DC)
#define Clr16Bisque               COLOR_CONVERSION_32_TO_16(0x00FFE4C4)
#define Clr16Black                COLOR_CONVERSION_32_TO_16(0x00000000)
#define Clr16BlanchedAlmond       COLOR_CONVERSION_32_TO_16(0x00FFEBCD)
#define Clr16Blue                 COLOR_CONVERSION_32_TO_16(0x000000FF)
#define Clr16BlueViolet           COLOR_CONVERSION_32_TO_16(0x008A2BE2)
#define Clr16Brown                COLOR_CONVERSION_32_TO_16(0x00A52A2A)
#define Clr16BurlyWood            COLOR_CONVERSION_32_TO_16(0x00DEB887)
#define Clr16CadetBlue            COLOR_CONVERSION_32_TO_16(0x005F9EA0)
#define Clr16Chartreuse           COLOR_CONVERSION_32_TO_16(0x007FFF00)
#define Clr16Chocolate            COLOR_CONVERSION_32_TO_16(0x00D2691E)
#define Clr16Coral                COLOR_CONVERSION_32_TO_16(0x00FF7F50)
#define Clr16CornflowerBlue       COLOR_CONVERSION_32_TO_16(0x006495ED)
#define Clr16Cornsilk             COLOR_CONVERSION_32_TO_16(0x00FFF8DC)
#define Clr16Crimson              COLOR_CONVERSION_32_TO_16(0x00DC143C)
#define Clr16Cyan                 COLOR_CONVERSION_32_TO_16(0x0000FFFF)
#define Clr16DarkBlue             COLOR_CONVERSION_32_TO_16(0x0000008B)
#define Clr16DarkCyan             COLOR_CONVERSION_32_TO_16(0x00008B8B)
#define Clr16DarkGoldenrod        COLOR_CONVERSION_32_TO_16(0x00B8860B)
#define Clr16DarkGray             COLOR_CONVERSION_32_TO_16(0x00A9A9A9)
#define Clr16DarkGreen            COLOR_CONVERSION_32_TO_16(0x00006400)
#define Clr16DarkKhaki            COLOR_CONVERSION_32_TO_16(0x00BDB76B)
#define Clr16DarkMagenta          COLOR_CONVERSION_32_TO_16(0x008B008B)
#define Clr16DarkOliveGreen       COLOR_CONVERSION_32_TO_16(0x00556B2F)
#define Clr16DarkOrange           COLOR_CONVERSION_32_TO_16(0x00FF8C00)
#define Clr16DarkOrchid           COLOR_CONVERSION_32_TO_16(0x009932CC)
#define Clr16DarkRed              COLOR_CONVERSION_32_TO_16(0x008B0000)
#define Clr16DarkSalmon           COLOR_CONVERSION_32_TO_16(0x00E9967A)
#define Clr16DarkSeaGreen         COLOR_CONVERSION_32_TO_16(0x008FBC8F)
#define Clr16DarkSlateBlue        COLOR_CONVERSION_32_TO_16(0x00483D8B)
#define Clr16DarkSlateGray        COLOR_CONVERSION_32_TO_16(0x002F4F4F)
#define Clr16DarkTurquoise        COLOR_CONVERSION_32_TO_16(0x0000CED1)
#define Clr16DarkViolet           COLOR_CONVERSION_32_TO_16(0x009400D3)
#define Clr16DeepPink             COLOR_CONVERSION_32_TO_16(0x00FF1493)
#define Clr16DeepSkyBlue          COLOR_CONVERSION_32_TO_16(0x0000BFFF)
#define Clr16DimGray              COLOR_CONVERSION_32_TO_16(0x00696969)
#define Clr16DodgerBlue           COLOR_CONVERSION_32_TO_16(0x001E90FF)
#define Clr16FireBrick            COLOR_CONVERSION_32_TO_16(0x00B22222)
#define Clr16FloralWhite          COLOR_CONVERSION_32_TO_16(0x00FFFAF0)
#define Clr16ForestGreen          COLOR_CONVERSION_32_TO_16(0x00228B22)
#define Clr16Fuchsia              COLOR_CONVERSION_32_TO_16(0x00FF00FF)
#define Clr16Gainsboro            COLOR_CONVERSION_32_TO_16(0x00DCDCDC)
#define Clr16GhostWhite           COLOR_CONVERSION_32_TO_16(0x00F8F8FF)
#define Clr16Gold                 COLOR_CONVERSION_32_TO_16(0x00FFD700)
#define Clr16Goldenrod            COLOR_CONVERSION_32_TO_16(0x00DAA520)
#define Clr16Gray                 COLOR_CONVERSION_32_TO_16(0x00808080)
#define Clr16Green                COLOR_CONVERSION_32_TO_16(0x00008000)
#define Clr16GreenYellow          COLOR_CONVERSION_32_TO_16(0x00ADFF2F)
#define Clr16Honeydew             COLOR_CONVERSION_32_TO_16(0x00F0FFF0)
#define Clr16HotPink              COLOR_CONVERSION_32_TO_16(0x00FF69B4)
#define Clr16IndianRed            COLOR_CONVERSION_32_TO_16(0x00CD5C5C)
#define Clr16Indigo               COLOR_CONVERSION_32_TO_16(0x004B0082)
#define Clr16Ivory                COLOR_CONVERSION_32_TO_16(0x00FFFFF0)
#define Clr16Khaki                COLOR_CONVERSION_32_TO_16(0x00F0E68C)
#define Clr16Lavender             COLOR_CONVERSION_32_TO_16(0x00E6E6FA)
#define Clr16LavenderBlush        COLOR_CONVERSION_32_TO_16(0x00FFF0F5)
#define Clr16LawnGreen            COLOR_CONVERSION_32_TO_16(0x007CFC00)
#define Clr16LemonChiffon         COLOR_CONVERSION_32_TO_16(0x00FFFACD)
#define Clr16LightBlue            COLOR_CONVERSION_32_TO_16(0x00ADD8E6)
#define Clr16LightCoral           COLOR_CONVERSION_32_TO_16(0x00F08080)
#define Clr16LightCyan            COLOR_CONVERSION_32_TO_16(0x00E0FFFF)
#define Clr16LightGoldenrodYellow COLOR_CONVERSION_32_TO_16(0x00FAFAD2)
#define Clr16LightGreen           COLOR_CONVERSION_32_TO_16(0x0090EE90)
#define Clr16LightGrey            COLOR_CONVERSION_32_TO_16(0x00D3D3D3)
#define Clr16LightPink            COLOR_CONVERSION_32_TO_16(0x00FFB6C1)
#define Clr16LightSalmon          COLOR_CONVERSION_32_TO_16(0x00FFA07A)
#define Clr16LightSeaGreen        COLOR_CONVERSION_32_TO_16(0x0020B2AA)
#define Clr16LightSkyBlue         COLOR_CONVERSION_32_TO_16(0x0087CEFA)
#define Clr16LightSlateGray       COLOR_CONVERSION_32_TO_16(0x00778899)
#define Clr16LightSteelBlue       COLOR_CONVERSION_32_TO_16(0x00B0C4DE)
#define Clr16LightYellow          COLOR_CONVERSION_32_TO_16(0x00FFFFE0)
#define Clr16Lime                 COLOR_CONVERSION_32_TO_16(0x0000FF00)
#define Clr16LimeGreen            COLOR_CONVERSION_32_TO_16(0x0032CD32)
#define Clr16Linen                COLOR_CONVERSION_32_TO_16(0x00FAF0E6)
#define Clr16Magenta              COLOR_CONVERSION_32_TO_16(0x00FF00FF)
#define Clr16Maroon               COLOR_CONVERSION_32_TO_16(0x00800000)
#define Clr16MediumAquamarine     COLOR_CONVERSION_32_TO_16(0x0066CDAA)
#define Clr16MediumBlue           COLOR_CONVERSION_32_TO_16(0x000000CD)
#define Clr16MediumOrchid         COLOR_CONVERSION_32_TO_16(0x00BA55D3)
#define Clr16MediumPurple         COLOR_CONVERSION_32_TO_16(0x009370DB)
#define Clr16MediumSeaGreen       COLOR_CONVERSION_32_TO_16(0x003CB371)
#define Clr16MediumSlateBlue      COLOR_CONVERSION_32_TO_16(0x007B68EE)
#define Clr16MediumSpringGreen    COLOR_CONVERSION_32_TO_16(0x0000FA9A)
#define Clr16MediumTurquoise      COLOR_CONVERSION_32_TO_16(0x0048D1CC)
#define Clr16MediumVioletRed      COLOR_CONVERSION_32_TO_16(0x00C71585)
#define Clr16MidnightBlue         COLOR_CONVERSION_32_TO_16(0x00191970)
#define Clr16MintCream            COLOR_CONVERSION_32_TO_16(0x00F5FFFA)
#define Clr16MistyRose            COLOR_CONVERSION_32_TO_16(0x00FFE4E1)
#define Clr16Moccasin             COLOR_CONVERSION_32_TO_16(0x00FFE4B5)
#define Clr16NavajoWhite          COLOR_CONVERSION_32_TO_16(0x00FFDEAD)
#define Clr16Navy                 COLOR_CONVERSION_32_TO_16(0x00000080)
#define Clr16OldLace              COLOR_CONVERSION_32_TO_16(0x00FDF5E6)
#define Clr16Olive                COLOR_CONVERSION_32_TO_16(0x00808000)
#define Clr16OliveDrab            COLOR_CONVERSION_32_TO_16(0x006B8E23)
#define Clr16Orange               COLOR_CONVERSION_32_TO_16(0x00FFA500)
#define Clr16OrangeRed            COLOR_CONVERSION_32_TO_16(0x00FF4500)
#define Clr16Orchid               COLOR_CONVERSION_32_TO_16(0x00DA70D6)
#define Clr16PaleGoldenrod        COLOR_CONVERSION_32_TO_16(0x00EEE8AA)
#define Clr16PaleGreen            COLOR_CONVERSION_32_TO_16(0x0098FB98)
#define Clr16PaleTurquoise        COLOR_CONVERSION_32_TO_16(0x00AFEEEE)
#define Clr16PaleVioletRed        COLOR_CONVERSION_32_TO_16(0x00DB7093)
#define Clr16PapayaWhip           COLOR_CONVERSION_32_TO_16(0x00FFEFD5)
#define Clr16PeachPuff            COLOR_CONVERSION_32_TO_16(0x00FFDAB9)
#define Clr16Peru                 COLOR_CONVERSION_32_TO_16(0x00CD853F)
#define Clr16Pink                 COLOR_CONVERSION_32_TO_16(0x00FFC0CB)
#define Clr16Plum                 COLOR_CONVERSION_32_TO_16(0x00DDA0DD)
#define Clr16PowderBlue           COLOR_CONVERSION_32_TO_16(0x00B0E0E6)
#define Clr16Purple               COLOR_CONVERSION_32_TO_16(0x00800080)
#define Clr16Red                  COLOR_CONVERSION_32_TO_16(0x00FF0000)
#define Clr16RosyBrown            COLOR_CONVERSION_32_TO_16(0x00BC8F8F)
#define Clr16RoyalBlue            COLOR_CONVERSION_32_TO_16(0x004169E1)
#define Clr16SaddleBrown          COLOR_CONVERSION_32_TO_16(0x008B4513)
#define Clr16Salmon               COLOR_CONVERSION_32_TO_16(0x00FA8072)
#define Clr16SandyBrown           COLOR_CONVERSION_32_TO_16(0x00F4A460)
#define Clr16SeaGreen             COLOR_CONVERSION_32_TO_16(0x002E8B57)
#define Clr16Seashell             COLOR_CONVERSION_32_TO_16(0x00FFF5EE)
#define Clr16Sienna               COLOR_CONVERSION_32_TO_16(0x00A0522D)
#define Clr16Silver               COLOR_CONVERSION_32_TO_16(0x00C0C0C0)
#define Clr16SkyBlue              COLOR_CONVERSION_32_TO_16(0x0087CEEB)
#define Clr16SlateBlue            COLOR_CONVERSION_32_TO_16(0x006A5ACD)
#define Clr16SlateGray            COLOR_CONVERSION_32_TO_16(0x00708090)
#define Clr16Snow                 COLOR_CONVERSION_32_TO_16(0x00FFFAFA)
#define Clr16SpringGreen          COLOR_CONVERSION_32_TO_16(0x0000FF7F)
#define Clr16SteelBlue            COLOR_CONVERSION_32_TO_16(0x004682B4)
#define Clr16Tan                  COLOR_CONVERSION_32_TO_16(0x00D2B48C)
#define Clr16Teal                 COLOR_CONVERSION_32_TO_16(0x00008080)
#define Clr16Thistle              COLOR_CONVERSION_32_TO_16(0x00D8BFD8)
#define Clr16Tomato               COLOR_CONVERSION_32_TO_16(0x00FF6347)
#define Clr16Turquoise            COLOR_CONVERSION_32_TO_16(0x0040E0D0)
#define Clr16Violet               COLOR_CONVERSION_32_TO_16(0x00EE82EE)
#define Clr16Wheat                COLOR_CONVERSION_32_TO_16(0x00F5DEB3)
#define Clr16White                COLOR_CONVERSION_32_TO_16(0x00FFFFFF)
#define Clr16WhiteSmoke           COLOR_CONVERSION_32_TO_16(0x00F5F5F5)
#define Clr16Yellow               COLOR_CONVERSION_32_TO_16(0x00FFFF00)
#define Clr16YellowGreen          COLOR_CONVERSION_32_TO_16(0x009ACD32)

#define Clr32AliceBlue            0x00F0F8FF
#define Clr32AntiqueWhite         0x00FAEBD7
#define Clr32Aqua                 0x0000FFFF
#define Clr32Aquamarine           0x007FFFD4
#define Clr32Azure                0x00F0FFFF
#define Clr32Beige                0x00F5F5DC
#define Clr32Bisque               0x00FFE4C4
#define Clr32Black                0x00000000
#define Clr32BlanchedAlmond       0x00FFEBCD
#define Clr32Blue                 0x000000FF
#define Clr32BlueViolet           0x008A2BE2
#define Clr32Brown                0x00A52A2A
#define Clr32BurlyWood            0x00DEB887
#define Clr32CadetBlue            0x005F9EA0
#define Clr32Chartreuse           0x007FFF00
#define Clr32Chocolate            0x00D2691E
#define Clr32Coral                0x00FF7F50
#define Clr32CornflowerBlue       0x006495ED
#define Clr32Cornsilk             0x00FFF8DC
#define Clr32Crimson              0x00DC143C
#define Clr32Cyan                 0x0000FFFF
#define Clr32DarkBlue             0x0000008B
#define Clr32DarkCyan             0x00008B8B
#define Clr32DarkGoldenrod        0x00B8860B
#define Clr32DarkGray             0x00A9A9A9
#define Clr32DarkGreen            0x00006400
#define Clr32DarkKhaki            0x00BDB76B
#define Clr32DarkMagenta          0x008B008B
#define Clr32DarkOliveGreen       0x00556B2F
#define Clr32DarkOrange           0x00FF8C00
#define Clr32DarkOrchid           0x009932CC
#define Clr32DarkRed              0x008B0000
#define Clr32DarkSalmon           0x00E9967A
#define Clr32DarkSeaGreen         0x008FBC8F
#define Clr32DarkSlateBlue        0x00483D8B
#define Clr32DarkSlateGray        0x002F4F4F
#define Clr32DarkTurquoise        0x0000CED1
#define Clr32DarkViolet           0x009400D3
#define Clr32DeepPink             0x00FF1493
#define Clr32DeepSkyBlue          0x0000BFFF
#define Clr32DimGray              0x00696969
#define Clr32DodgerBlue           0x001E90FF
#define Clr32FireBrick            0x00B22222
#define Clr32FloralWhite          0x00FFFAF0
#define Clr32ForestGreen          0x00228B22
#define Clr32Fuchsia              0x00FF00FF
#define Clr32Gainsboro            0x00DCDCDC
#define Clr32GhostWhite           0x00F8F8FF
#define Clr32Gold                 0x00FFD700
#define Clr32Goldenrod            0x00DAA520
#define Clr32Gray                 0x00808080
#define Clr32Green                0x00008000
#define Clr32GreenYellow          0x00ADFF2F
#define Clr32Honeydew             0x00F0FFF0
#define Clr32HotPink              0x00FF69B4
#define Clr32IndianRed            0x00CD5C5C
#define Clr32Indigo               0x004B0082
#define Clr32Ivory                0x00FFFFF0
#define Clr32Khaki                0x00F0E68C
#define Clr32Lavender             0x00E6E6FA
#define Clr32LavenderBlush        0x00FFF0F5
#define Clr32LawnGreen            0x007CFC00
#define Clr32LemonChiffon         0x00FFFACD
#define Clr32LightBlue            0x00ADD8E6
#define Clr32LightCoral           0x00F08080
#define Clr32LightCyan            0x00E0FFFF
#define Clr32LightGoldenrodYellow 0x00FAFAD2
#define Clr32LightGreen           0x0090EE90
#define Clr32LightGrey            0x00D3D3D3
#define Clr32LightPink            0x00FFB6C1
#define Clr32LightSalmon          0x00FFA07A
#define Clr32LightSeaGreen        0x0020B2AA
#define Clr32LightSkyBlue         0x0087CEFA
#define Clr32LightSlateGray       0x00778899
#define Clr32LightSteelBlue       0x00B0C4DE
#define Clr32LightYellow          0x00FFFFE0
#define Clr32Lime                 0x0000FF00
#define Clr32LimeGreen            0x0032CD32
#define Clr32Linen                0x00FAF0E6
#define Clr32Magenta              0x00FF00FF
#define Clr32Maroon               0x00800000
#define Clr32MediumAquamarine     0x0066CDAA
#define Clr32MediumBlue           0x000000CD
#define Clr32MediumOrchid         0x00BA55D3
#define Clr32MediumPurple         0x009370DB
#define Clr32MediumSeaGreen       0x003CB371
#define Clr32MediumSlateBlue      0x007B68EE
#define Clr32MediumSpringGreen    0x0000FA9A
#define Clr32MediumTurquoise      0x0048D1CC
#define Clr32MediumVioletRed      0x00C71585
#define Clr32MidnightBlue         0x00191970
#define Clr32MintCream            0x00F5FFFA
#define Clr32MistyRose            0x00FFE4E1
#define Clr32Moccasin             0x00FFE4B5
#define Clr32NavajoWhite          0x00FFDEAD
#define Clr32Navy                 0x00000080
#define Clr32OldLace              0x00FDF5E6
#define Clr32Olive                0x00808000
#define Clr32OliveDrab            0x006B8E23
#define Clr32Orange               0x00FFA500
#define Clr32OrangeRed            0x00FF4500
#define Clr32Orchid               0x00DA70D6
#define Clr32PaleGoldenrod        0x00EEE8AA
#define Clr32PaleGreen            0x0098FB98
#define Clr32PaleTurquoise        0x00AFEEEE
#define Clr32PaleVioletRed        0x00DB7093
#define Clr32PapayaWhip           0x00FFEFD5
#define Clr32PeachPuff            0x00FFDAB9
#define Clr32Peru                 0x00CD853F
#define Clr32Pink                 0x00FFC0CB
#define Clr32Plum                 0x00DDA0DD
#define Clr32PowderBlue           0x00B0E0E6
#define Clr32Purple               0x00800080
#define Clr32Red                  0x00FF0000
#define Clr32RosyBrown            0x00BC8F8F
#define Clr32RoyalBlue            0x004169E1
#define Clr32SaddleBrown          0x008B4513
#define Clr32Salmon               0x00FA8072
#define Clr32SandyBrown           0x00F4A460
#define Clr32SeaGreen             0x002E8B57
#define Clr32Seashell             0x00FFF5EE
#define Clr32Sienna               0x00A0522D
#define Clr32Silver               0x00C0C0C0
#define Clr32SkyBlue              0x0087CEEB
#define Clr32SlateBlue            0x006A5ACD
#define Clr32SlateGray            0x00708090
#define Clr32Snow                 0x00FFFAFA
#define Clr32SpringGreen          0x0000FF7F
#define Clr32SteelBlue            0x004682B4
#define Clr32Tan                  0x00D2B48C
#define Clr32Teal                 0x00008080
#define Clr32Thistle              0x00D8BFD8
#define Clr32Tomato               0x00FF6347
#define Clr32Turquoise            0x0040E0D0
#define Clr32Violet               0x00EE82EE
#define Clr32Wheat                0x00F5DEB3
#define Clr32White                0x00FFFFFF
#define Clr32WhiteSmoke           0x00F5F5F5
#define Clr32Yellow               0x00FFFF00
#define Clr32YellowGreen          0x009ACD32

typedef struct {
	int16_t x_min;
	int16_t x_max;
	int16_t y_min;
	int16_t y_max;
}box_t;


int draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap, uint16_t foreColor, uint16_t inkColor);
void draw_circle(spi_t *inst, box_t *box, uint8_t *buf, signed int x, signed int y, unsigned int _radius, bool fill, uint32_t color);
void draw_line(spi_t *inst, box_t *box, uint8_t *buf, signed int X1, signed int Y1, signed int X2, signed int Y2, uint8_t width, uint32_t color);
void draw_elipse(spi_t *inst, box_t *box, uint8_t *buf, signed int x,signed int y,unsigned int rx,unsigned int ry, bool fill, uint32_t color);
void draw_triangle(spi_t *inst, box_t *box, uint8_t *buf, signed int  ax,signed int  ay,signed int  bx,signed int  by,signed int  cx,signed int  cy, bool fill, uint32_t color);

void draw_msg_and_progress(spi_t *spi_screen, uint8_t *screen_buf, const char *text_P, char *text_R, int32_t progress_min, int32_t progress_max, int32_t progress_value);
void draw_msg(spi_t *spi_screen, uint8_t *screen_buf, const char *text_P, char *text_R, uint8_t x, uint8_t y);

#endif /* DRAW_H_ */