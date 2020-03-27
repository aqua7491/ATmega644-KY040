#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


#include "LCD_1602A.h"

lcd_t lcd;

volatile uint8_t counter = 0;

volatile uint8_t previous_pins = 0;
	
ISR(PCINT0_vect)
{
	uint8_t read = PINA;
	if((read ^ previous_pins) & 0b00000001 ){
		counter = 0;	
	}else if((read ^ previous_pins) & 0b00000010 && (read & 0b00000010)){
		if(read & 0b00000100){
			counter--;
		}else{
			counter++;
		}	
	}
	
	LCD_1602A_clear(&lcd);
	LCD_1602A_print_char(&lcd, counter);
	previous_pins = read;
	_delay_ms(5);
}	

int main(void)
{	
	lcd.Controll_ddr = &DDRC;
	lcd.Controll_pin = &PINC;
	lcd.Controll_port = &PORTC;
	lcd.RS = 0b00000001;
	lcd.RW = 0b00000010;
	lcd.E  = 0b01000000;
	
	lcd.DB_ddr = &DDRD;
	lcd.DB_pin = &PIND;
	lcd.DB_port = &PORTD;
	lcd.data_pins = 0b11111111;
	
	LCD_1602A_init(&lcd);
	
	
	//setup intterupt on pin A0
	DDRA &= ~0b00000011;
	PORTA &= ~0b00000011;
	
	sei(); 
	PCICR  = 0b00000001;
	PCMSK0 = 0b00000011;

	LCD_1602A_print_int(&lcd, counter);
	while (1)
	{
		_delay_ms(1000);
	}
	
	
}