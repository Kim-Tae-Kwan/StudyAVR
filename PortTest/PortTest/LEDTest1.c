/*
 * LEDTest1.c
 *
 * Created: 2020-06-12 오후 3:39:32
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>
#define speed 10

unsigned char Shift(unsigned char LED_DATA)
{
	
	PORTC=LED_DATA;
	_delay_ms(speed);
	if(LED_DATA==0xFF)
	{
		for(int i=0;i<7;i++){
			PORTC=0xFF;
			_delay_ms(speed);
			PORTC=0x00;
			_delay_ms(speed);
		}
		while(1)
		{
			LED_DATA=LED_DATA>>1;
			PORTC=LED_DATA;
			_delay_ms(speed);
			if(LED_DATA==0x00)
			{
				LED_DATA=0x01;
				break;
			}
		}
	}
	else{ LED_DATA=(LED_DATA<<1) + 1;}
	
	return LED_DATA;
}

int main()
{	
	unsigned char LED_DATA=0x01;
	DDRC=0xFF;
	
	while(1)
	{
		
		LED_DATA=Shift(LED_DATA);
		
	}
	return 0;	
}