/*
 * MMC/SD driver file for arduFPGA designs.
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


#ifndef MMC_SD_SPI_H_
#define MMC_SD_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "driver/spi.h"
#include "fat_fs/inc/ff.h"


/*
 * Common
 */
/*#####################################################*/
/* MMC/SD command */
#define CMD0    (0)         /* GO_IDLE_STATE */
#define CMD1    (1)         /* SEND_OP_COND (MMC) */
#define CMD2    (2)         /* SEND_CID */
#define ACMD41  (0x80+41)   /* SEND_OP_COND (SDC) */
#define CMD8    (8)         /* SEND_IF_COND */
#define CMD9    (9)         /* SEND_CSD */
#define CMD10   (10)        /* SEND_CID */
#define CMD12   (12)        /* STOP_TRANSMISSION */
#define ACMD13  (0x80+13)   /* SD_STATUS (SDC) */
#define CMD16   (16)        /* SET_BLOCKLEN */
#define CMD17   (17)        /* READ_SINGLE_BLOCK */
#define CMD18   (18)        /* READ_MULTIPLE_BLOCK */
#define CMD23   (23)        /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80+23)   /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (24)        /* WRITE_BLOCK */
#define CMD25   (25)        /* WRITE_MULTIPLE_BLOCK */
#define CMD32   (32)        /* ERASE_ER_BLK_START */
#define CMD33   (33)        /* ERASE_ER_BLK_END */
#define CMD38   (38)        /* ERASE */
#define CMD55   (55)        /* APP_CMD */
#define CMD58   (58)        /* READ_OCR */
#define CMD59   (59)        /* CRC_ON_OFF */

/*
 * EXT_CSD switch cmd macros
 */

#define EXT_CSD_FLUSH_CACHE             32      /* W */
#define EXT_CSD_CACHE_CTRL              33      /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION  34      /* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX    35      /* RO */
#define EXT_CSD_PACKED_CMD_STATUS       36      /* RO */
#define EXT_CSD_EXP_EVENTS_STATUS       54      /* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL         56      /* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE        61      /* R */
#define EXT_CSD_GP_SIZE_MULT            143     /* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE     156     /* R/W */
#define EXT_CSD_PARTITION_SUPPORT       160     /* RO */
#define EXT_CSD_HPI_MGMT                161     /* R/W */
#define EXT_CSD_RST_N_FUNCTION          162     /* R/W */
#define EXT_CSD_BKOPS_EN                163     /* R/W */
#define EXT_CSD_BKOPS_START             164     /* W */
#define EXT_CSD_SANITIZE_START          165     /* W */
#define EXT_CSD_WR_REL_PARAM            166     /* RO */
#define EXT_CSD_RPMB_MULT               168     /* RO */
#define EXT_CSD_BOOT_WP                 173     /* R/W */
#define EXT_CSD_ERASE_GROUP_DEF         175     /* R/W */
#define EXT_CSD_PART_CONFIG             179     /* R/W */
#define EXT_CSD_ERASED_MEM_CONT         181     /* RO */
#define EXT_CSD_BUS_WIDTH               183     /* R/W */
#define EXT_CSD_HS_TIMING               185     /* R/W */
#define EXT_CSD_POWER_CLASS             187     /* R/W */
#define EXT_CSD_REV                     192     /* RO */
#define EXT_CSD_STRUCTURE               194     /* RO */
#define EXT_CSD_CARD_TYPE               196     /* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME   198     /* RO */
#define EXT_CSD_PART_SWITCH_TIME        199     /* RO */
#define EXT_CSD_PWR_CL_52_195           200     /* RO */
#define EXT_CSD_PWR_CL_26_195           201     /* RO */
#define EXT_CSD_PWR_CL_52_360           202     /* RO */
#define EXT_CSD_PWR_CL_26_360           203     /* RO */
#define EXT_CSD_SEC_CNT                 212     /* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT             217     /* RO */
#define EXT_CSD_REL_WR_SEC_C            222     /* RO */
#define EXT_CSD_HC_WP_GRP_SIZE          221     /* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT      223     /* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE       224     /* RO */
#define EXT_CSD_BOOT_MULT               226     /* RO */
#define EXT_CSD_SEC_TRIM_MULT           229     /* RO */
#define EXT_CSD_SEC_ERASE_MULT          230     /* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT     231     /* RO */
#define EXT_CSD_TRIM_MULT               232     /* RO */
#define EXT_CSD_PWR_CL_200_195          236     /* RO */
#define EXT_CSD_PWR_CL_200_360          237     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_195       238     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_360       239     /* RO */
#define EXT_CSD_BKOPS_STATUS            246     /* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME     247     /* RO */
#define EXT_CSD_GENERIC_CMD6_TIME       248     /* RO */
#define EXT_CSD_CACHE_SIZE              249     /* RO, 4 bytes */
#define EXT_CSD_TAG_UNIT_SIZE           498     /* RO */
#define EXT_CSD_DATA_TAG_SUPPORT        499     /* RO */
#define EXT_CSD_MAX_PACKED_WRITES       500     /* RO */
#define EXT_CSD_MAX_PACKED_READS        501     /* RO */
#define EXT_CSD_BKOPS_SUPPORT           502     /* RO */
#define EXT_CSD_HPI_FEATURES            503     /* RO */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_WR_REL_PARAM_EN         (1<<2)

#define EXT_CSD_BOOT_WP_B_PWR_WP_DIS    (0x40)
#define EXT_CSD_BOOT_WP_B_PERM_WP_DIS   (0x10)
#define EXT_CSD_BOOT_WP_B_PERM_WP_EN    (0x04)
#define EXT_CSD_BOOT_WP_B_PWR_WP_EN     (0x01)

#define EXT_CSD_PART_CONFIG_ACC_MASK    (0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT0   (0x1)
#define EXT_CSD_PART_CONFIG_ACC_RPMB    (0x3)
#define EXT_CSD_PART_CONFIG_ACC_GP0     (0x4)

#define EXT_CSD_PART_SUPPORT_PART_EN    (0x1)

#define EXT_CSD_CMD_SET_NORMAL          (1<<0)
#define EXT_CSD_CMD_SET_SECURE          (1<<1)
#define EXT_CSD_CMD_SET_CPSECURE        (1<<2)

#define EXT_CSD_CARD_TYPE_26    (1<<0)  /* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52    (1<<1)  /* Card can run at 52MHz */
#define EXT_CSD_CARD_TYPE_MASK  0x3F    /* Mask out reserved bits */
#define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)   /* Card can run at 52MHz */
                                              /* DDR mode @1.8V or 3V I/O */
#define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)   /* Card can run at 52MHz */
                                              /* DDR mode @1.2V I/O */
#define EXT_CSD_CARD_TYPE_DDR_52       (EXT_CSD_CARD_TYPE_DDR_1_8V  \
                                         | EXT_CSD_CARD_TYPE_DDR_1_2V)
#define EXT_CSD_CARD_TYPE_SDR_1_8V      (1<<4)  /* Card can run at 200MHz */
#define EXT_CSD_CARD_TYPE_SDR_1_2V      (1<<5)  /* Card can run at 200MHz */
                                                 /* SDR mode @1.2V I/O */

#define EXT_CSD_BUS_WIDTH_1     0       /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4     1       /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8     2       /* Card is in 8 bit mode */
#define EXT_CSD_DDR_BUS_WIDTH_4 5       /* Card is in 4 bit DDR mode */
#define EXT_CSD_DDR_BUS_WIDTH_8 6       /* Card is in 8 bit DDR mode */

#define EXT_CSD_SEC_ER_EN       BIT(0)
#define EXT_CSD_SEC_BD_BLK_EN   BIT(2)
#define EXT_CSD_SEC_GB_CL_EN    BIT(4)
#define EXT_CSD_SEC_SANITIZE    BIT(6)  /* v4.5 only */

#define EXT_CSD_RST_N_EN_MASK   0x3
#define EXT_CSD_RST_N_ENABLED   1       /* RST_n is enabled on card */

#define EXT_CSD_NO_POWER_NOTIFICATION   0
#define EXT_CSD_POWER_ON                1
#define EXT_CSD_POWER_OFF_SHORT         2
#define EXT_CSD_POWER_OFF_LONG          3

#define EXT_CSD_PWR_CL_8BIT_MASK        0xF0    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_4BIT_MASK        0x0F    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_8BIT_SHIFT       4
#define EXT_CSD_PWR_CL_4BIT_SHIFT       0

#define EXT_CSD_PACKED_EVENT_EN BIT(3)

/*
 * EXCEPTION_EVENT_STATUS field
 */
#define EXT_CSD_URGENT_BKOPS            BIT(0)
#define EXT_CSD_DYNCAP_NEEDED           BIT(1)
#define EXT_CSD_SYSPOOL_EXHAUSTED       BIT(2)
#define EXT_CSD_PACKED_FAILURE          BIT(3)

#define EXT_CSD_PACKED_GENERIC_ERROR    BIT(0)
#define EXT_CSD_PACKED_INDEXED_ERROR    BIT(1)

/* Card type flags (CardType) */
#define CT_MMC      0x01        /* MMC ver 3 */
#define CT_SD1      0x02        /* SD ver 1 */
#define CT_SD2      0x04        /* SD ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK    0x08        /* Block addressing */

/*
 * MMC_SWITCH access modes
 *
 * The SWITCH command response is of type R1b, therefore, the host should read the card status, using
 * SEND_STATUS command, after the busy signal is de-asserted, to check the result of the SWITCH
 * operation.
 */
#define MMC_SWITCH_MODE_CMD_SET         0x00    /* The command set is changed according to the Cmd Set field of the argument */
#define MMC_SWITCH_MODE_SET_BITS        0x01    /* The bits in the pointed byte are set, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_CLEAR_BITS      0x02    /* The bits in the pointed byte are cleared, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_WRITE_BYTE      0x03    /* The Value field is written into the pointed byte. */


/*#####################################################*/












/*
 * EXT_CSD struct
 */

/*#####################################################*/
#define _Disk_ErrInit                   1
#define _Disk_ErrReset                  2
#define _Disk_InitOk                    3
#define _Disk_Busy                      4
#define _Disk_DelayWrite                5
#define _Disk_DelayRead                 6
#define _Disk_ReleaseError              7
#define _Disk_ReadWriteOk               8
#define _Disk_ReadInitError             9
#define _Disk_ErrVoltageRange           10
/*#####################################################*/
#define MMC_CMD_RESET                   0
#define MMC_CMD_START_INITIALIZATION    1
#define MMC_SEND_IF_COND                8
#define MMC_CMD_SEND_CSD                9
#define MMC_CMD_SEND_CID                10
#define MMC_CMD_STOP_TRANSMISSION       12
#define MMC_CMD_SEND_STATUS             13
#define MMC_CMD_SET_BLOCK_SIZE          16
#define MMC_CMD_READ_SINGLE_BLOCK       17
#define MMC_CMD_READ_MULTIPLE_BLOCKS    18
#define MMC_CMD_WRITE_SINGLE_BLOCK      24
#define MMC_CMD_WRITE_MULTIPLE_BLOCKS   25
#define MMC_CMD_PROGRAM_CSD             27
#define MMC_CMD_SET_WRITE_PROT          28
#define MMC_CMD_CLR_WRITE_PROT          29
#define MMC_CMD_SEND_WRITE_PROT         30
#define MMC_CMD_TAG_SECTOR_START        32
#define MMC_CMD_TAG_SECTOR_END          33
#define MMC_CMD_UNTAG_SECTOR            34
#define MMC_CMD_TAG_ERASE_GROUP_START   35
#define MMC_CMD_TAG_ERASE_GROUP_END     36
#define MMC_CMD_UNTAG_ERASE_GROUP       37
#define MMC_CMD_ERASE                   38
#define MMC_CMD_SD_SEND_OP_COND         41
#define MMC_CMD_LOCK_UNLOCK             42
#define MMC_CMD_APP_CMD                 55
#define MMC_CMD_READ_OCR                58
#define MMC_CMD_CRC_ON_OFF              59
/*#####################################################*/
// SPI Response Flags
#define MMC_IN_IDLE_STATE               0x01
#define MMC_ERASE_RESET                 0x02
#define MMC_ILLEGAL_COMMAND             0x04
#define MMC_COM_CRC_ERROR               0x08
#define MMC_ERASE_ERROR                 0x10
#define MMC_ADRESS_ERROR                0x20
#define MMC_PARAMETER_ERROR             0x40
/*#####################################################*/
#define SD_RAW_SPEC_1                   0
#define SD_RAW_SPEC_2                   1
#define SD_RAW_SPEC_SDHC                2
/*#####################################################*/
#define MMC_DATA_TOKEN                  0xFE
/*#####################################################*/
#define MMC_RESPONSE_OK                 0x05
#define MMC_RESPONSE_CRC_ERROR          0x0B
#define MMC_RESPONSE_WRITE_ERROR        0x0D
#define MMC_RESPONSE_MASK               0x1F
/*#####################################################*/
#define DelayWriteCommand               65536
#define DelayReadCommand                1024
#define SdCard_NumberToTryResetCmd      100
#define SdCard_NumberToTryIfCondCmd     254
#define SdCard_NumberToTryInitCmdSd     30
#define SdCard_NumberToTryInitCmdSdHc   254
#define SdDriverDelaysConstant          20
#define NumberOfIdleBytes               65536
/*#####################################################*/
#define SD_CMD(x)   (x)

/* Command/Response flags for notifying some information to controller */
#define SD_CMDRSP_NONE          BIT(0)
#define SD_CMDRSP_STOP          BIT(1)
#define SD_CMDRSP_FS            BIT(2)
#define SD_CMDRSP_ABORT         BIT(3)
#define SD_CMDRSP_BUSY          BIT(4)
#define SD_CMDRSP_136BITS       BIT(5)
#define SD_CMDRSP_DATA          BIT(6)
#define SD_CMDRSP_READ          BIT(7)
#define SD_CMDRSP_WRITE         BIT(8)


#define SD_CMDRSP_R1            (0)
#define SD_CMDRSP_R1b           (0 | SD_CMDRSP_BUSY)
#define SD_CMDRSP_R2            (SD_CMDRSP_136BITS)
#define SD_CMDRSP_R3            (SD_CMDRSP_R1)



/* SD voltage enumeration as per VHS field of the interface command */
#define SD_VOLT_2P7_3P6                 (0x000100u)

/* SD OCR register definitions */
/* High capacity */
#define SD_OCR_HIGH_CAPACITY        BIT(30)
/* Voltage */
#define SD_OCR_VDD_2P7_2P8      BIT(15)
#define SD_OCR_VDD_2P8_2P9      BIT(16)
#define SD_OCR_VDD_2P9_3P0      BIT(17)
#define SD_OCR_VDD_3P0_3P1      BIT(18)
#define SD_OCR_VDD_3P1_3P2      BIT(19)
#define SD_OCR_VDD_3P2_3P3      BIT(20)
#define SD_OCR_VDD_3P3_3P4      BIT(21)
#define SD_OCR_VDD_3P4_3P5      BIT(22)
#define SD_OCR_VDD_3P5_3P6      BIT(23)
/* This is for convenience only. Sets all the VDD fields */
#define SD_OCR_VDD_WILDCARD     (0x1FF << 15)

/* SD CSD register definitions */
#define SD_TRANSPEED_25MBPS     (0x32u)
#define SD_TRANSPEED_50MBPS     (0x5Au)

#define SD_CARD_CSD_VERSION(crd) (((crd)->raw_csd[3] & 0xC0000000) >> 30)

#define SD_CSD0_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x000003FF) << 2) | ((csd1 & 0xC0000000) >> 30))
#define SD_CSD0_MULT(csd3, csd2, csd1, csd0) ((csd1 & 0x00038000) >> 15)
#define SD_CSD0_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD0_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD0_DEV_SIZE(crd) SD_CSD0_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_MULT(crd) SD_CSD0_MULT((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_RDBLKLEN(crd) SD_CSD0_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_TRANSPEED(crd) SD_CSD0_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_NUMBLK(crd) ((SD_CARD0_DEV_SIZE((crd)) + 1) * (1 << (SD_CARD0_MULT((crd)) + 2)))
#define SD_CARD0_SIZE(crd) ((SD_CARD0_NUMBLK((crd))) * (1 << (SD_CARD0_RDBLKLEN(crd))))

#define SD_CSD1_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x0000003F) << 16) | ((csd1 & 0xFFFF0000) >> 16))
#define SD_CSD1_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD1_WRBLKLEN(csd3, csd2, csd1, csd0) ((csd0 & 0x03C00000) >> 22)
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD1_DEV_SIZE(crd) SD_CSD1_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd) SD_CSD1_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_WRBLKLEN(crd) SD_CSD1_WRBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_TRANSPEED(crd) SD_CSD1_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_SIZE(crd) ((unsigned long long)(SD_CARD1_DEV_SIZE((crd)) + 1) * (unsigned long long)(524288))


/* Check RCA/status */
#define SD_RCA_ADDR(rca)             ((rca & 0xFFFF0000) >> 16)
#define SD_RCA_STAT(rca)             (rca & 0x0xFFFF)

/* Check pattern that can be used for card response validation */
#define SD_CHECK_PATTERN   0xAA

/* SD SCR related macros */
#define SD_VERSION_1P0      0
#define SD_VERSION_1P1      1
#define SD_VERSION_2P0      2
#define SD_BUS_WIDTH_1BIT   1
#define SD_BUS_WIDTH_4BIT   4
#define SD_BUS_WIDTH_8BIT   8

/* Helper macros */
/* Note card registers are big endian */
#define SD_CARD_VERSION(sdcard)     ((sdcard)->raw_scr[0] & 0xF)
#define SD_CARD_BUSWIDTH(sdcard)    (((sdcard)->raw_scr[0] & 0xF00) >> 8)
#define GET_SD_CARD_BUSWIDTH(sdcard)  ((((sdcard.busWidth) & 0x0F) == 0x01) ? \
                                      0x1 : ((((sdcard).busWidth & 0x04) == \
                                      0x04) ? 0x04 : 0xFF))
#define GET_SD_CARD_FRE(sdcard)       (((sdcard.tranSpeed) == 0x5A) ? 50 : \
                                      (((sdcard.tranSpeed) == 0x32) ? 25 : 0))

/* Cacheline size */
#ifndef SOC_CACHELINE_SIZE
#define SOC_CACHELINE_SIZE         128
#endif

/* CM6 Swith mode arguments for High Speed */
#define SD_SWITCH_MODE        0x80FFFFFF
#define SD_CMD6_GRP1_SEL      0xFFFFFFF0
#define SD_CMD6_GRP1_HS       0x1
/*#####################################################*/

typedef enum
{
	IsNoCard = 0, IsSd, IsSdhc,
} mmcsd_type_e;

typedef struct  
{
	unsigned long blkLen;
	unsigned long nBlks;
	unsigned long long size;
	unsigned char tranSpeed;
	bool SD_Init_OK;
	bool connected;
	unsigned int initFlg;
	mmcsd_type_e sdType;
	unsigned long raw_csd[4];
	unsigned long raw_cid[4];
	bool fs_mounted;
	spi_t *spi_inst;
	unsigned int unitNr;
#ifdef USE_REDUCED_FAT_FS
	FAT *fatFs;
#else /* !USE_REDUCED_FAT_FS */
	FATFS *fatFs;
#endif
}mmc_sd_t;

void mmc_sd_spi_idle(mmc_sd_t *inst);

#endif /* MMC_SD_SPI_H_ */