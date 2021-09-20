/*
 * vs10xx.c
 *
 * Created: 19.06.2020 21:33:26
 *  Author: MorgothCreator
 */ 

#include "vs10xx.h"

#include "delay.h"
#include "def.h"
 
void vs10xx_assert_rst() {
	VS10xx_RST_PORT &= ~VS10xx_RST_PIN;
}

void vs10xx_deassert_rst() {
	VS10xx_RST_PORT |= VS10xx_RST_PIN;
}

static inline void vs10xx_assert_dcs() {
	SPI_xDCS_CS_ASSERT();
}

static inline void vs10xx_deassert_dcs() {
	SPI_xDCS_CS_DEASSERT();
}

static inline void vs10xx_assert_cs() {
	SPI_xCS_CS_ASSERT();
}

static inline void vs10xx_deassert_cs() {
	SPI_xCS_CS_DEASSERT();
}

void vs10xx_check_busy() {
	while(~VS10xx_DREQ_IN & VS10xx_DREQ_PIN);
}

bool vs10xx_check_busy_skip() {
	if(~VS10xx_DREQ_IN & VS10xx_DREQ_PIN)
		return true;
	else
		return false;
}

void vs10xx_reg_write(spi_t *spi, uint8_t reg, uint16_t value) {
	vs10xx_check_busy();
	vs10xx_assert_cs();
	unsigned char buffer[4];
	buffer[0] = VS_INS_WRITE;
	buffer[1] = reg;
	buffer[2] = value >> 8;
	buffer[3] = value;
	spi_wr_buf(spi, buffer, 4);
	vs10xx_deassert_cs();
	vs10xx_check_busy();
}

uint16_t vs10xx_reg_read(spi_t *spi, unsigned char reg) {
	uint16_t value;
	vs10xx_check_busy();
	vs10xx_assert_cs();
	spi_wrd_byte(spi, VS_INS_READ);
	spi_wrd_byte(spi, reg);
	value = (spi_wrd_byte(spi, 0xFF) << 8) | spi_wrd_byte(spi, 0xFF);
	vs10xx_deassert_cs();
	return value;
}
#define VS1011
void vs10xx_soft_reset(spi_t *spi) {
	*spi->spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
#ifdef VS10XX_CRISTAL_FREQ
	vs10xx_set_pll(spi, VS10XX_CRISTAL_FREQ);
#else
	vs10xx_set_pll(spi, 12288000);
#endif
	*spi->spcr = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
	vs10xx_reg_read(spi, VS_SCI_STATUS);
	vs10xx_reg_write(spi, VS_SCI_MODE, VS_SM_SDINEW | VS_SM_RESET);
	//vs10xx_reg_write(param, VS_SCI_CLOCKF, 0x2000);
    // Switch on the analog parts
	//vs10xx_reg_write(spi, VS_SCI_AUDATA, 44101); // 44.1kHz stereo
	// The next clocksetting allows SPI clocking at 5 MHz, 4 MHz is safe then.
	//vs10xx_reg_write(spi, VS_SCI_CLOCKF, 6 << 12); // Normal clock settings multiplyer 3.0 = 12.2 MHz
	delay_ms(2);
}

void vs10xx_hard_reset(spi_t *spi) {
	*spi->spsr = (0<<SPI2X);
	*spi->spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	vs10xx_assert_rst();
	delay_ms(1);
	vs10xx_deassert_rst();
	delay_ms(200);
#ifdef VS10XX_CRISTAL_FREQ
	vs10xx_set_pll(spi, VS10XX_CRISTAL_FREQ);
#else
	vs10xx_set_pll(spi, 12288000);
#endif
	*spi->spcr = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
	//*spi->spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (0 << SPR0);
	//*spi->spsr |= (1<<SPI2X);
	vs10xx_reg_read(spi, VS_SCI_STATUS);
	vs10xx_reg_write(spi, VS_SCI_MODE, VS_SM_SDINEW);
	delay_ms(2);
	delay_ms(2);
}

void vs10xx_set_pll(spi_t *spi, uint32_t qFreq) {
	vs10xx_reg_write(spi, VS_SCI_CLOCKF, 0x8000 + (qFreq / 2000));
}

void vs10xx_set_volume(spi_t *spi, uint8_t r, uint8_t l) {
	vs10xx_reg_write(spi, VS_SCI_VOL, r + (l << 8));
}

uint16_t vs10xx_get_volume(spi_t *spi) {
	return vs10xx_reg_read(spi, VS_SCI_VOL);
}

void vs10xx_set_bas(spi_t *spi, uint8_t bl, uint8_t bc, uint8_t tl, uint8_t tc ) {
	vs10xx_reg_write(spi, VS_SCI_BASS, ((bl & 0x000F) << 12) | ((bc & 0x000F) << 8) | ((tl & 0x000F) << 4) | (tc & 0x000F));// 0x7AFA
}

void vs10xx_send_cancel(spi_t *spi) {
	vs10xx_reg_write(spi, VS_SCI_MODE, vs10xx_reg_read(spi, VS_SCI_MODE) | VS_SM_CANCEL);// 0x7AFA
}

void vs10xx_send_null(spi_t *spi, uint16_t len) {
	vs10xx_check_busy();
	vs10xx_assert_dcs();
	uint16_t nullCount;
	for(nullCount = 0; nullCount < len; nullCount++)
		spi_wrd_byte(spi, 255);
	vs10xx_deassert_dcs();
}

bool vs10xx_send_32B_data(spi_t *spi, uint8_t* buffer) {
	if(vs10xx_check_busy_skip())
		return false;
	vs10xx_assert_dcs();
	spi_wr_buf(spi, buffer, 32);
	vs10xx_deassert_dcs();
	return true;
}

bool vs10xx_send_1byte_data(spi_t *spi, uint8_t data) {
	if(vs10xx_check_busy_skip()) {
		spi_wrd_byte(spi, 255);
		return true;
	}
	return false;
}

void vs10xx_init(spi_t *spi) {
	VS10xx_RST_DIR |= VS10xx_RST_PIN;
	VS10xx_DREQ_DIR &= ~VS10xx_DREQ_PIN;
	vs10xx_hard_reset(spi);
}
