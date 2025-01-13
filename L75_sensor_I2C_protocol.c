/*
 * Created: 1/12/2025 11:54:58 PM
 * Author : Tajikur Rahman Turjo
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define SENSOR_ADDRESS 0x48
#define RED_LED PB0
#define BLUE_LED PB1

void Init_i2c(){
	TWSR &= ~(1<<TWPS0) & ~ (1<<TWPS1);
	TWBR =0X48;
	TWCR |=(1<<TWEN);	
	
}
void start_i2c(){
	TWCR |=(1<<TWSTA)|(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));	
}
void stop_i2c(){
	TWCR |= (1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
	_delay_us(10);	
}
void write_i2c(uint16_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
}
uint16_t read_i2c(uint8_t ack){
	TWCR =(1<<TWINT)|(1<<TWEN)|(ack<<TWEA);
	while(!(TWCR &(1<<TWINT)));
	return TWDR;	
}
float Read_temp(){
	start_i2c();
	write_i2c(SENSOR_ADDRESS<<1|0);
	write_i2c(0x00);
	start_i2c();
	write_i2c(SENSOR_ADDRESS<<1|1);
	uint8_t msb = read_i2c(1);
	uint8_t lsb = read_i2c(0);
	stop_i2c();
	
	int16_t temp_sensor = (msb<< 8 )| lsb;
	temp_sensor >>=7;
	return temp_sensor*0.5;
	
	
}


int main(void)
{
    Init_i2c();
	DDRB |= (1<<RED_LED)|(1<<BLUE_LED);
    while (1) 
    {
		float temperature = Read_temp();
		
		if(temperature > 25.0){
			PORTB |=(1<<RED_LED);
			PORTB &= ~(1<<BLUE_LED);
		}
		else{
			PORTB |=(1<<BLUE_LED);
			PORTB &= ~(1<<RED_LED);
		}
		_delay_ms(20000);
	}
}