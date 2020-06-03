/*
 * SPI driver file for arduFPGA designs.
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


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>


typedef struct  
{
	volatile uint8_t *spcr;
	volatile uint8_t *spsr;
	volatile uint8_t *spdr;
}spi_t;

void spi_init(spi_t *inst);
uint8_t spi_wrd_byte(spi_t *inst, uint8_t b);
void spi_wrd_buf(spi_t *inst, uint8_t *rx_buf, uint8_t *tx_buf, uint16_t count);
void spi_wr_buf(spi_t *inst, uint8_t *tx_buf, uint16_t count);
void spi_rd_buf(spi_t *inst, uint8_t *rx_buf, uint16_t count);
#endif /* SPI_H_ */