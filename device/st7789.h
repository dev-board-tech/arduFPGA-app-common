/*
 * ST7789 TFT LCD display driver file for arduFPGA designs.
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


#ifndef ST7789_H_
#define ST7789_H_

#include <stdbool.h>
#include <stdint.h>
#include "gfx/draw.h"
#include "def.h"
#include "driver/spi.h"


#define ST7789_TFTWIDTH 	240
#define ST7789_TFTHEIGHT 	240

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD



void st7789_init(spi_t *inst, uint8_t *buf);
void st7789_wr_cmd(spi_t *inst, uint8_t cmd);
uint16_t st7789_get_x();
uint16_t st7789_get_y();
void st7789_set_rotation(spi_t *inst, uint16_t rotation);
void st7789_rfsh(spi_t *inst, uint8_t *buf);
void st7789_on(spi_t *inst, bool state);
void st7789_set_contrast(spi_t *inst, uint8_t cont);
void st7789_draw_pixel(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color);
void st7789_draw_rectangle(spi_t *inst, box_t *box, uint8_t *buf, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color);
void st7789_draw_h_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t x1, int16_t x2, int16_t y, uint8_t width,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color);
void st7789_draw_v_line(spi_t *inst, box_t *box, uint8_t *buf, int16_t y1, int16_t y2, int16_t x, uint8_t width,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color);
void st7789_clear(spi_t *inst, uint8_t *buf,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 color);
int st7789_draw_string(spi_t *inst, box_t *box, uint8_t *buf, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 foreColor,
#ifdef ST7789_BW_MODE
 bool
#else
 uint16_t
#endif
 inkColor);

#endif /* ST7789_H_ */