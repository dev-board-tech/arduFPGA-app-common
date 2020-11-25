/*
 * freeNove_ThreeWeled driver file for arduFPGA designs.
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

#include <stdlib.h>

#include "freenove_ThreeWeeled.h"

static uint16_t numMap(unsigned short value, unsigned short fromLow, unsigned short fromHigh, unsigned short toLow, unsigned short toHigh) {
	if(value < fromLow) {
		return fromLow;
	} else if(value > fromHigh) {
		return fromHigh;
	}
	return ((value - fromLow) * 0x10000) / (((fromHigh - fromLow) * 0x10000) / toHigh) + toLow;
}

freeNove_ThreeWeled_t *freeNove_ThreeWeled_init(twi_t *twiInst) {
	freeNove_ThreeWeled_t *freeNoveInst = (freeNove_ThreeWeled_t *)calloc(1, sizeof(freeNove_ThreeWeled_t));
	if(!freeNoveInst) {
		return (freeNove_ThreeWeled_t*)0;
	}
    if(!twiInst)
    {
        freeNoveInst->err = twiErr_InvalidParam;
        return freeNoveInst;
    }
    freeNoveInst->twiInst = twiInst;
    freeNoveInst->devAddr = 0x18;
    freeNoveInst->err = twiErr_Ok;
	freeNoveInst->servoMin[0] = 700;
	freeNoveInst->servoMax[0] = 1500;
	freeNoveInst->servoMin[1] = 700;
	freeNoveInst->servoMax[1] = 1500;
	freeNoveInst->servoMin[2] = 700;
	freeNoveInst->servoMax[2] = 1500;
	freeNoveInst->servoMin[3] = 700;
	freeNoveInst->servoMax[3] = 1500;
    return freeNoveInst;
}

freeNove_ThreeWeled_t *freeNove_ThreeWeled_deinit(freeNove_ThreeWeled_t *freeNoveInst) {
	if(freeNoveInst) {
		free(freeNoveInst);
	}
	return (freeNove_ThreeWeled_t*)0;
}

bool freeNove_ThreeWeled_servo(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, unsigned short value) {
    if(!freeNoveInst || !freeNoveInst->twiInst || nr > 3)
        return false;
    unsigned char buff[3];
    unsigned short tmp = numMap(value, 0, 180, freeNoveInst->servoMin[nr], freeNoveInst->servoMax[nr]);
    buff[0] = freeNove_TV_CMD_SERVO1 + nr;
    buff[1] = tmp >> 8;
    buff[2] = tmp;
    if(twi_writeBytes(freeNoveInst->twiInst, freeNoveInst->devAddr, buff, 3) < 1)
        return false;
    return true;
}

bool freeNove_ThreeWeled_pwm(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, unsigned short value) {
    if(!freeNoveInst || !freeNoveInst->twiInst || nr > 1)
        return false;
    unsigned char buff[3];
    buff[0] = freeNove_TV_CMD_PWM1 + nr;
    buff[1] = value >> 8;
    buff[2] = value;
    if(twi_writeBytes(freeNoveInst->twiInst, freeNoveInst->devAddr, buff, 3) < 1)
        return false;
    return true;
}

bool freeNove_ThreeWeled_dir(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, bool value) {
    if(!freeNoveInst || !freeNoveInst->twiInst || nr > 1)
        return false;
    unsigned char buff[3];
    buff[0] = freeNove_TV_CMD_DIR1 + nr;
    buff[1] = value;
    buff[2] = 0;
    if(twi_writeBytes(freeNoveInst->twiInst, freeNoveInst->devAddr, buff, 3) < 1)
        return false;
    return true;
}

bool freeNove_ThreeWeled_buzzer(freeNove_ThreeWeled_t *freeNoveInst, unsigned short value) {
    if(!freeNoveInst || !freeNoveInst->twiInst)
        return false;
    unsigned char buff[3];
    buff[0] = freeNove_TV_CMD_BUZZER;
    buff[1] = value >> 8;
    buff[2] = value;
    if(twi_writeBytes(freeNoveInst->twiInst, freeNoveInst->devAddr, buff, 3) < 1)
        return false;
    return true;
}

bool freeNove_ThreeWeled_io(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, bool value) {
    if(!freeNoveInst || !freeNoveInst->twiInst || nr > 2)
        return false;
    unsigned char buff[3];
    buff[0] = freeNove_TV_CMD_IO1 + nr;
    buff[1] = value;
    buff[2] = 0;
    if(twi_writeBytes(freeNoveInst->twiInst, freeNoveInst->devAddr, buff, 3) < 1)
        return false;
    return true;
}

bool freeNove_ThreeWeled_sonic(freeNove_ThreeWeled_t *freeNoveInst, unsigned short value) {
    return false;
}
