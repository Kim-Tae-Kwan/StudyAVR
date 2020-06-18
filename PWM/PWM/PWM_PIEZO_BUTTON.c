/*
 * PWM_PIEZO_BUTTON.c
 *
 * Created: 2020-06-17 오후 1:51:07
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int DoReMi[8]={261,5870,6590,6980,7830,8800,9870,10460};
volatile unsigned char sound_flag=1;

int main()
{
	DDRE = 0x08;
	DDRD = 0x00;
	
	//fast PWM 설정
	TCCR3A = 0x00;
	TCCR3B = 0x19;
	TCCR3C = 0x00;
	TCNT3 =0x00;
	
	//외부 인터럽트 설정
	EICRA=0xFF;
	EIMSK=0x0F;
	EIFR=0x0F;
	sei();
	
	
	while(1)
	{
		if(sound_flag)
		{
			_delay_ms(500);
			TCCR3A=0x00;
			sound_flag=0;
		}	
	}
	
	return 0;
}
SIGNAL(INT0_vect)
{
	cli();
	ICR3 = 7372800/DoReMi[0]/2;
	TCCR3A = 0x40;
	sound_flag=1;
	sei();
}
SIGNAL(INT1_vect)
{
	cli();
	ICR3 = 7372800/DoReMi[1]/2;
	TCCR3A = 0x40;
	sound_flag=1;
	sei();
}
SIGNAL(INT2_vect)
{
	cli();
	ICR3 = 7372800/DoReMi[2]/2;
	TCCR3A = 0x40;
	sound_flag=1;
	sei();
}
SIGNAL(INT3_vect)
{
	cli();
	ICR3 = 7372800/DoReMi[3]/2;
	TCCR3A = 0x40;
	sound_flag=1;
	sei();
}