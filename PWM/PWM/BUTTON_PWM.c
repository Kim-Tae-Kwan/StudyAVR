/*
 * BUTTON_PWM.c
 *
 * Created: 2020-06-17 오전 11:37:01
 *  Author: PKNU
 */
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int Light=0; 
volatile unsigned char Light_flag=1;

int main()
{
	DDRB = 0x80;
	DDRD = 0x00;
	DDRE = 0x00;
	
	
	TCCR2=0x65;
	TCNT2=0;
	
	EICRA=0xFF;
	EICRB = 0xFF;
	EIMSK = 0xFF;
	EIFR = 0xFF;
	
	sei();
	
	
	
	while(1)
	{
		if(Light_flag)
		{
			OCR2=Light;
			Light_flag=0;		
		}
	}
	
	
	
	return 0;
}

SIGNAL(INT0_vect)
{
	cli();
	Light=0;
	Light_flag=1;
	sei();
}
SIGNAL(INT1_vect)
{
	cli();
	Light-=10;
	if(Light<0) Light=0;
	Light_flag=1;
	
	sei();
}
SIGNAL(INT2_vect)
{
	cli();
	
	Light-=25;
	if(Light<0) Light=0;
	Light_flag=1;
	sei();
}
SIGNAL(INT3_vect)
{
	cli();
	Light-=51;
	if(Light<0) Light=0;
	Light_flag=1;
	
	sei();
}

SIGNAL(INT4_vect)
{
	cli();
	Light+=10;
	if(Light>255) Light=255;
	Light_flag=1;
	
	sei();
}
SIGNAL(INT5_vect)
{
	cli();
	Light+=25;
	if(Light>255) Light=255;
	Light_flag=1;
	
	sei();
}
SIGNAL(INT6_vect)
{
	cli();
	Light+=51;
	if(Light>255) Light=255;
	Light_flag=1;
	
	sei();
}
SIGNAL(INT7_vect)
{
	cli();
	Light=255;
	Light_flag=1;
	sei();
}