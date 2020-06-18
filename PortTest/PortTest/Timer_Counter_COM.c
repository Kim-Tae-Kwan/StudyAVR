/*
 * Timer_Counter_COM.c
 *
 * Created: 2020-06-16 오전 9:47:19
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};
volatile unsigned char time_s=0;
volatile unsigned char time_s10=0;
unsigned char timer0Cnt=0;


int main()
{	
	DDRA = 0xFF;
	DDRD = 0xFF;
	
	TCCR0 = 0x07;
	OCR0 = 72;
	TIMSK = 0x02;
	TIFR |=1<<OCF0;
	sei();
	while(1)
	{
		PORTA = FND_DATA_TBL[time_s10];
		PORTD = FND_DATA_TBL[time_s];
	}
	
	
	return 0;
}

SIGNAL(TIMER0_COMP_vect)
{
	cli();
	OCR0 +=72;
	timer0Cnt++;
	if(timer0Cnt==100)
	{
		time_s++;
		if(time_s>9) 
		{
			time_s=0;
			time_s10++;
			if(time_s10>9) time_s10=0;
		}
		timer0Cnt=0;
	}
	sei();
}