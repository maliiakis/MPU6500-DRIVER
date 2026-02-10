# STM32 MPU-6500 Driver

A simple and efficient C driver for the InvenSense MPU-6500 6-axis MEMS sensor (Accelerometer + Gyroscope), designed for STM32 microcontrollers using the HAL library.

## Features
- Reads Accelerometer, Gyroscope, and Temperature data.
- Converts raw data to physical units.
- Includes Gyroscope calibration to remove zero-rate offset.
- Configurable sensitivity ranges (up to 16g and 2000dps).

## How to Use
1. Add mpu6500.c and mpu6500.h to your project source folders.
2. Initialize the sensor using the init function.
3. Run the calibration function to fix gyro offsets.
4. Read sensor data using the get_accel and get_gyro functions.
