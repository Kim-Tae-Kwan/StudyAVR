/*
 * ICN_FND.c
 *
 * Created: 2020-06-17 오전 9:42:42
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};
volatile unsigned char time_s=0;
volatile unsigned char FND_flag=0,edge_flag=0;


int main()
{
	DDRA=0xFF;
	DDRE=0x00;
	DDRF=0xFF;
	
	TCCR3A=0x00;
	TCCR3B=0x45;
	ETIMSK=0x020;
	ETIFR|=1<<ICF3;
	
	sei();
	PORTA= FND_DATA_TBL[0];
	while(1)
	{
			
			if(FND_flag)
			{
				if(time_s>15)
					time_s=15;
				
				PORTA= FND_DATA_TBL[time_s];
				FND_flag=0;
			}
			else PORTF=FND_DATA_TBL[ICR3/7200];
	}
	
	
	return 0;
}
SIGNAL(TIMER3_CAPT_vect)
{
	cli();
	
	unsigned int count_check;
	if(edge_flag==0)
	{
		TCCR3B=0x05;
		TCNT3=0;
		ICR3 =0;
		edge_flag=1;
	}
	else
	{
		TCCR3B=0x45;
		count_check=ICR3;
		time_s=count_check/7200;
		FND_flag = 1;
		edge_flag=0;
	}
	sei();
	
}