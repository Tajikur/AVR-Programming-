#ifndef I2C_H
#define I2C_H

#define i2c_SCL 100000UL
#define F_CPU 8000000UL



#include <avr/io.h>


void i2c_init(void);

void i2c_start(uint8_t address);
void i2c_write(uint8_t data);

uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

void i2c_stop(void);

#endif