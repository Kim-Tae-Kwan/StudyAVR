/*
 * Interrupt_LED.c
 *
 * Created: 2020-06-15 오후 2:13:05
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F,0x77,0x7c,0x39,0x5e,0x79,0x71};
int cnt=0;
int RESET=0;

int main()
{
	DDRA =0xFF;
	DDRD = 0x00;
	EICRA=0x33;
	EIMSK=0x09;
	EIFR = 0x09;
	sei();
	
	while(1)
	{
		PORTA=FND_DATA_TBL[cnt];
		/*
		if(RESET==1)
		{
			cnt=0;
		}
		else if(RESET==0)
		{
			if(cnt>16) cnt=0;
			else cnt++;
		}
		_delay_ms(300);
		*/
	}
	
	
	return 0;
}
/*
SIGNAL(INT0_vect)
{
	cli();
	if(RESET==0) RESET=1;
	sei();
}
SIGNAL(INT3_vect)
{
	cli();
	if(RESET==1) RESET=0;
	sei();
	
}
*/