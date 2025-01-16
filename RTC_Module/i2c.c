#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

#define F_CPU 8000000UL
#define i2c_SCL 100000UL

void init(void){
    
    TWSR = 0x00;  // Prescaler set to 1
    TWBR = (F_CPU / SCL_CLOCK - 16) / 2;// Set SCL frequency NOT AC
    TWCR = (1 << TWEN);
}


void i2c_start(uint8_t address){
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));

    // Send device address
    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));

}

void i2c_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));

}

uint8_t i2c_read_ack(void){
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;

}

uint8_t i2c_read_nack(void){
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void i2c_stop(void){
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    while (TWCR & (1 << TWSTO));
}

#endif