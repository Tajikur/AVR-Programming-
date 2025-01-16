#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

//Base resister for RTC 0x68
#define RTC_WRITE 0xD0 //After calculation 
#define RTC_READ 0xD1 //After calculation
//Base resister for LM75 0x48
#define LM75_WRITE 0x90 //After calculation for write mode 0
#define LM75_READ  0x91 //After calculation for read mode 1

void init_led(){
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
}
void RED_LED_ON(){
    PORTB |= (1 << PB0);

}
void BLUE_LED_ON(){
    PORTB |= (1 << PB1);
    
}
void RED_LED_OFF(){
    PORTB &= ~(1 << PB0);
    
}
void BLUE_LED_OFF(){
    PORTB &= ~(1 << PB1);
    
}

void set_time(uint8_t hour , uint8_t mint , uint8_t sec){
    i2c_start(RTC_WRITE);
    i2c_write(0x00); // Start from register 0x00 (Seconds)
    i2c_write(((sec / 10) << 4) | (sec % 10));
    i2c_write(((mint / 10) << 4) | (mint % 10));
    i2c_write(((hour / 10) << 4) | (hour % 10));
    i2c_stop();
}

void get_time(uint8_t *hour, uint8_t *mint, uint8_t *sec){
    i2c_start(RTC_WRITE);
    i2c_write(0x00);
    i2c_stop();

    i2c_start(RTC_READ);
    *sec = i2c_read_ack();
    *mint = i2c_read_ack();
    *hour = i2c_read_nack();
    i2c_stop();

    *sec = ((*sec >> 4) * 10) + (*sec & 0x0F);
    *mint = ((*mint >> 4) * 10) + (*mint & 0x0F);
    *hour = ((*hour >> 4) * 10) + (*hour & 0x0F);

}

float get_temperature_data (void){
    i2c_start(LM75_WRITE);
    i2c_write(0x00); //For temperature
    i2c_stop();

    i2c_start(LM75_READ);
    uint8_t msb = i2c_read_ack();
	uint8_t lsb = i2c_read_nack();
	i2c_stop();

    int16_t temp_sensor = (msb<< 8 )| lsb;
	temp_sensor >>=7;
	return temp_sensor*0.5;
}

int main(void){

    uint8_t hours, minutes, seconds;

    i2c_init();
    init_led();
    set_time(10, 10, 0);

    

    while(1){
        get_time(&hours, &minutes , &seconds);
        if(hours==10 && minutes==0 && seconds ==0){
            float temperature = get_temperature_data();
            
            if(temperature > 30.0){
                RED_LED_ON();
                BLUE_LED_OFF();
            }
            else{
                RED_LED_OFF();
                BLUE_LED_ON();

            }
        }
        else{
            RED_LED_OFF();
            BLUE_LED_OFF();
        }

        _delay_ms(1000);


    }
}