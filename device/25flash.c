/*
 * 25 series of SPI FLASH memory driver file for arduFPGA designs.
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

#include <stdbool.h>
#include "def.h"
#include "25flash.h"

static uint8_t _25flash_read_status(_25flash_t *dev) {
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, 0x05);
	uint8_t tmp = spi_wrd_byte(dev->spi, 0xFF);
	*dev->cs_port_out |= dev->pin_mask;
	return tmp;
}

static void _25flash_write_enable(_25flash_t *dev, bool state) {
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, state ? 0x06 : 0x04);
	*dev->cs_port_out |= dev->pin_mask;
	uint8_t status;
	do {
		status = _25flash_read_status(dev);
		status = state ? (~status & 0x02) : (status & 0x02);
	} while (status);
}

void _24flash_write_status(_25flash_t *dev, uint8_t status) {
	_25flash_write_enable(dev, true);
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, 0x01);
	spi_wrd_byte(dev->spi, status);
	*dev->cs_port_out |= dev->pin_mask;
	while(_25flash_read_status(dev) & 0x01);
}


void _25flash_write(_25flash_t *dev, uint32_t addr, uint8_t *buff, uint16_t size) {
	_25flash_write_enable(dev, true);
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, 0x02);
	spi_wrd_byte(dev->spi, addr >> 16);
	spi_wrd_byte(dev->spi, addr >> 8);
	spi_wrd_byte(dev->spi, addr);
	while (size--) {
		spi_wrd_byte(dev->spi, *buff++);
	}
	*dev->cs_port_out |= dev->pin_mask;
	while(_25flash_read_status(dev) & 0x01);
}

void _25flash_read(_25flash_t *dev, uint32_t addr, uint8_t *buff, uint16_t size) {
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, 0x0B);
	spi_wrd_byte(dev->spi, addr >> 16);
	spi_wrd_byte(dev->spi, addr >> 8);
	spi_wrd_byte(dev->spi, addr);
	spi_wrd_byte(dev->spi, 0xFF);
	while (size--) {
		*buff++ = spi_wrd_byte(dev->spi, 0xFF);
	}
	*dev->cs_port_out |= dev->pin_mask;
}

void _25flash_erase(_25flash_t *dev, uint32_t addr) {
	_25flash_write_enable(dev, true);
	*dev->cs_port_out &= ~dev->pin_mask;
	spi_wrd_byte(dev->spi, 0x20);
	spi_wrd_byte(dev->spi, addr >> 16);
	spi_wrd_byte(dev->spi, addr >> 8);
	spi_wrd_byte(dev->spi, addr);
	*dev->cs_port_out |= dev->pin_mask;
	while(_25flash_read_status(dev) & 0x01);
}



