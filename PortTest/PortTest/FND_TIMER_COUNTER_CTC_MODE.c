/*
 * FND_TIMER_COUNTER_CTC_MODE.c
 *
 * Created: 2020-06-17 오전 9:16:08
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};
volatile unsigned char cnt=0;
	
int main()
{
	DDRA=0xFF;
	TCCR1A=0x00;
	TCCR1B=0x0D;
	OCR1A=7200;
	TIMSK=0x10;
	TIFR=0x10;
	sei();
	
	
	while(1)
	{
		PORTA=FND_DATA_TBL[cnt];
	}
	
	
	return 0;
}
SIGNAL(TIMER1_COMPA_vect)
{
	cli();
	if(cnt>=9) cnt=0;
	else cnt++;
	sei();
}