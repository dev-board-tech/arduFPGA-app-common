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

#ifndef __FREE_NOVE_THREE_WEELED__
#define __FREE_NOVE_THREE_WEELED__

#include <stdbool.h>
#include "driver/twi_s.h"

/*
CMD_SERVO1 Servo1 W 0-20000 
CMD_SERVO2 Servo2 W 0-20000 
CMD_SERVO3 Servo3 W 0-20000 
CMD_SERVO4 Servo4 W 0-20000 
CMD_PWM1 Motor1 Speed W 0-1000 
CMD_PWM2 Motor2 Speed W 0-1000 
CMD_DIR1 Motor1 Steering W 0 or non-0 
CMD_DIR2 Motor2 Steering W 0 or non-0 
CMD_BUZZER Buzzer W 0-65535 
CMD_IO1 IO1 W 0 or non-0 
CMD_IO2 IO2 W 0 or non-0 
CMD_IO3 IO3 W 0 or non-0 
CMD_SONIC TRIG/ECHO R
*/
typedef enum {
	freeNove_TV_CMD_SERVO1 		= 	0,
	freeNove_TV_CMD_SERVO2 		= 	1,
	freeNove_TV_CMD_SERVO3 		= 	2,
	freeNove_TV_CMD_SERVO4 		= 	3,
	freeNove_TV_CMD_PWM1		=	4,
	freeNove_TV_CMD_PWM2		=	5,
	freeNove_TV_CMD_DIR1		=	6,
	freeNove_TV_CMD_DIR2		=	7,
	freeNove_TV_CMD_BUZZER		=	8,
	freeNove_TV_CMD_IO1			=	9,
	freeNove_TV_CMD_IO2			=	10,
	freeNove_TV_CMD_IO3			=	11,
	freeNove_TV_CMD_SONIC		=	12,
	freeNove_TV_SERVO_MAX_PULSE_WIDTH = 2500,
	freeNove_TV_SERVO_MIN_PULSE_WIDTH = 500,
	freeNove_TV_SONIC_MAX_HIGH_BYTE = 50,
}freeNove_TV_CMD_e;

typedef struct freeNove_ThreeWeled_s {
    int err;
    twi_t *twiInst;
    unsigned char devAddr;
	uint16_t servoMin[4];
	uint16_t servoMax[4];
}freeNove_ThreeWeled_t;

freeNove_ThreeWeled_t *freeNove_ThreeWeled_init(twi_t *twiInst);
freeNove_ThreeWeled_t *freeNove_ThreeWeled_deinit(freeNove_ThreeWeled_t *freeNoveInst);

bool freeNove_ThreeWeled_servo(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, unsigned short value);
bool freeNove_ThreeWeled_pwm(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, unsigned short value);
bool freeNove_ThreeWeled_dir(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, bool value);
bool freeNove_ThreeWeled_buzzer(freeNove_ThreeWeled_t *freeNoveInst, unsigned short value);
bool freeNove_ThreeWeled_io(freeNove_ThreeWeled_t *freeNoveInst, unsigned char nr, bool value);
bool freeNove_ThreeWeled_sonic(freeNove_ThreeWeled_t *freeNoveInst, unsigned short value);

#endif
