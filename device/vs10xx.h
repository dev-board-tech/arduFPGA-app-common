/*
 * vs10xx.h
 *
 * Created: 19.06.2020 21:33:12
 *  Author: MorgothCreator
 */ 


#ifndef VS10XX_H_
#define VS10XX_H_

#include <stdbool.h>
#include "spi.h"

/*#####################################################*/
#define VS_SCI_MODE			0x00	//RW	Mode control
#define VS_SCI_STATUS		0x01	//RW	Status
#define VS_SCI_BASS			0x02	//RW	Built-in bass enhancer
#define VS_SCI_CLOCKF		0x03	//RW	Clock freq+doubler
#define VS_SCI_DEC_TIME		0x04	//R		Decode time in seconds
#define VS_SCI_AUDATA		0x05	//RW	Misc. audio data
#define VS_SCI_WRAM			0x06	//RW	RAM write
#define VS_SCI_WRAMADDR		0x07	//RW	Base address for RAM write
#define VS_SCI_HDAT0		0x08	//R		Stream header data 0
#define VS_SCI_HDAT1		0x09	//R		Stream header data 1
#define VS_SCI_AIADDR		0x0A	//RW	Start address of application
#define VS_SCI_VOL			0x0B	//RW	Volume control
#define VS_SCI_AICTRL0		0x0C	//RW	Application control register 0
#define VS_SCI_AICTR11		0x0D	//RW	Application control register 1
#define VS_SCI_AICTRL2		0x0E	//RW	Application control register 2
#define VS_SCI_AICTRL3		0x0F	//RW	Application control register 3

#define VS_INS_WRITE		0x02
#define VS_INS_READ			0x03
/*#####################################################*/

void vs10xx_soft_reset(spi_t *spi);
void vs10xx_hard_reset(spi_t *spi);
void vs10xx_set_pll(spi_t *spi, uint32_t qFreq);
void vs10xx_set_volume(spi_t *spi, uint8_t r, uint8_t l);
uint16_t vs10xx_get_volume(spi_t *spi);
void vs10xx_send_null(spi_t *spi, uint16_t len);
bool vs10xx_send_32B_data(spi_t *spi, uint8_t *buffer);
bool vs10xx_send_1byte_data(spi_t *spi, uint8_t data);
void vs10xx_init(spi_t *spi);


#endif /* VS10XX_H_ */