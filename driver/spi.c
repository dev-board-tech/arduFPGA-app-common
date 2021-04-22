/*
 * SPI driver file for arduFPGA designs.
 * 
 * Copyright (C) 2020  Iulian Gheorghiu (morgoth@mail.devboard.tech)
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

#include "def.h"
#include "spi.h"
#include <avr/io.h>

void spi_init(spi_t *inst) {
#ifdef SPI_CS_1_PORT
	SPI_CS_1_DIR |= SPI_CS_1_PIN;
#endif
#ifdef SPI_CS_2_PORT
	SPI_CS_2_DIR |= SPI_CS_2_PIN;
#endif
#ifdef SPI_CS_3_PORT
	SPI_CS_3_DIR |= SPI_CS_3_PIN;
#endif
#ifdef SPI_CS_4_PORT
	SPI_CS_4_DIR |= SPI_CS_4_PIN;
#endif
#ifdef SPI_CS_5_PORT
	SPI_CS_5_DIR |= SPI_CS_5_PIN;
#endif
#ifdef SPI_CS_6_PORT
	SPI_CS_6_DIR |= SPI_CS_6_PIN;
#endif
#ifdef SPI_CS_7_PORT
	SPI_CS_7_DIR |= SPI_CS_7_PIN;
#endif
	SPI_SCK_DIR |= SPI_SCK_PIN;
	SPI_MOSI_DIR |= SPI_MOSI_PIN;
	*inst->spcr = (1<<SPE) | (1<<MSTR);
	*inst->spsr = (1<<SPI2X);
}

uint8_t spi_wrd_byte(spi_t *inst, uint8_t b) {
	*inst->spdr = b;
	while ( !( *inst->spsr & (1<<SPIF)) );
	return *inst->spdr;
}

void spi_wrd_buf(spi_t *inst, uint8_t *rx_buf, uint8_t *tx_buf, uint16_t count) {
	while(count--) {
		*rx_buf++ = spi_wrd_byte(inst, *tx_buf++);
	}
}

void spi_wr_buf(spi_t *inst, uint8_t *tx_buf, uint16_t count) {
	while(count--) {
		spi_wrd_byte(inst, *tx_buf++);
	}
}

void spi_rd_buf(spi_t *inst, uint8_t *rx_buf, uint16_t count) {
	while(count--) {
		*rx_buf++ = spi_wrd_byte(inst, 0xFF);
	}
}

