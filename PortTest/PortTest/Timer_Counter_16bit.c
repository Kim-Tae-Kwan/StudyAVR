/*
 * Timer_Counter_16bit.c
 *
 * Created: 2020-06-16 오전 11:36:22
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned char LED_Data=0x00;
unsigned char timer1Cnt=0;

int main()
{
	DDRA=0xFF;
	
	TCCR3A=0x00;
	TCCR3B= 0x04;
	
	TCNT3 = 36736;
	ETIMSK = 0x04;
	ETIFR|=1<<TOV3;
	sei();
	
	while(1)
	{
		PORTA = LED_Data;
	}
	
	
	return 0;
}
SIGNAL(TIMER3_OVF_vect)
{
	cli();
	TCNT3=36736;
	
	LED_Data++;
	if(LED_Data>0xFF) LED_Data=0;
	sei();
}