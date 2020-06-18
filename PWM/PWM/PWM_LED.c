/*
 * PWM.c
 *
 * Created: 2020-06-17 오전 11:21:16
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void)
{	unsigned char Light=0;
	
	DDRB=0x10;
	
	TCCR0= 0x77; //PC PWM 모드, 0101 0101
	TCNT0=0x00;
	
    while (1) 
    {
		for(Light=0;Light<255;Light++)
		{
			OCR0=Light;
			_delay_ms(5);
			if(Light==254) _delay_ms(1000);
		}
		for(Light=255;Light>0;Light--)
		{
			OCR0=Light;
			_delay_ms(5);
			if(Light==0) _delay_ms(1000);
		}
    }
}

