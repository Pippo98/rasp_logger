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

#define CS 3

int main(){
	
	wiringPiSetup();

	spi = wiringPiSPISetup(0, 9000000);

	pinMode(CS, OUTPUT);

	init(0, CS, &gyroScale, &accelScale);

}
