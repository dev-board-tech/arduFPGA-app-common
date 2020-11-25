/*
 * TWI driver file for arduFPGA designs.
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

#include <stdio.h>

#include "def.h"
#include "twi_s.h"

void twi_init(twi_t *inst, uint32_t twiFreq) {
	unsigned int baud = (F_CPU / (twiFreq * 2)) - 1;
	if(baud > 255)
		baud = 255;
	*inst->baud = baud;
	*inst->ctrla = FPGA_TWI_MASTER_ENABLE_bm;
}

static twiErrCode_e wr(twi_t *inst, uint8_t slaveAddr, uint8_t* bufSend, uint8_t lenSend, uint8_t* bufRec, uint16_t lenRec) {
	unsigned int cnt_bytes;
	if(lenSend)
	{
		*inst->data = slaveAddr;
		while ((*inst->status & FPGA_TWI_MASTER_WIF_bm) == 0);
		if(*inst->status & FPGA_TWI_MASTER_RXACK_bm)
		{
			*inst->ctrlc = FPGA_TWI_MASTER_CMD_STOP_gc;
			while ((*inst->ctrlc & FPGA_TWI_MASTER_CMD_gm) != 0);
			return twiErr_Nack;
		}
		for(cnt_bytes = 0; cnt_bytes < lenSend; cnt_bytes++)
		{
			*inst->data = *bufSend++;
			while ((*inst->status & FPGA_TWI_MASTER_WIF_bm) == 0);
			if(*inst->status & FPGA_TWI_MASTER_RXACK_bm)
			{
				*inst->ctrlc = FPGA_TWI_MASTER_CMD_STOP_gc;
				while ((*inst->ctrlc & FPGA_TWI_MASTER_CMD_gm) != 0);
				return twiErr_Nack;
			}
		}
	}
	if(lenRec)
	{
		if(lenSend)
		{
			*inst->ctrlc = FPGA_TWI_MASTER_CMD_REPSTART_gc;
			while ((*inst->ctrlc & FPGA_TWI_MASTER_CMD_gm) != 0);
		}
		*inst->data = slaveAddr | 0x01;
		while ((*inst->status & FPGA_TWI_MASTER_WIF_bm) == 0);
		if(*inst->status & FPGA_TWI_MASTER_RXACK_bm)
		{
			*inst->ctrlc = FPGA_TWI_MASTER_CMD_STOP_gc;
			while ((*inst->ctrlc & FPGA_TWI_MASTER_CMD_gm) != 0);
			return twiErr_Nack;
		}
		for(cnt_bytes = 0; cnt_bytes < lenRec; cnt_bytes++)
		{
			*inst->ctrlc = FPGA_TWI_MASTER_CMD_RECVTRANS_gc | ((cnt_bytes == lenRec - 1) ? FPGA_TWI_MASTER_ACKACT_bm : 0 );
			while ((*inst->status & FPGA_TWI_MASTER_RIF_bm) == 0);
			*bufRec++ = *inst->data;
		}
	}
	*inst->ctrlc = FPGA_TWI_MASTER_CMD_STOP_gc;
	while ((*inst->ctrlc & FPGA_TWI_MASTER_CMD_gm) != 0);
	return twiErr_Ok;
}

int8_t twi_writeBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buf, uint8_t len) {
	if (!inst)
		return twiErr_InvalidParam;
	return wr(inst, slaveAddr << 1, buf, len, NULL, 0);
}

int8_t twi_readBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buf, uint8_t len) {
	if (!inst)
		return twiErr_InvalidParam;
	return wr(inst, slaveAddr << 1, NULL, 0, buf, len);
}

int8_t twi_writeRead(twi_t *inst, uint8_t slaveAddr, uint8_t* bufSend, uint8_t lenSend, uint8_t* bufRec, uint16_t lenRec) {
	if (!inst)
		return twiErr_InvalidParam;
	return wr(inst, slaveAddr << 1, bufSend, lenSend, bufRec, lenRec);
}

