/*
 * mpu6500.h
 *
 *  Created on: Feb 6, 2026
 *      Author: Mali
 */

#ifndef INC_MPU6500_H_
#define INC_MPU6500_H_

//--------------------------------------------------------------------------INCLUDES-----------------------------------------------------------------------------//
#include "main.h"

//--------------------------------------------------------------------------DEFINITIONS--------------------------------------------------------------------------//
#define MPU6500_I2C_ADDR	  0x68
#define MPU6500_CONFIG		  0x1A
#define MPU6500_GYRO_CONFIG	  0x1B
#define MPU6500_ACCEL_CONFIG  0x1C
#define MPU6500_ACCEL_CONFIG2 0x1D
#define MPU6500_FIFO_EN	 	  0x23
#define MPU6500_INT_PIN_CFG	  0x37
#define MPU6500_INT_ENABLE	  0x38
#define MPU6500_INT_STATUS	  0x3A
#define MPU6500_ACCEL_XOUT_H  0x3B /* Accelerometer */
#define MPU6500_ACCEL_XOUT_L  0x3C
#define MPU6500_ACCEL_YOUT_H  0x3D
#define MPU6500_ACCEL_YOUT_L  0x3E
#define MPU6500_ACCEL_ZOUT_H  0x3F
#define MPU6500_ACCEL_ZOUT_L  0x40
#define MPU6500_TEMP_OUT_H    0x41 /* Temperature */
#define MPU6500_TEMP_OUT_L	  0x42
#define MPU6500_GYRO_XOUT_H   0x43 /* Gyroscope */
#define MPU6500_GYRO_XOUT_L	  0x44
#define MPU6500_GYRO_YOUT_H	  0x45
#define MPU6500_GYRO_YOUT_L	  0x46
#define MPU6500_GYRO_ZOUT_H	  0x47
#define MPU6500_GYRO_ZOUT_L   0x48
#define MPU6500_USER_CTRL	  0x6A
#define MPU6500_PWR_MGMT_1 	  0x6B
#define MPU6500_PWR_MGMT_2	  0x6C
#define MPU6500_WHO_AM_I	  0x75 /* WHO AM I*/
#define MPU6500_WHO_AM_I_RET  0x70

//--------------------------------------------------------------------------CONFIG DEFINITIONS-------------------------------------------------------------------//
#define ENABLE				  0x00
#define WAKE_UP				  0x01 /* Wake up and Auto Select Clock (need to write MPU6500_PWR_MGMT_1 reg)*/
#define DISABLE_GYRO		  0x38 /* Disable X, Y, Z Gyroscope */
#define DISABLE_ACCEL		  0x07
#define SLEEP_ENABLE		  0x40
#define RESET_ENABLE		  0x80

//--------------------------------------------------------------------------TYPEDEFS-----------------------------------------------------------------------------//
typedef enum{
	STANDARD = 0,
	LOW_POWER,
	SLEEP,
	GYRO_ONLY,
	DEVICE_RESET
}device_start_mode_t;

typedef enum{
	RANGE_ACCEL_2G  = 0x00,
	RANGE_ACCEL_4G  = 0x08,
	RANGE_ACCEL_8G  = 0x10,
	RANGE_ACCEL_16G = 0x18
}accel_range_t;

typedef enum{
	RANGE_GYRO_250DPS  = 0x00,
	RANGE_GYRO_500DPS  = 0x08,
	RANGE_GYRO_1000DPS = 0x10,
	RANGE_GYRO_2000DPS = 0x18
}gyro_range_t;

typedef enum{
	NO_ACTION = 0,
	NOT_AVAILABLE,
	INVALID_PARAM,
	INVALID_CONFIG,
	TIMED_OUT,
	NO_ERROR
}return_code_t;

//--------------------------------------------------------------------------VARIABLES----------------------------------------------------------------------------//
extern uint8_t accel_current_value;
extern float accel_sensitivity;
extern uint8_t gyro_current_value;
extern float gyro_sensitivity;
extern float gyro_offset_x;
extern float gyro_offset_y;
extern float gyro_offset_z;

//--------------------------------------------------------------------------FUNCTIONS----------------------------------------------------------------------------//
void mpu6500_init(return_code_t *return_code, device_start_mode_t device_start_mode);
void mpu6500_read_reg(return_code_t *return_code, uint8_t addr, uint8_t *buffer, uint16_t trials);
void mpu6500_write_reg(return_code_t *return_code, uint8_t addr, uint8_t *value);
void mpu6500_set_accel_range(return_code_t *return_code, accel_range_t accel_range);
void mpu6500_get_accel(return_code_t *return_code, float buffer[3]);
void mpu6500_set_gyro_range(return_code_t *return_code, gyro_range_t gyro_range);
void mpu6500_get_gyro(return_code_t *return_code, float buffer[3]);
void mpu6500_gyro_calib(return_code_t *return_code);
float mpu6500_get_temp(return_code_t *return_code);

#endif /* INC_MPU6500_H_ */
