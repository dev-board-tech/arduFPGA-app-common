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

#ifndef TWI_S_H_
#define TWI_S_H_

#include <stdint.h>

#include <avr/io.h>
#include <stdbool.h>

#define TW_SLA_W(ADDR)		((ADDR << 1) | TW_WRITE)
#define TW_SLA_R(ADDR)		((ADDR << 1) | TW_READ)
#define TW_READ_ACK			1
#define TW_READ_NACK		0

/*
--------------------------------------------------------------------------
TWI - Two wire interface
--------------------------------------------------------------------------
*/
/* Two-Wire Interface */
typedef struct FPGA_TWI_MASTER_struct
{
	volatile uint8_t CTRLA;  /* Control Register A */
	volatile uint8_t CTRLB;  /* Control Register B */
	volatile uint8_t CTRLC;  /* Control Register C */
	volatile uint8_t STATUS;  /* Status Register */
	volatile uint8_t BAUD;  /* Baurd Rate Control Register */
	volatile uint8_t ADDR;  /* Address Register */
	volatile uint8_t DATA;  /* Data Register */
} FPGA_TWI_MASTER_t;


/*  */
typedef struct FPGA_TWI_SLAVE_struct
{
	volatile uint8_t CTRLA;  /* Control Register A */
	volatile uint8_t CTRLB;  /* Control Register B */
	volatile uint8_t STATUS;  /* Status Register */
	volatile uint8_t ADDR;  /* Address Register */
	volatile uint8_t DATA;  /* Data Register */
	volatile uint8_t ADDRMASK;  /* Address Mask Register */
} FPGA_TWI_SLAVE_t;


/*  */
typedef struct FPGA_TWI_TIMEOUT_struct
{
	volatile uint8_t TOS;  /* Timeout Status Register */
	volatile uint8_t TOCONF;  /* Timeout Configuration Register */
} FPGA_TWI_TIMEOUT_t;

/*  */
typedef struct FPGA_TWI_struct
{
	volatile uint8_t CTRL;  /* TWI Common Control Register */
	FPGA_TWI_MASTER_t MASTER;  /* TWI master module */
	FPGA_TWI_SLAVE_t SLAVE;  /* TWI slave module */
	FPGA_TWI_TIMEOUT_t TIMEOUT;  /* TWI SMBUS timeout module */
} FPGA_TWI_t;


/* SDA Hold Time */
typedef enum FPGA_TWI_SDAHOLD_enum
{
	FPGA_TWI_SDAHOLD_OFF_gc = (0x00<<4),  /* SDA Hold Time off */
	FPGA_TWI_SDAHOLD_50NS_gc = (0x01<<4),  /* SDA Hold Time 50 ns */
	FPGA_TWI_SDAHOLD_300NS_gc = (0x02<<4),  /* SDA Hold Time 300 ns */
	FPGA_TWI_SDAHOLD_400NS_gc = (0x03<<4),  /* SDA Hold Time 400 ns */
} FPGA_TWI_SDAHOLD_t;

/* Master Interrupt Level */
typedef enum FPGA_TWI_MASTER_INTLVL_enum
{
	FPGA_TWI_MASTER_INTLVL_OFF_gc = (0x00<<6),  /* Interrupt Disabled */
	FPGA_TWI_MASTER_INTLVL_LO_gc = (0x01<<6),  /* Low Level */
	FPGA_TWI_MASTER_INTLVL_MED_gc = (0x02<<6),  /* Medium Level */
	FPGA_TWI_MASTER_INTLVL_HI_gc = (0x03<<6),  /* High Level */
} FPGA_TWI_MASTER_INTLVL_t;

/* Inactive Timeout */
typedef enum FPGA_TWI_MASTER_TIMEOUT_enum
{
	FPGA_TWI_MASTER_TIMEOUT_DISABLED_gc = (0x00<<2),  /* Bus Timeout Disabled */
	FPGA_TWI_MASTER_TIMEOUT_50US_gc = (0x01<<2),  /* 50 Microseconds */
	FPGA_TWI_MASTER_TIMEOUT_100US_gc = (0x02<<2),  /* 100 Microseconds */
	FPGA_TWI_MASTER_TIMEOUT_200US_gc = (0x03<<2),  /* 200 Microseconds */
} FPGA_TWI_MASTER_TIMEOUT_t;

/* Master Command */
typedef enum FPGA_TWI_MASTER_CMD_enum
{
	FPGA_TWI_MASTER_CMD_NOACT_gc = (0x00<<0),  /* No Action */
	FPGA_TWI_MASTER_CMD_REPSTART_gc = (0x01<<0),  /* Issue Repeated Start Condition */
	FPGA_TWI_MASTER_CMD_RECVTRANS_gc = (0x02<<0),  /* Receive or Transmit Data */
	FPGA_TWI_MASTER_CMD_STOP_gc = (0x03<<0),  /* Issue Stop Condition */
} FPGA_TWI_MASTER_CMD_t;

/* Master Bus State */
typedef enum FPGA_TWI_MASTER_BUSSTATE_enum
{
	FPGA_TWI_MASTER_BUSSTATE_UNKNOWN_gc = (0x00<<0),  /* Unknown Bus State */
	FPGA_TWI_MASTER_BUSSTATE_IDLE_gc = (0x01<<0),  /* Bus is Idle */
	FPGA_TWI_MASTER_BUSSTATE_OWNER_gc = (0x02<<0),  /* This Module Controls The Bus */
	FPGA_TWI_MASTER_BUSSTATE_BUSY_gc = (0x03<<0),  /* The Bus is Busy */
} FPGA_TWI_MASTER_BUSSTATE_t;

/* Slave Interrupt Level */
typedef enum FPGA_TWI_SLAVE_INTLVL_enum
{
	FPGA_TWI_SLAVE_INTLVL_OFF_gc = (0x00<<6),  /* Interrupt Disabled */
	FPGA_TWI_SLAVE_INTLVL_LO_gc = (0x01<<6),  /* Low Level */
	FPGA_TWI_SLAVE_INTLVL_MED_gc = (0x02<<6),  /* Medium Level */
	FPGA_TWI_SLAVE_INTLVL_HI_gc = (0x03<<6),  /* High Level */
} FPGA_TWI_SLAVE_INTLVL_t;

/* Slave Command */
typedef enum FPGA_TWI_SLAVE_CMD_enum
{
	FPGA_TWI_SLAVE_CMD_NOACT_gc = (0x00<<0),  /* No Action */
	FPGA_TWI_SLAVE_CMD_COMPTRANS_gc = (0x02<<0),  /* Used To Complete a Transaction */
	FPGA_TWI_SLAVE_CMD_RESPONSE_gc = (0x03<<0),  /* Used in Response to Address/Data Interrupt */
} FPGA_TWI_SLAVE_CMD_t;

/* Master Timeout */
typedef enum FPGA_TWI_MASTER_TTIMEOUT_enum
{
	FPGA_TWI_MASTER_TTIMEOUT_25MS_gc = (0x00<<0),  /* 25 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_24MS_gc = (0x01<<0),  /* 24 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_23MS_gc = (0x02<<0),  /* 23 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_22MS_gc = (0x03<<0),  /* 22 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_26MS_gc = (0x04<<0),  /* 26 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_27MS_gc = (0x05<<0),  /* 27 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_28MS_gc = (0x06<<0),  /* 28 Milliseconds */
	FPGA_TWI_MASTER_TTIMEOUT_29MS_gc = (0x07<<0),  /* 29 Milliseconds */
} FPGA_TWI_MASTER_TTIMEOUT_t;

/* Slave Ttimeout */
typedef enum FPGA_TWI_SLAVE_TTIMEOUT_enum
{
	FPGA_TWI_SLAVE_TTIMEOUT_25MS_gc = (0x00<<5),  /* 25 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_24MS_gc = (0x01<<5),  /* 24 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_23MS_gc = (0x02<<5),  /* 23 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_22MS_gc = (0x03<<5),  /* 22 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_26MS_gc = (0x04<<5),  /* 26 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_27MS_gc = (0x05<<5),  /* 27 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_28MS_gc = (0x06<<5),  /* 28 Milliseconds */
	FPGA_TWI_SLAVE_TTIMEOUT_29MS_gc = (0x07<<5),  /* 29 Milliseconds */
} FPGA_TWI_SLAVE_TTIMEOUT_t;

/* Master/Slave Extend Timeout */
typedef enum FPGA_TWI_MASTER_TMSEXT_enum
{
	FPGA_TWI_MASTER_TMSEXT_10MS25MS_gc = (0x00<<3),  /* Tmext 10ms Tsext 25ms */
	FPGA_TWI_MASTER_TMSEXT_9MS24MS_gc = (0x01<<3),  /* Tmext 9ms  Tsext 24ms */
	FPGA_TWI_MASTER_TMSEXT_11MS26MS_gc = (0x02<<3),  /* Tmext 11ms Tsext 26ms */
	FPGA_TWI_MASTER_TMSEXT_12MS27MS_gc = (0x03<<3),  /* Tmext 12ms Tsext 27ms */
} FPGA_TWI_MASTER_TMSEXT_t;

/* TWI - Two-Wire Interface */
/* TWI.CTRL  bit masks and bit positions */
#define FPGA_TWI_BRIDGEEN_bm  0x80  /* Bridge Enable bit mask. */
#define FPGA_TWI_BRIDGEEN_bp  7  /* Bridge Enable bit position. */

#define FPGA_TWI_SFMPEN_bm  0x40  /* Slave Fast Mode Plus Enable bit mask. */
#define FPGA_TWI_SFMPEN_bp  6  /* Slave Fast Mode Plus Enable bit position. */

#define FPGA_TWI_SSDAHOLD_gm  0x30  /* Slave SDA Hold Time Enable group mask. */
#define FPGA_TWI_SSDAHOLD_gp  4  /* Slave SDA Hold Time Enable group position. */
#define FPGA_TWI_SSDAHOLD0_bm  (1<<4)  /* Slave SDA Hold Time Enable bit 0 mask. */
#define FPGA_TWI_SSDAHOLD0_bp  4  /* Slave SDA Hold Time Enable bit 0 position. */
#define FPGA_TWI_SSDAHOLD1_bm  (1<<5)  /* Slave SDA Hold Time Enable bit 1 mask. */
#define FPGA_TWI_SSDAHOLD1_bp  5  /* Slave SDA Hold Time Enable bit 1 position. */

#define FPGA_TWI_FMPEN_bm  0x08  /* FMPLUS Enable bit mask. */
#define FPGA_TWI_FMPEN_bp  3  /* FMPLUS Enable bit position. */

#define FPGA_TWI_SDAHOLD_gm  0x06  /* SDA Hold Time Enable group mask. */
#define FPGA_TWI_SDAHOLD_gp  1  /* SDA Hold Time Enable group position. */
#define FPGA_TWI_SDAHOLD0_bm  (1<<1)  /* SDA Hold Time Enable bit 0 mask. */
#define FPGA_TWI_SDAHOLD0_bp  1  /* SDA Hold Time Enable bit 0 position. */
#define FPGA_TWI_SDAHOLD1_bm  (1<<2)  /* SDA Hold Time Enable bit 1 mask. */
#define FPGA_TWI_SDAHOLD1_bp  2  /* SDA Hold Time Enable bit 1 position. */

#define FPGA_TWI_EDIEN_bm  0x01  /* External Driver Interface Enable bit mask. */
#define FPGA_TWI_EDIEN_bp  0  /* External Driver Interface Enable bit position. */

/* TWI_MASTER.CTRLA  bit masks and bit positions */
#define FPGA_TWI_MASTER_INTLVL_gm  0xC0  /* Interrupt Level group mask. */
#define FPGA_TWI_MASTER_INTLVL_gp  6  /* Interrupt Level group position. */
#define FPGA_TWI_MASTER_INTLVL0_bm  (1<<6)  /* Interrupt Level bit 0 mask. */
#define FPGA_TWI_MASTER_INTLVL0_bp  6  /* Interrupt Level bit 0 position. */
#define FPGA_TWI_MASTER_INTLVL1_bm  (1<<7)  /* Interrupt Level bit 1 mask. */
#define FPGA_TWI_MASTER_INTLVL1_bp  7  /* Interrupt Level bit 1 position. */

#define FPGA_TWI_MASTER_RIEN_bm  0x20  /* Read Interrupt Enable bit mask. */
#define FPGA_TWI_MASTER_RIEN_bp  5  /* Read Interrupt Enable bit position. */

#define FPGA_TWI_MASTER_WIEN_bm  0x10  /* Write Interrupt Enable bit mask. */
#define FPGA_TWI_MASTER_WIEN_bp  4  /* Write Interrupt Enable bit position. */

#define FPGA_TWI_MASTER_ENABLE_bm  0x08  /* Enable TWI Master bit mask. */
#define FPGA_TWI_MASTER_ENABLE_bp  3  /* Enable TWI Master bit position. */

/* TWI_MASTER.CTRLB  bit masks and bit positions */
#define FPGA_TWI_MASTER_TIMEOUT_gm  0x0C  /* Inactive Bus Timeout group mask. */
#define FPGA_TWI_MASTER_TIMEOUT_gp  2  /* Inactive Bus Timeout group position. */
#define FPGA_TWI_MASTER_TIMEOUT0_bm  (1<<2)  /* Inactive Bus Timeout bit 0 mask. */
#define FPGA_TWI_MASTER_TIMEOUT0_bp  2  /* Inactive Bus Timeout bit 0 position. */
#define FPGA_TWI_MASTER_TIMEOUT1_bm  (1<<3)  /* Inactive Bus Timeout bit 1 mask. */
#define FPGA_TWI_MASTER_TIMEOUT1_bp  3  /* Inactive Bus Timeout bit 1 position. */

#define FPGA_TWI_MASTER_QCEN_bm  0x02  /* Quick Command Enable bit mask. */
#define FPGA_TWI_MASTER_QCEN_bp  1  /* Quick Command Enable bit position. */

#define FPGA_TWI_MASTER_SMEN_bm  0x01  /* Smart Mode Enable bit mask. */
#define FPGA_TWI_MASTER_SMEN_bp  0  /* Smart Mode Enable bit position. */

#define FPGA_TWI_MASTER_TTOUTEN_bm  0x10  /* Ttimeout Enable bit mask. */
#define FPGA_TWI_MASTER_TTOUTEN_bp  4  /* Ttimeout Enable bit position. */

#define FPGA_TWI_MASTER_TSEXTEN_bm  0x20  /* Slave Extend Timeout Enable bit mask. */
#define FPGA_TWI_MASTER_TSEXTEN_bp  5  /* Slave Extend Timeout Enable bit position. */

#define FPGA_TWI_MASTER_TMEXTEN_bm  0x40  /* Master Extend Timeout Enable bit mask. */
#define FPGA_TWI_MASTER_TMEXTEN_bp  6  /* Master Extend Timeout Enable bit position. */

#define FPGA_TWI_MASTER_TOIE_bm  0x80  /* Timeout Interrupt Enable bit mask. */
#define FPGA_TWI_MASTER_TOIE_bp  7  /* Timeout Interrupt Enable bit position. */

/* TWI_MASTER.CTRLC  bit masks and bit positions */
#define FPGA_TWI_MASTER_ACKACT_bm  0x04  /* Acknowledge Action bit mask. */
#define FPGA_TWI_MASTER_ACKACT_bp  2  /* Acknowledge Action bit position. */

#define FPGA_TWI_MASTER_CMD_gm  0x03  /* Command group mask. */
#define FPGA_TWI_MASTER_CMD_gp  0  /* Command group position. */
#define FPGA_TWI_MASTER_CMD0_bm  (1<<0)  /* Command bit 0 mask. */
#define FPGA_TWI_MASTER_CMD0_bp  0  /* Command bit 0 position. */
#define FPGA_TWI_MASTER_CMD1_bm  (1<<1)  /* Command bit 1 mask. */
#define FPGA_TWI_MASTER_CMD1_bp  1  /* Command bit 1 position. */

/* TWI_MASTER.STATUS  bit masks and bit positions */
#define FPGA_TWI_MASTER_RIF_bm  0x80  /* Read Interrupt Flag bit mask. */
#define FPGA_TWI_MASTER_RIF_bp  7  /* Read Interrupt Flag bit position. */

#define FPGA_TWI_MASTER_WIF_bm  0x40  /* Write Interrupt Flag bit mask. */
#define FPGA_TWI_MASTER_WIF_bp  6  /* Write Interrupt Flag bit position. */

#define FPGA_TWI_MASTER_CLKHOLD_bm  0x20  /* Clock Hold bit mask. */
#define FPGA_TWI_MASTER_CLKHOLD_bp  5  /* Clock Hold bit position. */

#define FPGA_TWI_MASTER_RXACK_bm  0x10  /* Received Acknowledge bit mask. */
#define FPGA_TWI_MASTER_RXACK_bp  4  /* Received Acknowledge bit position. */

#define FPGA_TWI_MASTER_ARBLOST_bm  0x08  /* Arbitration Lost bit mask. */
#define FPGA_TWI_MASTER_ARBLOST_bp  3  /* Arbitration Lost bit position. */

#define FPGA_TWI_MASTER_BUSERR_bm  0x04  /* Bus Error bit mask. */
#define FPGA_TWI_MASTER_BUSERR_bp  2  /* Bus Error bit position. */

#define FPGA_TWI_MASTER_BUSSTATE_gm  0x03  /* Bus State group mask. */
#define FPGA_TWI_MASTER_BUSSTATE_gp  0  /* Bus State group position. */
#define FPGA_TWI_MASTER_BUSSTATE0_bm  (1<<0)  /* Bus State bit 0 mask. */
#define FPGA_TWI_MASTER_BUSSTATE0_bp  0  /* Bus State bit 0 position. */
#define FPGA_TWI_MASTER_BUSSTATE1_bm  (1<<1)  /* Bus State bit 1 mask. */
#define FPGA_TWI_MASTER_BUSSTATE1_bp  1  /* Bus State bit 1 position. */

/* TWI_SLAVE.CTRLA  bit masks and bit positions */
#define FPGA_TWI_SLAVE_INTLVL_gm  0xC0  /* Interrupt Level group mask. */
#define FPGA_TWI_SLAVE_INTLVL_gp  6  /* Interrupt Level group position. */
#define FPGA_TWI_SLAVE_INTLVL0_bm  (1<<6)  /* Interrupt Level bit 0 mask. */
#define FPGA_TWI_SLAVE_INTLVL0_bp  6  /* Interrupt Level bit 0 position. */
#define FPGA_TWI_SLAVE_INTLVL1_bm  (1<<7)  /* Interrupt Level bit 1 mask. */
#define FPGA_TWI_SLAVE_INTLVL1_bp  7  /* Interrupt Level bit 1 position. */

#define FPGA_TWI_SLAVE_DIEN_bm  0x20  /* Data Interrupt Enable bit mask. */
#define FPGA_TWI_SLAVE_DIEN_bp  5  /* Data Interrupt Enable bit position. */

#define FPGA_TWI_SLAVE_APIEN_bm  0x10  /* Address/Stop Interrupt Enable bit mask. */
#define FPGA_TWI_SLAVE_APIEN_bp  4  /* Address/Stop Interrupt Enable bit position. */

#define FPGA_TWI_SLAVE_ENABLE_bm  0x08  /* Enable TWI Slave bit mask. */
#define FPGA_TWI_SLAVE_ENABLE_bp  3  /* Enable TWI Slave bit position. */

#define FPGA_TWI_SLAVE_PIEN_bm  0x04  /* Stop Interrupt Enable bit mask. */
#define FPGA_TWI_SLAVE_PIEN_bp  2  /* Stop Interrupt Enable bit position. */

#define FPGA_TWI_SLAVE_PMEN_bm  0x02  /* Promiscuous Mode Enable bit mask. */
#define FPGA_TWI_SLAVE_PMEN_bp  1  /* Promiscuous Mode Enable bit position. */

#define FPGA_TWI_SLAVE_SMEN_bm  0x01  /* Smart Mode Enable bit mask. */
#define FPGA_TWI_SLAVE_SMEN_bp  0  /* Smart Mode Enable bit position. */

/* TWI_SLAVE.CTRLB  bit masks and bit positions */
#define FPGA_TWI_SLAVE_ACKACT_bm  0x04  /* Acknowledge Action bit mask. */
#define FPGA_TWI_SLAVE_ACKACT_bp  2  /* Acknowledge Action bit position. */

#define FPGA_TWI_SLAVE_CMD_gm  0x03  /* Command group mask. */
#define FPGA_TWI_SLAVE_CMD_gp  0  /* Command group position. */
#define FPGA_TWI_SLAVE_CMD0_bm  (1<<0)  /* Command bit 0 mask. */
#define FPGA_TWI_SLAVE_CMD0_bp  0  /* Command bit 0 position. */
#define FPGA_TWI_SLAVE_CMD1_bm  (1<<1)  /* Command bit 1 mask. */
#define FPGA_TWI_SLAVE_CMD1_bp  1  /* Command bit 1 position. */

#define FPGA_TWI_SLAVE_TTOUTEN_bm  0x10  /* Ttimeout Enable bit mask. */
#define FPGA_TWI_SLAVE_TTOUTEN_bp  4  /* Ttimeout Enable bit position. */

#define FPGA_TWI_SLAVE_TOIE_bm  0x80  /* Timeout Interrupt Enable bit mask. */
#define FPGA_TWI_SLAVE_TOIE_bp  7  /* Timeout Interrupt Enable bit position. */

/* TWI_SLAVE.STATUS  bit masks and bit positions */
#define FPGA_TWI_SLAVE_DIF_bm  0x80  /* Data Interrupt Flag bit mask. */
#define FPGA_TWI_SLAVE_DIF_bp  7  /* Data Interrupt Flag bit position. */

#define FPGA_TWI_SLAVE_APIF_bm  0x40  /* Address/Stop Interrupt Flag bit mask. */
#define FPGA_TWI_SLAVE_APIF_bp  6  /* Address/Stop Interrupt Flag bit position. */

#define FPGA_TWI_SLAVE_CLKHOLD_bm  0x20  /* Clock Hold bit mask. */
#define FPGA_TWI_SLAVE_CLKHOLD_bp  5  /* Clock Hold bit position. */

#define FPGA_TWI_SLAVE_RXACK_bm  0x10  /* Received Acknowledge bit mask. */
#define FPGA_TWI_SLAVE_RXACK_bp  4  /* Received Acknowledge bit position. */

#define FPGA_TWI_SLAVE_COLL_bm  0x08  /* Collision bit mask. */
#define FPGA_TWI_SLAVE_COLL_bp  3  /* Collision bit position. */

#define FPGA_TWI_SLAVE_BUSERR_bm  0x04  /* Bus Error bit mask. */
#define FPGA_TWI_SLAVE_BUSERR_bp  2  /* Bus Error bit position. */

#define FPGA_TWI_SLAVE_DIR_bm  0x02  /* Read/Write Direction bit mask. */
#define FPGA_TWI_SLAVE_DIR_bp  1  /* Read/Write Direction bit position. */

#define FPGA_TWI_SLAVE_AP_bm  0x01  /* Slave Address or Stop bit mask. */
#define FPGA_TWI_SLAVE_AP_bp  0  /* Slave Address or Stop bit position. */

/* TWI_SLAVE.ADDRMASK  bit masks and bit positions */
#define FPGA_TWI_SLAVE_ADDRMASK_gm  0xFE  /* Address Mask group mask. */
#define FPGA_TWI_SLAVE_ADDRMASK_gp  1  /* Address Mask group position. */
#define FPGA_TWI_SLAVE_ADDRMASK0_bm  (1<<1)  /* Address Mask bit 0 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK0_bp  1  /* Address Mask bit 0 position. */
#define FPGA_TWI_SLAVE_ADDRMASK1_bm  (1<<2)  /* Address Mask bit 1 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK1_bp  2  /* Address Mask bit 1 position. */
#define FPGA_TWI_SLAVE_ADDRMASK2_bm  (1<<3)  /* Address Mask bit 2 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK2_bp  3  /* Address Mask bit 2 position. */
#define FPGA_TWI_SLAVE_ADDRMASK3_bm  (1<<4)  /* Address Mask bit 3 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK3_bp  4  /* Address Mask bit 3 position. */
#define FPGA_TWI_SLAVE_ADDRMASK4_bm  (1<<5)  /* Address Mask bit 4 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK4_bp  5  /* Address Mask bit 4 position. */
#define FPGA_TWI_SLAVE_ADDRMASK5_bm  (1<<6)  /* Address Mask bit 5 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK5_bp  6  /* Address Mask bit 5 position. */
#define FPGA_TWI_SLAVE_ADDRMASK6_bm  (1<<7)  /* Address Mask bit 6 mask. */
#define FPGA_TWI_SLAVE_ADDRMASK6_bp  7  /* Address Mask bit 6 position. */

#define FPGA_TWI_SLAVE_ADDREN_bm  0x01  /* Address Enable bit mask. */
#define FPGA_TWI_SLAVE_ADDREN_bp  0  /* Address Enable bit position. */

/* TWI_TIMEOUT.TOS  bit masks and bit positions */
#define FPGA_TWI_TIMEOUT_TTOUTMIF_bm  0x01  /* Master Ttimeout Interrupt Flag bit mask. */
#define FPGA_TWI_TIMEOUT_TTOUTMIF_bp  0  /* Master Ttimeout Interrupt Flag bit position. */

#define FPGA_TWI_TIMEOUT_TSEXTIF_bm  0x02  /* Slave Extend Interrupt Flag bit mask. */
#define FPGA_TWI_TIMEOUT_TSEXTIF_bp  1  /* Slave Extend Interrupt Flag bit position. */

#define FPGA_TWI_TIMEOUT_TMEXTIF_bm  0x04  /* Master Extend Interrupt Flag bit mask. */
#define FPGA_TWI_TIMEOUT_TMEXTIF_bp  2  /* Master Extend Interrupt Flag bit position. */

#define FPGA_TWI_TIMEOUT_TTOUTSIF_bm  0x10  /* Slave Ttimeout Interrupt Flag bit mask. */
#define FPGA_TWI_TIMEOUT_TTOUTSIF_bp  4  /* Slave Ttimeout Interrupt Flag bit position. */

/* TWI_TIMEOUT.TOCONF  bit masks and bit positions */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL_gm  0x07  /* Master Ttimeout Select group mask. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL_gp  0  /* Master Ttimeout Select group position. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL0_bm  (1<<0)  /* Master Ttimeout Select bit 0 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL0_bp  0  /* Master Ttimeout Select bit 0 position. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL1_bm  (1<<1)  /* Master Ttimeout Select bit 1 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL1_bp  1  /* Master Ttimeout Select bit 1 position. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL2_bm  (1<<2)  /* Master Ttimeout Select bit 2 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTMSEL2_bp  2  /* Master Ttimeout Select bit 2 position. */

#define FPGA_TWI_TIMEOUT_TMSEXTSEL_gm  0x18  /* Master/Slave Timeout Select group mask. */
#define FPGA_TWI_TIMEOUT_TMSEXTSEL_gp  3  /* Master/Slave Timeout Select group position. */
#define FPGA_TWI_TIMEOUT_TMSEXTSEL0_bm  (1<<3)  /* Master/Slave Timeout Select bit 0 mask. */
#define FPGA_TWI_TIMEOUT_TMSEXTSEL0_bp  3  /* Master/Slave Timeout Select bit 0 position. */
#define FPGA_TWI_TIMEOUT_TMSEXTSEL1_bm  (1<<4)  /* Master/Slave Timeout Select bit 1 mask. */
#define FPGA_TWI_TIMEOUT_TMSEXTSEL1_bp  4  /* Master/Slave Timeout Select bit 1 position. */

#define FPGA_TWI_TIMEOUT_TTOUTSSEL_gm  0xE0  /* Slave Ttimeout Select group mask. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL_gp  5  /* Slave Ttimeout Select group position. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL0_bm  (1<<5)  /* Slave Ttimeout Select bit 0 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL0_bp  5  /* Slave Ttimeout Select bit 0 position. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL1_bm  (1<<6)  /* Slave Ttimeout Select bit 1 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL1_bp  6  /* Slave Ttimeout Select bit 1 position. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL2_bm  (1<<7)  /* Slave Ttimeout Select bit 2 mask. */
#define FPGA_TWI_TIMEOUT_TTOUTSSEL2_bp  7  /* Slave Ttimeout Select bit 2 position. */

typedef enum {
	twiErr_Ok = 0,
	twiErr_InvalidParam,
	twiErr_Nack
} twiErrCode_e;

typedef struct
{
	volatile uint8_t *ctrla;
	volatile uint8_t *ctrlb;
	volatile uint8_t *ctrlc;
	volatile uint8_t *status;
	volatile uint8_t *baud;
	volatile uint8_t *data;
}twi_t;

void twi_init(twi_t *inst, uint32_t twiFreq);
int8_t twi_writeBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len);
int8_t twi_readBytes(twi_t *inst, uint8_t slaveAddr, uint8_t* buff, uint8_t len);
int8_t twi_writeRead(twi_t *inst, uint8_t slaveAddr, uint8_t* buffSend, uint8_t lenSend, uint8_t* p_data_rx, uint16_t rx_len);


#endif /* TWI_S_H_ */