/*
 * mpu6500.c
 *
 *  Created on: Feb 6, 2026
 *      Author: Mali
 */


#include "mpu6500.h"

//--------------------------------------------------------------------------VARIABLES----------------------------------------------------------------------------//
uint8_t accel_current_value = 0;
float accel_sensitivity = 16384.0f;
uint8_t gyro_current_value = 0;
float gyro_sensitivity = 131.0f;
float gyro_offset_x = 0.0;
float gyro_offset_y = 0.0;
float gyro_offset_z = 0.0;

//--------------------------------------------------------------------------FUNCTIONS----------------------------------------------------------------------------//
void mpu6500_read_reg(return_code_t *return_code, uint8_t addr, uint8_t *buffer, uint16_t trials)
{
	if(HAL_I2C_Mem_Read(&hi2c1, MPU6500_I2C_ADDR<<1, addr, I2C_MEMADD_SIZE_8BIT, buffer, trials, HAL_MAX_DELAY) == HAL_OK)
	{
		*return_code = NO_ERROR;
	}
	else
	{
		*return_code = INVALID_PARAM;
	}
}

void mpu6500_write_reg(return_code_t *return_code, uint8_t addr, uint8_t *value)
{
	if(HAL_I2C_Mem_Write(&hi2c1, MPU6500_I2C_ADDR<<1, addr, I2C_MEMADD_SIZE_8BIT, value, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		*return_code = NO_ERROR;
	}
	else
	{
		*return_code = INVALID_PARAM;
	}
}

void mpu6500_init(return_code_t *return_code, device_start_mode_t device_start_mode)
{
	uint8_t who_am_i_ret = 0;
	uint8_t tx_value = 0;
	mpu6500_read_reg(return_code, MPU6500_WHO_AM_I, &who_am_i_ret, 1);
	if(who_am_i_ret == MPU6500_WHO_AM_I_RET)
	{
		*return_code = NO_ERROR;
	}

	if(device_start_mode == STANDARD)
	{
		tx_value = WAKE_UP;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_1, &tx_value);
		tx_value = ENABLE;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_2, &tx_value);
	}

	else if(device_start_mode == LOW_POWER)
	{
		tx_value = WAKE_UP;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_1, &tx_value);
		tx_value = DISABLE_GYRO;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_2, &tx_value);
	}

	else if(device_start_mode == SLEEP) /* Deep Sleep Mode */
	{
		tx_value = SLEEP_ENABLE;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_1, &tx_value);
		tx_value = ENABLE;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_2, &tx_value);
	}

	else if(device_start_mode == GYRO_ONLY)
	{
		tx_value = WAKE_UP;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_1, &tx_value);
		tx_value = DISABLE_ACCEL;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_2, &tx_value);
	}

	else if(device_start_mode == DEVICE_RESET)
	{
		tx_value = RESET_ENABLE;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_1, &tx_value);
		HAL_Delay(100);
		tx_value = ENABLE;
		mpu6500_write_reg(return_code, MPU6500_PWR_MGMT_2, &tx_value);
	}

	else
	{
		*return_code = INVALID_CONFIG;
	}
}

void mpu6500_set_accel_range(return_code_t *return_code, accel_range_t accel_range)
{
	uint8_t accel_check_value = 0;
	uint8_t accel_set_value = 0;
	mpu6500_read_reg(return_code, MPU6500_ACCEL_CONFIG, &accel_current_value, 1);
	accel_check_value = accel_current_value & 0x18; /* to get 3-4 bits */

	if(accel_check_value == accel_range)
	{
		*return_code = NO_ACTION;
		return;
	}

	else
	{
		accel_set_value = accel_current_value & 0xE7; /* to clear 3-4 bits */
		accel_set_value |= accel_range;
		mpu6500_write_reg(return_code, MPU6500_ACCEL_CONFIG, &accel_set_value);

		switch(accel_range)
		{
			case RANGE_ACCEL_2G:  accel_sensitivity = 16384.0f; break;
			case RANGE_ACCEL_4G:  accel_sensitivity = 8192.0f;  break;
			case RANGE_ACCEL_8G:  accel_sensitivity = 4096.0f;  break;
			case RANGE_ACCEL_16G: accel_sensitivity = 2048.0f;  break;
		}
	}

}

void mpu6500_get_accel(return_code_t *return_code, float buffer[3])
{
	uint8_t rx_buffer[6];
	int16_t raw_x = 0;
	int16_t raw_y = 0;
	int16_t raw_z = 0;

	mpu6500_read_reg(return_code, MPU6500_ACCEL_XOUT_H, rx_buffer, 6);

	raw_x = (int16_t)((rx_buffer[0] << 8) | rx_buffer[1]);
	raw_y = (int16_t)((rx_buffer[2] << 8) | rx_buffer[3]);
	raw_z = (int16_t)((rx_buffer[4] << 8) | rx_buffer[5]);

	buffer[0] = (float)raw_x / accel_sensitivity;
	buffer[1] = (float)raw_y / accel_sensitivity;
	buffer[2] = (float)raw_z / accel_sensitivity;
}

void mpu6500_set_gyro_range(return_code_t *return_code, gyro_range_t gyro_range)
{
	uint8_t gyro_check_value = 0;
	uint8_t gyro_set_value = 0;
	mpu6500_read_reg(return_code, MPU6500_GYRO_CONFIG, &gyro_current_value, 1);
	gyro_check_value = gyro_current_value & 0x18; /* to get 3-4 bits */

	if(gyro_check_value == gyro_range)
	{
		*return_code = NO_ACTION;
		return;
	}

	else
	{
		gyro_set_value = gyro_current_value & 0xE7; /* to clear 3-4 bits */
		gyro_set_value |= gyro_range;
		mpu6500_write_reg(return_code, MPU6500_GYRO_CONFIG, &gyro_set_value);

		switch(gyro_range)
		{
			case RANGE_GYRO_250DPS:  gyro_sensitivity  = 131.0f; break;
			case RANGE_GYRO_500DPS:  gyro_sensitivity  = 65.5f;  break;
			case RANGE_GYRO_1000DPS: gyro_sensitivity  = 32.8f;  break;
			case RANGE_GYRO_2000DPS: gyro_sensitivity  = 16.4f;  break;
		}
	}
}

void mpu6500_gyro_calib(return_code_t *return_code)
{
	float sum_x = 0.0;
	float sum_y = 0.0;
	float sum_z = 0.0;
	float gyro_test_buff[3];

	gyro_offset_x = 0.0;
	gyro_offset_y = 0.0;
	gyro_offset_z = 0.0;

	for(int i = 0; i < 1000; i++)
	{
		mpu6500_get_gyro(return_code, gyro_test_buff);
		sum_x = sum_x + gyro_test_buff[0];
		sum_y = sum_y + gyro_test_buff[1];
		sum_z = sum_z + gyro_test_buff[2];
		HAL_Delay(3);
	}

	gyro_offset_x = sum_x / 1000.0;
	gyro_offset_y = sum_y / 1000.0;
	gyro_offset_z = sum_z / 1000.0;

}

void mpu6500_get_gyro(return_code_t *return_code, float buffer[3])
{
	uint8_t rx_buffer[6];
	int16_t raw_x = 0;
	int16_t raw_y = 0;
	int16_t raw_z = 0;

	mpu6500_read_reg(return_code, MPU6500_GYRO_XOUT_H, rx_buffer, 6);

	raw_x = (int16_t)((rx_buffer[0] << 8) | rx_buffer[1]);
	raw_y = (int16_t)((rx_buffer[2] << 8) | rx_buffer[3]);
	raw_z = (int16_t)((rx_buffer[4] << 8) | rx_buffer[5]);

	buffer[0] = ((float)raw_x / gyro_sensitivity) - gyro_offset_x;
	buffer[1] = ((float)raw_y / gyro_sensitivity) - gyro_offset_y;
	buffer[2] = ((float)raw_z / gyro_sensitivity) - gyro_offset_z;
}

float mpu6500_get_temp(return_code_t *return_code)
{
	uint8_t rx_buffer[2];
	int16_t raw_temp = 0;
	float temp = 0.0;
	mpu6500_read_reg(return_code, MPU6500_TEMP_OUT_H, rx_buffer, 2);

	raw_temp = (int16_t)((rx_buffer[0] << 8) | rx_buffer[1]);
	temp = (float)((raw_temp / 333.87f) + 21.0f);

	return temp-4;
}


