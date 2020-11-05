/*
 * Description:    i2c lib
 * Name:           i2cmaster.h
 * Reference:      www.jump.to/fleury
 * Created:        16.1.2018
 * Author:         Ali Gholami
 * License:        Open-source 
 * Core:           8 bit ATMEL_MiCROCHIP
 * Last update:    16.1.2018
 * Test Des:       OK for atmega328p,8mhz
 * Website:        https://atmels.wordpress.com/
 */ 

#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include<avr/io.h>

 void          i2c_init(void);
 void          i2c_stop(void);
 unsigned char i2c_start_addr(unsigned char addr);
 unsigned char i2c_write(unsigned char data);
 unsigned char i2c_readAck(void);
 unsigned char i2c_readNack(void);
 void          i2c_start();

#endif /* I2CMASTER_H_ */