/*
 * vs10xx.c
 *
 * Created: 19.06.2020 21:33:26
 *  Author: MorgothCreator
 */ 

#include "vs10xx.h"
#include <stdbool.h>
#include "delay.h"
#include "spi.h"
#include "def.h"
 
static inline void vs10xx_assert_rst() {
	VS10xx_RST_PORT &= ~VS10xx_RST_PIN;
}

static inline void vs10xx_deassert_rst() {
	VS10xx_RST_PORT |= VS10xx_RST_PIN;
}

static inline void vs10xx_assert_dcs() {
	SPI_CS_7_PORT &= ~SPI_CS_7_PIN;
}

static inline void vs10xx_deassert_dcs() {
	SPI_CS_7_PORT |= SPI_CS_7_PIN;
}

static inline void vs10xx_assert_cs() {
	SPI_CS_6_PORT &= ~SPI_CS_6_PIN;
}

static inline void vs10xx_deassert_cs() {
	SPI_CS_6_PORT |= SPI_CS_6_PIN;
}

static inline void vs10xx_check_busy() {
	while(~VS10xx_DREQ_DIR & VS10xx_DREQ_PIN);
}

static inline bool vs10xx_check_busy_skip() {
	if(~VS10xx_DREQ_DIR & VS10xx_DREQ_PIN)
		return true;
	else
		return false;
}

static void vs10xx_reg_write(spi_t *spi, uint8_t reg, uint16_t value) {
	vs10xx_check_busy();
	vs10xx_assert_cs();
	unsigned char buffer[4];
	buffer[0] = VS_INS_WRITE;
	buffer[1] = reg;
	buffer[2] = value >> 8;
	buffer[3] = value;
	spi_wr_buf(spi, buffer, 4);
	vs10xx_deassert_cs();
	delay_us(100);
}

static uint16_t vs10xx_reg_read(spi_t *spi, unsigned char reg) {
	uint16_t value;
	vs10xx_check_busy();
	vs10xx_assert_cs();
	//unsigned char buffer[4];
	//buffer[0] = VS_INS_READ;
	//buffer[1] = Reg;
	//buffer[2] = 0xFF;
	//buffer[3] = 0xFF;
	//_param->spi->Buff = (volatile unsigned char *)buffer;
	spi_wrd_byte(spi, VS_INS_READ);
	spi_wrd_byte(spi, reg);
	value = spi_wrd_byte(spi, 0xFF) <<8;
	value += spi_wrd_byte(spi, 0xFF);
	//Value = buffer[2] << 8;
	//Value += buffer[3];
	vs10xx_deassert_cs();
	delay_us(100);
	return value;
}

void vs10xx_soft_reset(spi_t *spi) {
	vs10xx_reg_write(spi, VS_SCI_MODE, 4);
	vs10xx_reg_write(spi, VS_SCI_MODE, 0x0800);
	vs10xx_reg_write(spi, VS_SCI_BASS, 0x7A00);
	//vs10xx_reg_write(param, VS_SCI_CLOCKF, 0x2000);
	vs10xx_check_busy();
}

void vs10xx_hard_reset(spi_t *spi) {
	vs10xx_assert_rst();
	delay_ms(100);
	vs10xx_deassert_rst();
	vs10xx_reg_write(spi, VS_SCI_MODE, 0x0800);
	vs10xx_reg_write(spi, VS_SCI_BASS, 0x7A00);
	//vs10xx_reg_write(param, VS_SCI_CLOCKF, 0x2000);
	vs10xx_check_busy();
}

void vs10xx_set_pll(spi_t *spi, uint32_t QFreq) {
	vs10xx_reg_write(spi, VS_SCI_CLOCKF, QFreq/2000);
}

void vs10xx_set_volume(spi_t *spi, unsigned char R, unsigned char L) {
	vs10xx_reg_write(spi, VS_SCI_VOL, R + (L << 8));
}

unsigned short vs10xx_get_volume(spi_t *spi) {
	return vs10xx_reg_read(spi, VS_SCI_VOL);
}

void vs10xx_send_null(spi_t *spi, uint16_t len) {
	vs10xx_check_busy();
	vs10xx_assert_dcs();
	uint16_t nullCount;
	for(nullCount = 0; nullCount < len; nullCount++)
		spi_wrd_byte(spi, 255);
	vs10xx_deassert_dcs();
}

bool vs10xx_send_32B_data(spi_t *spi, unsigned char* buffer) {
	if(vs10xx_check_busy_skip())
		return false;
	vs10xx_assert_dcs();
	spi_wr_buf(spi, buffer, 32);
	vs10xx_deassert_dcs();
	return true;
}

bool vs10xx_send_1byte_data(spi_t *spi, unsigned char Data) {
	if(vs10xx_check_busy_skip()) {
		spi_wrd_byte(spi, 255);
		return true;
	}
	return false;
}

void vs10xx_init(spi_t *spi) {
	vs10xx_hard_reset(spi);
	vs10xx_soft_reset(spi);
	vs10xx_set_pll(spi, 12288000);
	delay_ms(100);
}
