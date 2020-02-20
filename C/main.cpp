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

int accelScale = 4;
int gyroScale = 500;

int CS = 3;

int main(){
	
	wiringPiSetup();

	spi = wiringPiSPISetup(0, 9000000);

	pinMode(CS, OUTPUT);

	init(0, CS, &gyroScale, &accelScale);

}

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

void send_config(int SPIChannel, int CS, uint8_t *addr, uint8_t *val)
{
    digitalWrite(CS, 0);

    wiringPiSPIDataRW(SPIChannel, addr, 1);
    wiringPiSPIDataRW(SPIChannel, val, 1);

    digitalWrite(CS, 1);
}

