/*
 * twi.c
 *
 * Created: 20.11.2020 09:31:33
 *  Author: MorgothCreator
 */ 

#include "twi.h"
#include "def.h"

static twiErrCode_e twStart(twi_t *inst) {
	/* Send START condition */
	*inst->twcr =  (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	/* Wait for TWINT flag to set */
	while (!(*inst->twcr & (1 << TWINT)));
	/* Check error */
	if ((*inst->twsr & TW_STATUS_MASK) != TW_START && (*inst->twsr & TW_STATUS_MASK) != TW_REP_START) {
		return (*inst->twsr & TW_STATUS_MASK);
	}
	return twiErr_Ok;
}

static void twStop(twi_t *inst) {
	/* Send STOP condition */
	*inst->twcr = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

static twiErrCode_e twWriteSla(twi_t *inst, uint8_t sla) {
	/* Transmit slave address with read/write flag */
	*inst->twdr = sla;
	*inst->twcr = (1 << TWINT) | (1 << TWEN);
	
	/* Wait for TWINT flag to set */
	while (!(*inst->twcr & (1 << TWINT)));
	if ((*inst->twsr & TW_STATUS_MASK) != TW_MT_SLA_ACK && (*inst->twsr & TW_STATUS_MASK) != TW_MR_SLA_ACK) {
		return (*inst->twsr & TW_STATUS_MASK);
	}
	return twiErr_Ok;
}

static twiErrCode_e twWrite(twi_t *inst, uint8_t data) {
	/* Transmit 1 byte*/
	*inst->twdr = data;
	*inst->twcr = (1 << TWINT) | (1 << TWEN);
	
	/* Wait for TWINT flag to set */
	while (!(*inst->twcr & (1 << TWINT)));
	if ((*inst->twsr & TW_STATUS_MASK) != TW_MT_DATA_ACK) {
		return (*inst->twsr & TW_STATUS_MASK);
	}
	return twiErr_Ok;
}

static uint8_t twRead(twi_t *inst, bool read_ack) {
	if (read_ack) {
		*inst->twcr = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		while (!(*inst->twcr & (1 << TWINT)));
		if ((*inst->twsr & TW_STATUS_MASK) != TW_MR_DATA_ACK) {
			return (*inst->twsr & TW_STATUS_MASK);
		}
	} else {
		*inst->twcr = (1 << TWINT) | (1 << TWEN);
		while (!(*inst->twcr & (1 << TWINT)));
		if ((*inst->twsr & TW_STATUS_MASK) != TW_MR_DATA_NACK) {
			return (*inst->twsr & TW_STATUS_MASK);
		}
	}
	uint8_t data = *inst->twdr;
	return data;
}

static int8_t twiMTx(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len, bool repeat_start) {
	twiErrCode_e error_code;
	/* Send START condition */
	int i = 0;
	error_code = twStart(inst);
	if (error_code != twiErr_Ok) {
		return i;
	}
	/* Send slave address with WRITE flag */
	error_code = twWriteSla(inst, TW_SLA_W(slaveAddr));
	if (error_code != twiErr_Ok) {
		return i;
	}
	/* Send data byte in single or burst mode */
	for (i = 0; i < len; ++i) {
		error_code = twWrite(inst, buff[i]);
		if (error_code != twiErr_Ok) {
			return i;
		}
	}
	if (!repeat_start) {
		/* Send STOP condition */
		twStop(inst);
	}
	return len;
}

void twi_init(twi_t *inst, uint32_t twiFreq) {
	*inst->twbr = ((F_CPU / twiFreq) - 16) / 2;
}

int8_t twi_writeBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len) {
	return twiMTx(inst, slaveAddr, buff, len, false);
}

int8_t twi_readBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len) {
	twiErrCode_e error_code;
	int i = 0;
	/* Send START condition */
	error_code = twStart(inst);
	if (error_code != twiErr_Ok) {
		return i;
	}
	/* Write slave address with READ flag */
	error_code = twWriteSla(inst, TW_SLA_R(slaveAddr));
	if (error_code != twiErr_Ok) {
		return i;
	}
	/* Read single or multiple data byte and send ack */
	for (i = 0; i < len-1; ++i) {
		buff[i] = twRead(inst, TW_READ_ACK);
	}
	buff[len-1] = twRead(inst, TW_READ_NACK);
	/* Send STOP condition */
	twStop(inst);
	return len;
}

int8_t twi_writeRead(twi_t *inst, uint8_t slaveAddr, uint8_t* buffSend, uint8_t lenSend, 
		uint8_t* p_data_rx, uint16_t rx_len) {
	twiErrCode_e bytes_snd;
	if((bytes_snd = twiMTx(inst, slaveAddr, buffSend, lenSend, true)) != lenSend) {
		return bytes_snd;
	}
	return twi_readBytes(inst, slaveAddr, p_data_rx, rx_len);
}