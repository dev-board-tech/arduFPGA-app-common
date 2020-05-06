/*
 * 25 series of SPI FLASH memory driver for BOOT-LOADRER of ARDUFPGA soft core design.
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


#ifndef __25FLASH_H__
#define __25FLASH_H__

#include <stdint.h>
#include "spi.h"

typedef struct _25flash_s 
{
	spi_t *spi;
	volatile uint8_t *cs_port_out;
	uint8_t pin_mask;
}_25flash_t;

void _24flash_write_status(_25flash_t *dev, uint8_t status);
void _25flash_write(_25flash_t *dev, uint32_t addr, uint8_t *buff, uint16_t size);
void _25flash_read(_25flash_t *dev, uint32_t addr, uint8_t *buff, uint16_t size);
void _25flash_erase(_25flash_t *dev, uint32_t addr);

#endif /* 25FLASH_H_ */