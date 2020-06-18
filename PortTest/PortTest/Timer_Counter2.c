/*
	
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data=0x01;
unsigned char timer2Cnt=0,Shift_Flag = 0;

SIGNAL(TIMER2_OVF_vect);

int main()
{	
	DDRA = 0xFF;
	
	TCCR2=0x05;
	TCNT0 = 0xFF-72;
	TIMSK = 0x40;
	TIFR |=1<<TOV2;
	
	sei();
	
	while(1)
	{
		PORTA = LED_Data;
	}
	
	return 0;
}
SIGNAL(TIMER2_OVF_vect)
{
	cli();
	TCNT2 = 0xFF-72;
	timer2Cnt++;
	if(timer2Cnt == 50)
	{
		if(Shift_Flag==0)
		{
			LED_Data <<=1;
			if(LED_Data==0x80) Shift_Flag = 1;
		}
		else
		{
			LED_Data>>=1;
			if(LED_Data==0x01) Shift_Flag=0;
		}
		timer2Cnt=0;
	}
	sei();
}
