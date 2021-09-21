/*
 * Copyright (c) 2020 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_ICM42688_ICM42688_REG_H_
#define ZEPHYR_DRIVERS_SENSOR_ICM42688_ICM42688_REG_H_

/* BANK 0 */
#define REG_DEVICE_CONFIG	0x11
#define REG_DRIVE_CONFIG	0x13
#define REG_INT_CONFIG		0x14
#define REG_FIFO_CONFIG		0x16
#define REG_TEMP_DATA1		0x1D
#define REG_TEMP_DATA0		0x1E
#define REG_ACCEL_DATA_X1	0x1F
#define REG_ACCEL_DATA_X0	0x20
#define REG_ACCEL_DATA_Y1	0x21
#define REG_ACCEL_DATA_Y0	0x22
#define REG_ACCEL_DATA_Z1	0x23
#define REG_ACCEL_DATA_Z0	0x24
#define REG_GYRO_DATA_X1	0x25
#define REG_GYRO_DATA_X0	0x26
#define REG_GYRO_DATA_Y1	0x27
#define REG_GYRO_DATA_Y0	0x28
#define REG_GYRO_DATA_Z1	0x29
#define REG_GYRO_DATA_Z0	0x2A
#define REG_TMST_FSYNCH		0x2B
#define REG_TMST_FSYNCL		0x2C
#define REG_INT_STATUS		0x2D
#define REG_FIFO_COUNTH		0x2E
#define REG_FIFO_COUNTL		0x2F
#define REG_FIFO_DATA		0x30
#define REG_APEX_DATA0		0x31
#define REG_APEX_DATA1		0x32
#define REG_APEX_DATA2		0x33
#define REG_APEX_DATA3		0x34
#define REG_APEX_DATA4		0x35
#define REG_APEX_DATA5		0x36
#define REG_INT_STATUS2		0x37
#define REG_INT_STATUS3		0x38
#define REG_SIGNAL_PATH_RESET	0x4B
#define REG_INTF_CONFIG0	0x4C
#define REG_INTF_CONFIG1	0x4D
#define REG_PWR_MGMT0		0x4E
#define REG_GYRO_CONFIG0	0x4F
#define REG_ACCEL_CONFIG0	0x50
#define REG_GYRO_CONFIG1	0x51
#define REG_GYRO_ACCEL_CONFIG0	0x52
#define REG_ACCEL_CONFIG1	0x53
#define REG_TMST_CONFIG		0x54
#define REG_APEX_CONFIG0	0x56
#define REG_SMD_CONFIG		0x57
#define REG_FIFO_CONFIG1	0x5F
#define REG_FIFO_CONFIG2	0x60
#define REG_FIFO_CONFIG4	0x61
#define REG_FIFO_FSYNC_CONFIG	0x62
#define REG_INT_CONFIG0		0x63
#define REG_INT_CONFIG1		0x64
#define REG_INT_SOURCE0		0x65
#define REG_INT_SOURCE1		0x66
#define REG_INT_SOURCE3		0x68
#define REG_INT_SOURCE4		0x69
#define REG_FIFO_LOST_PKT0	0x6C
#define REG_FIFO_LOST_PKT1	0x6D
#define REG_SELF_TEST_CONFIG	0x70
#define REG_WHO_AM_I		0x75
#define REG_BANK_SEL		0x76

/* BANK 1 */
#define REG_SENSOR_CONFIG0		0x03
#define REG_GYRO_CONFIG_STATIC2		0x0B
#define REG_GYRO_CONFIG_STATIC3		0x0C
#define REG_GYRO_CONFIG_STATIC4		0x0D
#define REG_GYRO_CONFIG_STATIC5		0x0E
#define REG_GYRO_CONFIG_STATIC6		0x0F
#define REG_GYRO_CONFIG_STATIC7		0x10
#define REG_GYRO_CONFIG_STATIC8		0x11
#define REG_GYRO_CONFIG_STATIC9		0x12
#define REG_GYRO_CONFIG_STATIC10	0x13
#define REG_XG_ST_DATA			0x5F
#define REG_YG_ST_DATA			0x60
#define REG_ZG_ST_DATA			0x61
#define REG_TMSTVAL0			0x62
#define REG_TMSTVAL1			0x63
#define REG_TMSTVAL2			0x64
#define REG_INTF_CONFIG4		0x7A
#define REG_INTF_CONFIG5		0x7B
#define REG_INTF_CONFIG6		0x7C

/* BANK 2 */
#define REG_ACCEL_CONFIG_STATIC2	0x03
#define REG_ACCEL_CONFIG_STATIC3	0x04
#define REG_ACCEL_CONFIG_STATIC4	0x05
#define REG_XA_ST_DATA			0x3B
#define REG_YA_ST_DATA			0x3C
#define REG_ZA_ST_DATA			0x3D

/* BANK 4 */
#define REG_GYRO_ON_OFF_CONFIG		0x0E
#define REG_APEX_CONFIG1		0x40
#define REG_APEX_CONFIG2		0x41
#define REG_APEX_CONFIG3		0x42
#define REG_APEX_CONFIG4		0x43
#define REG_APEX_CONFIG5		0x44
#define REG_APEX_CONFIG6		0x45
#define REG_APEX_CONFIG7		0x46
#define REG_APEX_CONFIG8		0x47
#define REG_APEX_CONFIG9		0x48
#define REG_ACCEL_WOM_X_THR		0x4A
#define REG_ACCEL_WOM_Y_THR		0x4B
#define REG_ACCEL_WOM_Z_THR		0x4C
#define REG_INT_SOURCE6			0x4D
#define REG_INT_SOURCE7			0x4E
#define REG_INT_SOURCE8			0x4F
#define REG_INT_SOURCE9			0x50
#define REG_INT_SOURCE10		0x51
#define REG_OFFSET_USER0		0x77
#define REG_OFFSET_USER1		0x78
#define REG_OFFSET_USER2		0x79
#define REG_OFFSET_USER3		0x7A
#define REG_OFFSET_USER4		0x7B
#define REG_OFFSET_USER5		0x7C
#define REG_OFFSET_USER6		0x7D
#define REG_OFFSET_USER7		0x7E
#define REG_OFFSET_USER8		0x7F

/* #define REG_#define REG_BANK_SEL */
#define BIT_BANK_SEL_0			0x00
#define BIT_BANK_SEL_1			0x01
#define BIT_BANK_SEL_2			0x02
#define BIT_BANK_SEL_3			0x03
#define BIT_BANK_SEL_4			0x04

#define WHO_AM_I_ICM42688		0x42

/* Bank0 #define REG_DEVICE_CONFIG_REG */
#define BIT_SOFT_RESET			0x01

/* Bank0 #define REG_GYRO_CONFIG0, REG_ACCEL_CONFIG0 */
#define SHIFT_GYRO_FS_SEL		5
#define SHIFT_ACCEL_FS_SEL		5
#define SHIFT_ODR_CONF			0

/* Bank0 #define REG_GYRO_CONFIG1 */
#define BIT_TEMP_FILT_BW_BYPASS		0x00
#define BIT_TEMP_FILT_BW_170		0x20
#define BIT_TEMP_FILT_BW_82		0x40
#define BIT_TEMP_FILT_BW_40		0x60
#define BIT_TEMP_FILT_BW_20		0x80
#define BIT_TEMP_FILT_BW_10		0x90
#define BIT_TEMP_FILT_BW_5		0xC0
#define BIT_GYR_AVG_FLT_RATE_8KHZ	0x10
#define BIT_GYR_AVG_FLT_RATE_1KHZ	0x00
#define BIT_GYR_UI_FILT_ORD_IND_1	0x00
#define BIT_GYR_UI_FILT_ORD_IND_2	0x04
#define BIT_GYR_UI_FILT_ORD_IND_3	0x08
#define BIT_GYR_DEC2_M2_ORD_1		0x00
#define BIT_GYR_DEC2_M2_ORD_2		0x01
#define BIT_GYR_DEC2_M2_ORD_3		0x02

/* Bank0 REG_ACCEL_CONFIG1 */
#define BIT_ACC_UI_FILT_ODR_IND_1	0x00
#define BIT_ACC_UI_FILT_ODR_IND_2	0x08
#define BIT_ACC_UI_FILT_ODR_IND_3	0x10
#define BIT_ACC_DEC2_M2_ORD_1		0x00
#define BIT_ACC_DEC2_M2_ORD_2		0x02
#define BIT_ACC_DEC2_M2_ORD_3		0x04
#define BIT_ACC_AVG_FLT_RATE_8KHZ	0x01
#define BIT_ACC_AVG_FLT_RATE_1KHZ	0x00

/* Bank0 REG_INT_CONFIG_REG */
#define SHIFT_INT1_POLARITY		0
#define SHIFT_INT1_DRIVE_CIRCUIT	1
#define SHIFT_INT1_MODE			2

/* Bank0 REG_PWR_MGMT_0 */
#define BIT_TEMP_DIS			0x20
#define BIT_IDLE			0x10
#define BIT_GYRO_MODE_OFF		0x00
#define BIT_GYRO_MODE_STBY		0x04
#define BIT_GYRO_MODE_LPM		0x08
#define BIT_GYRO_MODE_LNM		0x0C
#define BIT_ACCEL_MODE_OFF		0x00
#define BIT_ACCEL_MODE_LPM		0x02
#define BIT_ACCEL_MODE_LNM		0x03

/* Bank0 REG_SIGNAL_PATH_RESET */
#define BIT_TEMP_RST			0x01
#define BIT_FIFO_FLUSH			0x02
#define BIT_TMST_STROBE			0x04
#define BIT_ABORT_AND_RESET		0x08
#define BIT_S4S_RESTART			0x10
#define BIT_DMP_MEM_RESET_EN		0x20
#define BIT_DMP_INIT_EN			0x40

/* Bank0 REG_INTF_CONFIG0 */
#define BIT_FIFO_COUNT_REC		0x40
#define BIT_COUNT_BIG_ENDIAN		0x20
#define BIT_SENS_DATA_BIG_ENDIAN	0x10
#define BIT_UI_SIFS_DISABLE_SPI		0x02
#define BIT_UI_SIFS_DISABLE_I2C		0x03

/* Bank0 REG_INTF_CONFIG1 */
#define BIT_GYRO_AFSR_MODE_LFS		0x00
#define BIT_GYRO_AFSR_MODE_HFS		0x40
#define BIT_GYRO_AFSR_MODE_DYN		0xC0
#define BIT_ACCEL_AFSR_MODE_LFS		0x00
#define BIT_ACCEL_AFSR_MODE_HFS		0x10
#define BIT_ACCEL_AFSR_MODE_DYN		0x30
#define BIT_ACCEL_LP_CLK_SEL		0x08
#define BIT_RTC_MODE			0x04
#define BIT_CLK_SEL_RC			0x00
#define BIT_CLK_SEL_PLL			0x01
#define BIT_CLK_SEL_DIS			0x03

/* Bank0 REG_FIFO_CONFIG1 */
#define BIT_FIFO_ACCEL_EN		0x01
#define BIT_FIFO_GYRO_EN		0x02
#define BIT_FIFO_TEMP_EN		0x04
#define BIT_FIFO_TMST_FSYNC_EN		0x08
#define BIT_FIFO_HIRES_EN		0x10
#define BIT_FIFO_WM_TH			0x20
#define BIT_FIFO_RESUME_PART_RD		0x40

/* Bank0 REG_INT_CONFIG1 */
#define BIT_INT_ASY_RST_DISABLE		0x10

/* Bank0 REG_INT_SOURCE0 */
#define BIT_INT_UI_AGC_RDY_INT1_EN	0x01
#define BIT_INT_FIFO_FULL_INT1_EN	0x02
#define BIT_INT_FIFO_THS_INT1_EN	0x04
#define BIT_INT_UI_DRDY_INT1_EN		0x08
#define BIT_INT_RESET_DONE_INT1_EN	0x10
#define BIT_INT_PLL_RDY_INT1_EN		0x20
#define BIT_INT_UI_FSYNC_INT1_EN	0x40

/* Bank0 REG_INT_SOURCE1 */
#define BIT_INT_WOM_X_INT1_EN		0x01
#define BIT_INT_WOM_Y_INT1_EN		0x02
#define BIT_INT_WOM_Z_INT1_EN		0x04
#define BIT_INT_SMD_INT1_EN		0x08
#define BIT_INT_WOM_XYZ_INT1_EN \
	(BIT_INT_WOM_X_INT1_EN | BIT_INT_WOM_Y_INT1_EN | BIT_INT_WOM_Z_INT1_EN)

/* Bank0 REG_SENSOR_SELFTEST_REG1 */
#define BIT_ACCEL_SELF_TEST_PASS	0x08
#define BIT_GYRO_SELF_TEST_PASS		0x04
#define BIT_ACCEL_SELF_TEST_DONE	0x02
#define BIT_GYRO_SELF_TEST_DONE		0x01

/* Bank0 REG_SELF_TEST_CONFIG */
#define BIT_SELF_TEST_REGULATOR_EN	0x40
#define BIT_TEST_AZ_EN			0x20
#define BIT_TEST_AY_EN			0x10
#define BIT_TEST_AX_EN			0x08
#define BIT_TEST_GZ_EN			0x04
#define BIT_TEST_GY_EN			0x02
#define BIT_TEST_GX_EN			0x01

/* Bank0 REG_INT_STATUS */
#define BIT_INT_STATUS_AGC_RDY		0x01
#define BIT_INT_STATUS_FIFO_FULL	0x02
#define BIT_INT_STATUS_FIFO_THS		0x04
#define BIT_INT_STATUS_DRDY		0x08
#define BIT_INT_STATUS_RESET_DONE	0x10
#define BIT_INT_STATUS_PLL_DRY		0x20
#define BIT_INT_STATUS_UI_FSYNC		0x40

/* Bank0 REG_INT_STATUS2 */
#define BIT_INT_STATUS_WOM_X		0x01
#define BIT_INT_STATUS_WOM_Y		0x02
#define BIT_INT_STATUS_WOM_Z		0x04
#define BIT_INT_STATUS_SMD		0x08
#define BIT_INT_STATUS_WOM_XYZ \
	(BIT_INT_STATUS_WOM_X | BIT_INT_STATUS_WOM_Y | BIT_INT_STATUS_WOM_Z)

/* Bank0 REG_INT_STATUS3 */
#define BIT_INT_STATUS_TAP_DET		0x01
#define BIT_INT_STATUS_SLEEP_DET	0x02
#define BIT_INT_STATUS_RAISE_DET	0x04
#define BIT_INT_STATUS_TILT_DET		0x08
#define BIT_INT_STATUS_STEP_CNT_OVFL	0x10
#define BIT_INT_STATUS_STEP_DET		0x20
#define BIT_INT_STATUS_DMP_POWER_SAVE	0x40

/* Bank0 REG_FIFO_CONFIG_REG */
#define BIT_FIFO_MODE_BYPASS		0x00
#define BIT_FIFO_MODE_STREAM		0x40
#define BIT_FIFO_MODE_STOP_FULL		0x80

/* Bank0 REG_GYRO_ACCEL_CONFIG0 */
#define BIT_ACCEL_UI_LNM_BW_2_FIR	0x00
#define BIT_ACCEL_UI_LNM_BW_4_IIR	0x10
#define BIT_ACCEL_UI_LNM_BW_5_IIR	0x20
#define BIT_ACCEL_UI_LNM_BW_8_IIR	0x30
#define BIT_ACCEL_UI_LNM_BW_10_IIR	0x40
#define BIT_ACCEL_UI_LNM_BW_16_IIR	0x50
#define BIT_ACCEL_UI_LNM_BW_20_IIR	0x60
#define BIT_ACCEL_UI_LNM_BW_40_IIR	0x70
#define BIT_ACCEL_UI_LNM_AVG_1		0xF0
#define BIT_ACCEL_UI_LPM_BW_2_FIR	0x00
#define BIT_ACCEL_UI_LPM_AVG_1		0x10
#define BIT_ACCEL_UI_LPM_AVG_2		0x20
#define BIT_ACCEL_UI_LPM_AVG_3		0x30
#define BIT_ACCEL_UI_LPM_AVG_4		0x40
#define BIT_ACCEL_UI_LPM_AVG_8		0x50
#define BIT_ACCEL_UI_LPM_AVG_16		0x60
#define BIT_ACCEL_UI_LPM_AVG_32		0x70
#define BIT_ACCEL_UI_LPM_AVG_64		0x80
#define BIT_ACCEL_UI_LPM_AVG_128	0x90
#define BIT_GYRO_UI_LNM_BW_2_FIR	0x00
#define BIT_GYRO_UI_LNM_BW_4_IIR	0x01
#define BIT_GYRO_UI_LNM_BW_5_IIR	0x02
#define BIT_GYRO_UI_LNM_BW_8_IIR	0x03
#define BIT_GYRO_UI_LNM_BW_10_IIR	0x04
#define BIT_GYRO_UI_LNM_BW_16_IIR	0x05
#define BIT_GYRO_UI_LNM_BW_20_IIR	0x06
#define BIT_GYRO_UI_LNM_BW_40_IIR	0x07
#define BIT_GYRO_UI_LNM_AVG_1		0xF0
#define BIT_GYRO_UI_LPM_BW_2_FIR	0x00
#define BIT_GYRO_UI_LPM_AVG_1		0x01
#define BIT_GYRO_UI_LPM_AVG_2		0x02
#define BIT_GYRO_UI_LPM_AVG_3		0x03
#define BIT_GYRO_UI_LPM_AVG_4		0x04
#define BIT_GYRO_UI_LPM_AVG_8		0x05
#define BIT_GYRO_UI_LPM_AVG_16		0x06
#define BIT_GYRO_UI_LPM_AVG_32		0x07
#define BIT_GYRO_UI_LPM_AVG_64		0x08
#define BIT_GYRO_UI_LPM_AVG_128		0x09

/* Bank0 REG_SMD_CONFIG */
#define BIT_WOM_INT_MODE_OR		0x00
#define BIT_WOM_INT_MODE_AND		0x08
#define BIT_WOM_MODE_INITIAL		0x00
#define BIT_WOM_MODE_PREV		0x04
#define BIT_SMD_MODE_OFF		0x00
#define BIT_SMD_MODE_OLD		0x01
#define BIT_SMD_MODE_SHORT		0x02
#define BIT_SMD_MODE_LONG		0x03

/* Bank0 REG_TMST_CONFIG */
#define BIT_FIFO_RAM_ISO_ENA		0x40
#define BIT_EN_DREG_FIFO_D2A		0x20
#define BIT_TMST_TO_REGS_EN		0x10
#define BIT_TMST_RESOL			0x08
#define BIT_TMST_DELTA_EN		0x04
#define BIT_TMST_FSYNC_EN		0x02
#define BIT_TMST_EN			0x01

/* Bank0 REG_APEX_CONFIG0 */
#define BIT_DMP_ODR_25HZ		0x00
#define BIT_DMP_ODR_50HZ		0x02
#define BIT_DMP_ODR_100HZ		0x03
#define BIT_RAISE_ENABLE		0x08
#define BIT_TILT_ENABLE			0x10
#define BIT_PEDO_ENABLE			0x20
#define BIT_TAP_ENABLE			0x40
#define BIT_DMP_POWER_SAVE_EN		0x80

/* Bank0 REG_ACCEL_CONFIG0 */
#define BIT_ACCEL_FSR			0xE0
#define BIT_ACCEL_ODR			0x0F

#define BIT_ACCEL_ODR_8000		0x03
#define BIT_ACCEL_ODR_4000		0x04
#define BIT_ACCEL_ODR_2000		0x05
#define BIT_ACCEL_ODR_1000		0x06
#define BIT_ACCEL_ODR_500		0x0F
#define BIT_ACCEL_ODR_200		0x07
#define BIT_ACCEL_ODR_100		0x08
#define BIT_ACCEL_ODR_50		0x09
#define BIT_ACCEL_ODR_25		0x0A
#define BIT_ACCEL_ODR_12		0x0B
#define BIT_ACCEL_ODR_6			0x0C
#define BIT_ACCEL_ODR_3			0x0D
#define BIT_ACCEL_ODR_1			0x0E

/* Bank0 REG_GYRO_CONFIG0 */
#define BIT_GYRO_FSR			0xE0
#define BIT_GYRO_ODR			0x0F
#define BIT_GYRO_ODR_8000		0x03
#define BIT_GYRO_ODR_4000		0x04
#define BIT_GYRO_ODR_2000		0x05
#define BIT_GYRO_ODR_1000		0x06
#define BIT_GYRO_ODR_500		0x0F
#define BIT_GYRO_ODR_200		0x07
#define BIT_GYRO_ODR_100		0x08
#define BIT_GYRO_ODR_50			0x09
#define BIT_GYRO_ODR_25			0x0A
#define BIT_GYRO_ODR_12			0x0B

/* Bank1 REG_INTF_CONFIG5 */
#define BIT_PIN9_FUNC_INT2		0x00
#define BIT_PIN9_FUNC_FSYNC		0x02
#define BIT_PIN9_FUNC_CLKIN		0x04
#define BIT_PIN9_FUNC_RSV		0x06

/* Bank4 REG_DRV_GYR_CFG0_REG */
#define GYRO_DRV_TEST_FSMFORCE_D2A_LINEAR_START_MODE		0x0D
#define GYRO_DRV_TEST_FSMFORCE_D2A_STEADY_STATE_AGC_REG_MODE	0x2A

/* Bank4 REG_DRV_GYR_CFG2_REG */
#define GYRO_DRV_SPARE2_D2A_EN		0x01

/* Bank4 REG_INT_SOURCE6 */
#define BIT_INT_TAP_DET_INT1_EN		0x01
#define BIT_INT_SLEEP_DET_INT1_EN	0x02
#define BIT_INT_RAISE_DET_INT1_EN	0x04
#define BIT_INT_TILT_DET_INT1_EN	0x08
#define BIT_INT_STEP_CNT_OVFL_INT1_EN	0x10
#define BIT_INT_STEP_DET_INT1_EN	0x20
#define BIT_INT_DMP_POWER_SAVE_INT1_EN	0x40

/* Bank4 REG_INT_SOURCE7 */
#define BIT_INT_TAP_DET_INT2_EN		0x01
#define BIT_INT_HIGHG_DET_INT2_EN	0x02
#define BIT_INT_LOWG_DET_INT2_EN	0x04
#define BIT_INT_TILT_DET_INT2_EN	0x80
#define BIT_INT_STEP_CNT_OVFL_INT2_EN	0x10
#define BIT_INT_STEP_DET_INT2_EN	0x20
#define BIT_INT_DMP_POWER_SAVE_INT2_EN	0x40

/* Bank4 REG_INT_SOURCE8 */
#define BIT_INT_AGC_RDY_IBI_EN		0x01
#define BIT_INT_FIFO_FULL_IBI_EN	0x02
#define BIT_INT_FIFO_THS_IBI_EN		0x04
#define BIT_INT_UI_DRDY_IBI_EN		0x08
#define BIT_INT_PLL_RDY_IBI_EN		0x10
#define BIT_INT_FSYNC_IBI_EN		0x20
#define BIT_INT_OIS1_DRDY_IBI_EN	0x40

/* Bank4 REG_INT_SOURCE9 */
#define BIT_INT_DMP_POWER_SAVE_IBI_EN	0x01
#define BIT_INT_WOM_X_IBI_EN		0x02
#define BIT_INT_WOM_Y_IBI_EN		0x04
#define BIT_INT_WOM_Z_IBI_EN		0x08
#define BIT_INT_SMD_IBI_EN		0x10

/* Bank4 REG_INT_SOURCE10 */
#define BIT_INT_TAP_DET_IBI_EN		0x01
#define BIT_INT_HIGHG_DET_IBI_EN	0x02
#define BIT_INT_LOWG_DET_IBI_EN		0x04
#define BIT_INT_TILT_DET_IBI_EN		0x08
#define BIT_INT_STEP_CNT_OVFL_IBI_EN	0x10
#define BIT_INT_STEP_DET_IBI_EN		0x20

/* fifo data packet header */
#define BIT_FIFO_HEAD_MSG		0x80
#define BIT_FIFO_HEAD_ACCEL		0x40
#define BIT_FIFO_HEAD_GYRO		0x20
#define BIT_FIFO_HEAD_20		0x10
#define BIT_FIFO_HEAD_TMSP_ODR		0x08
#define BIT_FIFO_HEAD_TMSP_NO_ODR	0x04
#define BIT_FIFO_HEAD_TMSP_FSYNC	0x0C
#define BIT_FIFO_HEAD_ODR_ACCEL		0x02
#define BIT_FIFO_HEAD_ODR_GYRO		0x01

/* data definitions */
#define FIFO_PACKET_BYTE_SINGLE		8
#define FIFO_PACKET_BYTE_6X		16
#define FIFO_PACKET_BYTE_HIRES		20
#define FIFO_COUNT_BYTE			2

/* sensor startup time */
#define INV_ICM42600_GYRO_START_TIME	100
#define INV_ICM42600_ACCEL_START_TIME	50

/* temperature sensor */
/* scale by 100, 1LSB=1degC, 9447 */
#define TEMP_SCALE			100
/* 25 degC */
#define TEMP_OFFSET			(25 * TEMP_SCALE)

#ifdef SUPPORT_RTC_MODE
#define BASE_SAMPLE_RATE		(RTC_FREQ_HZ / 32)
#else
#define BASE_SAMPLE_RATE		1000
#endif
#define GESTURE_ACCEL_RATE		50
#define ESI_GYRO_RATE			1000
#define MPU_INIT_SENSOR_RATE_LNM	12	/* min Hz in LNM */
#define MPU_INIT_SENSOR_RATE_LPM	3	/* min Hz in LPM */
#define MAX_FIFO_PACKET_READ		16
#define HARDWARE_FIFO_SIZE		2048
#define FIFO_SIZE			(HARDWARE_FIFO_SIZE * 7 / 8)
#define LEFT_OVER_BYTES			128
#define POWER_UP_TIME			100
#define REG_UP_TIME_USEC		100
#define IIO_BUFFER_BYTES		8
#define BYTES_PER_SENSOR		6
#define BYTES_FOR_TEMP			1
#define MAX_BATCH_FIFO_SIZE		FIFO_SIZE
#define FIRST_DROP_SAMPLES_ACC_500HZ	20
#define FIRST_DROP_SAMPLES_ACC_200HZ	10
#define FIRST_DROP_SAMPLES_GYR_500HZ	20
#define FIRST_DROP_SAMPLES_GYR_200HZ	10
#define WOM_THRESHOLD			13 /* 1000 / 256 * 13 = 50.7mg */

#define BIT_GYRO_FSR			0xE0
#define BIT_GYRO_ODR			0x0F
#define BIT_ACCEL_FSR			0xE0
#define BIT_ACCEL_ODR			0x0F

#define FIFO_ACCEL0_RESET_VALUE		0x80
#define FIFO_ACCEL1_RESET_VALUE		0x00
#define FIFO_GYRO0_RESET_VALUE		0x80
#define FIFO_GYRO1_RESET_VALUE		0x00

#define APEX_TAP			0x08
#define APEX_DOUBLE_TAP			0x10

/*
 * INT configurations
 * Polarity: 0 -> Active Low, 1 -> Active High
 * Drive circuit: 0 -> Open Drain, 1 -> Push-Pull
 * Mode: 0 -> Pulse, 1 -> Latch
 */
#define INT_POLARITY			1
#define INT_DRIVE_CIRCUIT		1
#define INT_MODE			0

#define ACC_LPM_MAX_RATE		500
#define GYR_LPM_MAX_RATE		200

enum {
	GYRO_FS_2000DPS = 0,
	GYRO_FS_1000DPS,
	GYRO_FS_500DPS,
	GYRO_FS_250DPS,
	GYRO_FS_125DPS,
	GYRO_FS_62DPS,
	GYRO_FS_32DPS,
	GYRO_FS_15DPS,
};

enum {
	ACCEL_FS_16G = 0,
	ACCEL_FS_8G,
	ACCEL_FS_4G,
	ACCEL_FS_2G,
};

#endif /* __SENSOR_ICM42688_ICM42688_REG__ */
