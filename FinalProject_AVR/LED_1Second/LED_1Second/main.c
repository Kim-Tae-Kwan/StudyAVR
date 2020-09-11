/*
 * LED_1Second.c
 *
 * Created: 2020-09-07 오후 3:56:46
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define  ON 1
#define  OFF 0


unsigned char On_Off=OFF;
unsigned char timer0Cnt=0;

SIGNAL(TIMER0_COMP_vect)
{
	cli();
	
	if(++timer0Cnt==250)
	{
		timer0Cnt=0;
		if(On_Off==OFF)
		{
			PORTA =0x01; //LED On
			On_Off=ON;
		}
		else
		{
			PORTA =0x00; //LED Off
			On_Off=OFF;
		}
		
	}
	sei();
}


int main(void)
{
	DDRA = 0x03; //PA0,1 출력 설정
	
	TCCR0 = 0x06; //분주비 256
	TCNT0=0x00;	  //카운터 초기값 0
	OCR0 = 250;   //비교 값 256
	TIMSK=0x02;   //비교 인터럽트 enable
	sei();        //전역 인터럽트 시작.
	while (1)
	{
	}
}

