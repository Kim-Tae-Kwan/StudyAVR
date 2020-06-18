/*
 * Timer_Counter_External_Interrupt.c
 *
 * Created: 2020-06-16 오전 10:17:22
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};
volatile unsigned char timer0Cnt=0;	
unsigned char start_stop=1;
unsigned char cnt=0;
	
int main()
{
	DDRA = 0xFF;
	DDRE = 0x00;
	TCCR2= 0x05;
	OCR2=72;
	TIMSK=0x40;
	TIFR |= 1<<OCF2;
	
	EICRB=0x03;
	EIMSK=0x10;
	EIFR=0x10;
	
	
	
	sei();
	
	
	
	while(1)
	{
		//if(start_stop==1)
		//{
			PORTA = FND_DATA_TBL[cnt];
		//}
		//else continue;
	}
	
	
	return 0;
}

SIGNAL(TIMER2_OVF_vect)
{
	cli();
	OCR2+=72;
	timer0Cnt++;
	if(timer0Cnt==100)
	{
		if(start_stop==1)
		{
			if(cnt>9) cnt=0;
			else cnt++;
		}
		timer0Cnt=0;
	}
	sei();
	
}
SIGNAL(INT4_vect)
{
	cli();
	if(start_stop==0) start_stop=1;
	else  start_stop=0;
	
	sei();
	
	
}