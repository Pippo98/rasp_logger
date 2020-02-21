#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "LSM9DS0.h"

int spi;
int spichannel = 1;

int CS_XM = 2;
int CS_G = 3;

int accelScale = 4;
int gyroScale = 500;

float gyro_x, gyro_y, gyro_z;

void LSMD9S0_gyro_read(int, int);
int check(int, int);

int main()
{

	int wpistp = wiringPiSetup();

	spi = wiringPiSPISetup(spichannel, 1000000);

	delay(10);

	pinMode(CS_G, OUTPUT);
	pinMode(CS_XM, OUTPUT);

	delay(10);

	digitalWrite(CS_XM, 1);
	digitalWrite(CS_G, 1);

	printf("wiringpi: %d, spi %d\r\n", wpistp, spi);

	init(spichannel, CS_G, &gyroScale, &accelScale);

	delay(10);

	for (int i = 0; i < 254; i++)
	{

		for (int j = 0; j < 254; j++)
		{
			uint8_t bff1[2];
			bff1[0] = i;
			bff1[1] = j;
			uint8_t bff2[2];
			bff2[0] = i;
			bff2[1] = j;

			digitalWrite(CS_G, 0);

			int val1 = wiringPiSPIDataRW(spichannel, bff1, 2);

			digitalWrite(CS_G, 1);

			delayMicroseconds(10);

			digitalWrite(CS_XM, 0);

			int val2 = wiringPiSPIDataRW(spichannel, bff2, 2);

			digitalWrite(CS_XM, 1);

			printf("val1: %d, i1: %d-%d, val2: %d, i2: %d-%d\r\n", val1, val2, bff1[0], bff1[1], bff2[0], bff2[1]);

			delayMicroseconds(10);
		}
	}

	// while (true)
	// {
	// 	LSMD9S0_gyro_read(spichannel, CS_G);
	// 	delay(10);
	// 	int val = check(spichannel, CS_G);
	// 	// printf("%d\r\n", val);
	// 	delay(100);
	// }
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//

uint8_t ZERO = 0x00;
uint8_t WHO_AM_I_G = 0x8F;
uint8_t WHO_AM_I_G_VAL;
uint8_t WHO_AM_I_XM = 0x8F;
uint8_t WHO_AM_I_XM_VAL;

uint8_t CTRL_REG1_G_ADD = 0x20;
uint8_t CTRL_REG1_G_VAL = 0x0F;
uint8_t CTRL_REG4_G_ADD = 0x23;
uint8_t CTRL_REG4_G_VAL = 0x10;

uint8_t CTRL_REG1_XM_ADD = 0x20;
uint8_t CTRL_REG1_XM_VAL = 0xA7;
uint8_t CTRL_REG2_XM_ADD = 0x21;
uint8_t CTRL_REG2_XM_VAL = 0x08;
uint8_t CTRL_REG5_XM_ADD = 0x24;
uint8_t CTRL_REG5_XM_VAL = 0x70;
uint8_t CTRL_REG6_XM_ADD = 0x25;
uint8_t CTRL_REG6_XM_VAL = 0x20;
uint8_t CTRL_REG7_XM_ADD = 0x26;
uint8_t CTRL_REG7_XM_VAL = 0x00;

// GYROSCOPE SCALE SETTING
uint8_t CTRL_REG4_G = 0x23;

uint8_t SCL_G_245 = 0x00;
uint8_t SCL_G_500 = 0x10;
uint8_t SCL_G_1000 = 0x20;
uint8_t SCL_G_2000 = 0x30;

// ACCELERORMETER SCALE SETTING
uint8_t CTRL_REG2_XM = 0x21;

uint8_t SCL_A_2 = 0x00;
uint8_t SCL_A_4 = 0x08;
uint8_t SCL_A_6 = 0x10;
uint8_t SCL_A_8 = 0x18;
uint8_t SCL_A_16 = 0x20;

// MAGNETORMETER SCALE SETTING
uint8_t CTRL_REG6_XM = 0x25;

uint8_t SCL_M_2 = 0x00;
uint8_t SCL_M_4 = 0x20;
uint8_t SCL_M_8 = 0x40;
uint8_t SCL_M_12 = 0x60;

// OUTPUT REG
uint8_t OUT_X_L_G_ADD = 0xA8;
uint8_t OUT_X_H_G_ADD = 0xA9;
uint8_t OUT_Y_L_G_ADD = 0xAA;
uint8_t OUT_Y_H_G_ADD = 0xAB;
uint8_t OUT_Z_L_G_ADD = 0xAC;
uint8_t OUT_Z_H_G_ADD = 0xAD;

uint8_t OUT_X_L_A_ADD = 0xA8;
uint8_t OUT_X_H_A_ADD = 0xA9;
uint8_t OUT_Y_L_A_ADD = 0xAA;
uint8_t OUT_Y_H_A_ADD = 0xAB;
uint8_t OUT_Z_L_A_ADD = 0xAC;
uint8_t OUT_Z_H_A_ADD = 0xAD;

int check(int SPIChannel, int CS)
{

	digitalWrite(CS, 0);

	int val = wiringPiSPIDataRW(SPIChannel, &WHO_AM_I_G, 1);

	digitalWrite(CS, 1);

	printf("%d ----- %u\n", val, WHO_AM_I_G);

	return val;

	// if (val == 212)
	// {
	// 	return 0;
	// }
	// else
	// {
	// 	return -1;
	// }
}

float LSMD9S0_read(int SPIChannel, int CS, uint8_t L_VAL, uint8_t H_VAL)
{

	float axis;

	unsigned char data[2];

	data[0] = L_VAL;
	data[1] = H_VAL;

	digitalWrite(CS, 0);
	delayMicroseconds(10);

	axis = wiringPiSPIDataRW(SPIChannel, data, 2);

	delayMicroseconds(10);

	digitalWrite(CS, 1);

	return axis;
}

//Reading G_axis values
//hspi = pointer to the spi port defined
//x = pointer gyroscope x variable
//y = pointer gyroscope y variable
//z = pointer gyroscope z variable
//x_offset = offset x value
//y_offset = offset y value
//z_offset = offset z value
void LSMD9S0_gyro_read(int SPIChannel, int CS)
{

	gyro_x = LSMD9S0_read(SPIChannel, CS, OUT_X_L_G_ADD, OUT_X_H_G_ADD);
	gyro_y = LSMD9S0_read(SPIChannel, CS, OUT_Y_L_G_ADD, OUT_Y_H_G_ADD);
	gyro_z = LSMD9S0_read(SPIChannel, CS, OUT_Z_L_G_ADD, OUT_Z_H_G_ADD);

	if (gyro_x > 32768)
	{
		gyro_x -= 65536;
	}
	if (gyro_y > 32768)
	{
		gyro_y -= 65536;
	}
	if (gyro_z > 32768)
	{
		gyro_z -= 65536;
	}

	gyro_x = gyro_x * ((float)gyroScale / 32768);
	gyro_y = gyro_y * ((float)gyroScale / 32768);
	gyro_z = gyro_z * ((float)gyroScale / 32768);

	printf("%d\r\n", (int)(gyro_x * 100));
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//

void send_config(int SPIChannel, int CS, uint8_t addr, uint8_t val)
{

	digitalWrite(CS, 0);
	delayMicroseconds(10);

	unsigned char data[2];

	data[0] = addr;
	data[1] = val;

	//wiringPiSPIDataRW(SPIChannel, data, 2);

	wiringPiSPIDataRW(SPIChannel, &addr, 1);
	wiringPiSPIDataRW(SPIChannel, &val, 1);

	delayMicroseconds(10);

	digitalWrite(CS, 1);
}

//accelerometer, gyroscope and magnetometer initialization
//call this function before requesting data from the sensor
//hspi = pointer to the spi port defined
void init(int SPIChannel, int CS, int *gyro_scale, int *accel_scale)
{

	CS = CS_G;

	// Wake Up Gyro, enabling x, y, z axis
	send_config(SPIChannel, CS, CTRL_REG1_G_ADD, CTRL_REG1_G_VAL);

	// Set Gyro scale range
	switch (*gyro_scale)
	{
	case 245:
		send_config(SPIChannel, CS, CTRL_REG4_G, SCL_G_245);
		break;
	case 500:
		send_config(SPIChannel, CS, CTRL_REG4_G, SCL_G_500);
		break;
	case 1000:
		send_config(SPIChannel, CS, CTRL_REG4_G, SCL_G_1000);
		break;
	case 2000:
		send_config(SPIChannel, CS, CTRL_REG4_G, SCL_G_2000);
		break;
	default:
		send_config(SPIChannel, CS, CTRL_REG4_G, SCL_G_245);
		*gyro_scale = 500;
		break;
	}

	CS = CS_XM;

	// Wake Up Accel, enabling x, y, z axis
	send_config(SPIChannel, CS, CTRL_REG1_XM_ADD, CTRL_REG1_XM_VAL);

	send_config(SPIChannel, CS, CTRL_REG5_XM_ADD, CTRL_REG5_XM_VAL);
	send_config(SPIChannel, CS, CTRL_REG6_XM_ADD, CTRL_REG6_XM_VAL);
	send_config(SPIChannel, CS, CTRL_REG7_XM_ADD, CTRL_REG7_XM_VAL);

	// Set Accel scale range
	switch (*accel_scale)
	{
	case 2:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_2);
		break;
	case 4:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_4);
		break;
	case 6:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_6);
		break;
	case 8:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_8);
		break;
	case 16:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_16);
		break;
	default:
		send_config(SPIChannel, CS, CTRL_REG2_XM, SCL_A_4);
		*accel_scale = 4;
		break;
	}
}
