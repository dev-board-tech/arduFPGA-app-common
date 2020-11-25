/*
 * twi.h
 *
 * Created: 20.11.2020 09:31:45
 *  Author: MorgothCreator
 */ 


#ifndef TWI_H_
#define TWI_H_
#include <stdint.h>

#include <avr/io.h>
#include <util/twi.h>
#include <stdbool.h>

#define TW_SLA_W(ADDR)		((ADDR << 1) | TW_WRITE)
#define TW_SLA_R(ADDR)		((ADDR << 1) | TW_READ)
#define TW_READ_ACK			1
#define TW_READ_NACK		0

typedef enum {
	twiErr_Ok = 0,
	twiErr_InvalidParam,
	twiErr_Nack
} twiErrCode_e;

typedef struct
{
	volatile uint8_t *twbr;
	volatile uint8_t *twsr;
	volatile uint8_t *twar;
	volatile uint8_t *twdr;
	volatile uint8_t *twcr;
	volatile uint8_t *twamr;
}twi_t;

void twi_init(twi_t *inst, uint32_t twiFreq);
int8_t twi_writeBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len);
int8_t twi_readBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len);
int8_t twi_writeRead(twi_t *inst, uint8_t slaveAddr, uint8_t* buffSend, uint8_t lenSend, uint8_t* p_data_rx, uint16_t rx_len);





#endif /* TWI_H_ */