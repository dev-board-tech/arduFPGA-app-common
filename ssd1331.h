/*
 * SSD1331 OLED display driver for BOOT-LOADRER of ARDUFPGA soft core design.
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


#ifndef SSD1331_H_
#define SSD1331_H_

#include <stdbool.h>
#include <stdint.h>
#include "def.h"
#include "spi.h"

#define SSD1331_CMD_DRAWLINE						0x21
#define SSD1331_CMD_DRAWRECTANGLE					0x22
#define SSD1331_CMD_COPYWINDOW						0x23
#define SSD1331_CMD_DIMWINDOW						0x24
#define SSD1331_CMD_CLEARWINDOW						0x25
#define SSD1331_CMD_FILLWINDOW						0x26
#define SSD1331_DISABLE_FILL						0x00
#define SSD1331_ENABLE_FILL							0x01
#define SSD1331_CMD_CONTINUOUSSCROLLINGSETUP		0x27
#define SSD1331_CMD_DEACTIVESCROLLING				0x2E
#define SSD1331_CMD_ACTIVESCROLLING					0x2F

#define SSD1331_CMD_SETCOLUMNADDRESS				0x15
#define SSD1331_CMD_SETROWADDRESS					0x75
#define SSD1331_CMD_SETCONTRASTA					0x81
#define SSD1331_CMD_SETCONTRASTB					0x82
#define SSD1331_CMD_SETCONTRASTC					0x83
#define SSD1331_CMD_MASTERCURRENTCONTROL			0x87
#define SSD1331_CMD_SETPRECHARGESPEEDA				0x8A
#define SSD1331_CMD_SETPRECHARGESPEEDB				0x8B
#define SSD1331_CMD_SETPRECHARGESPEEDC				0x8C
#define SSD1331_CMD_SETREMAP						0xA0
#define SSD1331_CMD_SETDISPLAYSTARTLINE				0xA1
#define SSD1331_CMD_SETDISPLAYOFFSET				0xA2
#define SSD1331_CMD_NORMALDISPLAY					0xA4
#define SSD1331_CMD_ENTIREDISPLAYON					0xA5
#define SSD1331_CMD_ENTIREDISPLAYOFF				0xA6
#define SSD1331_CMD_INVERSEDISPLAY					0xA7
#define SSD1331_CMD_SETMULTIPLEXRATIO				0xA8
#define SSD1331_CMD_DIMMODESETTING					0xAB
#define SSD1331_CMD_SETMASTERCONFIGURE				0xAD
#define SSD1331_CMD_DIMMODEDISPLAYON				0xAC
#define SSD1331_CMD_DISPLAYOFF						0xAE
#define SSD1331_CMD_DISPLAYON						0xAF
#define SSD1331_CMD_POWERSAVEMODE					0xB0
#define SSD1331_CMD_PHASEPERIODADJUSTMENT			0xB1
#define SSD1331_CMD_DISPLAYCLOCKDIV					0xB3
#define SSD1331_CMD_SETGRAySCALETABLE				0xB8
#define SSD1331_CMD_ENABLELINEARGRAYSCALETABLE		0xB9
#define SSD1331_CMD_SETPRECHARGEVOLTAGE				0xBB
#define SSD1331_CMD_SETVVOLTAGE						0xBE

void ssd1331_init(spi_t *inst);
void ssd1331_wr_cmd(spi_t *inst, uint8_t cmd);
void ssd1331_wr_data(spi_t *inst, uint8_t cmd);
void ssd1331_on(spi_t *inst, bool state);
void ssd1331_set_contrast(spi_t *inst, uint8_t cont);
uint16_t ssd1331_get_x(void);
uint16_t ssd1331_get_y(void);
void ssd1331_put_pixel(spi_t *inst, box_t *box, int16_t x, int16_t y, bool state);
void ssd1331_put_rectangle(spi_t *inst, box_t *box, int16_t x, int16_t y, int16_t x_size, int16_t y_size, bool fill, bool state);
void ssd1331_put_h_line(spi_t *inst, box_t *box, int16_t x1, int16_t x2, int16_t y, uint8_t width, bool state);
void ssd1331_put_v_line(spi_t *inst, box_t *box, int16_t y1, int16_t y2, int16_t x, uint8_t width, bool state);
void ssd1331_clear(spi_t *inst, bool state);
int ssd1331_draw_string(spi_t *inst, box_t *box, char *string, int16_t x, int16_t y, bool terminalMode, bool wordWrap, bool foreColor, bool inkColor);

#endif /* SSD1331_H_ */