#ifndef _LSM9DS0_H
#define _LSM9DS0_H

void send_config(int SPIChannel, int CS, uint8_t *addr, uint8_t *val);
void init(int SPIChannel, int CS, int *gyro_scale, int *accel_scale);


#endif
