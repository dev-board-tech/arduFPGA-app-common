/*
 * Main function for second stage BOOT-LOADRER/explorer of ARDUFPGA soft core design.
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "spi.h"
#include "gui.h"
#include "kbd.h"
#include "device/ssd1306.h"
#include "device/25flash.h"
#include "mmc_sd_spi.h"
#include "fat_fs/inc/ff.h"

box_t box = {0, 128, 0, 64};// For ssd1306 128x64 pixels.
//int main(void) __attribute__ ((naked)) __attribute__ ((section (".init1")));
//-Wl,--section-start,.data=0x800B00,--defsym=__heap_end=0x807fff

spi_t spi;
_25flash_t flash_des;
//_25flash_t flash_app;
mmc_sd_t uSD;
FATFS fatFs;
DIR dirObject;
FILINFO fInfo;
FIL filObject;

bool usd_state;

#ifndef SSD1306_USE_NO_BUF
uint8_t screen_buf[1024];
#else /* SSD1306_USE_NO_BUF */
uint8_t screen_buf[1];
#endif /* SSD1306_USE_NO_BUF */

/*void init(void) __attribute__ ((naked)) __attribute__ ((section (".init0")));
int main(void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void init() {
	__asm__ __volatile__ (
	"_init: \n\t"
	"cli \n\t"
	"eor	r1, r1 \n\t"
	"out	0x3f, r1 \n\t"//	; 63
	"ldi	r28, 0xFF \n\t"//	; 255
	"ldi	r29, 0x08 \n\t"//	; 10
	"out	0x3e, r29 \n\t"//	; 62
	"out	0x3d, r28 \n\t");//	; 61
}
*/
int main(void)
{
	asm("cli");
/*	for (int16_t cnt = 0; cnt < 240; cnt++) {
		(*(uint8_t *)cnt) = 0;
	}
	asm("sei");*/
	spi.spcr = &SPCR;
	spi.spsr = &SPSR;
	spi.spdr = &SPDR;
	spi_init(&spi);
	flash_des.spi = &spi;
	flash_des.cs_port_out = &SPI_CS_4_PORT;
	flash_des.pin_mask = SPI_CS_4_PIN;
	//flash_app.spi = &spi;
	//flash_app.cs_port_out = &SPI_CS_5_PORT;
	//flash_app.pin_mask = SPI_CS_5_PIN;
	DISPLAY_FUNC_INIT(&spi, screen_buf);
	uSD.unitNr = 0;
	uSD.fatFs = &fatFs;
	uSD.spi_inst = &spi;
	uSD.initFlg = true;
	mmc_sd_spi_idle(&uSD);
	kbd_init();
	gui_init(&uSD, &spi, screen_buf);
	//uart_init(115200);
	//uart_put_s("Hello world!! \n\r");
    while (1) {
		kbd_idle();
		mmc_sd_spi_idle(&uSD);
		if(uSD.fs_mounted != usd_state) {
			usd_state = uSD.fs_mounted;
			gui_init(&uSD, &spi, screen_buf);
		}
		gui_idle(&uSD, &spi, screen_buf);
		/*int16_t rx_c;
		if((rx_c = uart_get_c_nb()) >= 0)
			uart_put_c(rx_c);*/
    }
}

