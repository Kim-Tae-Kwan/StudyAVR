/*
 * TImer_Counter3.c
 *
 * Created: 2020-06-16 오전 9:21:29
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_DATA=0;
unsigned int timer0Cnt=0;

SIGNAL(TIMER0_OVF_vect);

int main()
{
	DDRA=0xFF;
	TCCR0=0x06;
	TCNT0=112;
	TIMSK=0x01;
	TIFR |=1<<TOV0;
	sei();
	while(1)
	{
		PORTA=LED_DATA;
	}
	return 0;
}

SIGNAL(TIMER0_OVF_vect)
{
	cli();
	TCNT0=112;
	timer0Cnt++;
	if(timer0Cnt==200)
	{
		LED_DATA++;
		if(LED_DATA>0xFF) LED_DATA=0;
		timer0Cnt=0;
	}
	sei();
}