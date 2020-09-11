// Timer1의 Compare Match A Interrupt를 이용하여 일정한 주기의 Interrupt(Clock)을 발생시키고,
// Interrupt Service Routine에서 필요한 명령을 실행 한다.

#include <stdio.h>
#include "timer1_COMPA_int_led.h"

#define LEDPORT PORTF
#define INTERRUPT_PERIOD 7814

char ledOperationMode;

void timer1_init(void)
{
	// (0 << COM1A1) | (0 << COM1A0) : Normal port operation, 
	//   OCnA Disconnected.
	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << WGM11) | (0 << WGM10);  
	// (0 << WGM13) | (0 << WGM12)| (0 << WGM11) | (0 << WGM10) : Normal,
	//   Top: 0xffff
	// (1 << CS12) | (0 << CS11) | (1 << CS10) : 010  prescale clk/1024,
	// Clock 주기 : 16M / 1024 = 64uSec.
	TCCR1B = (0 << WGM13) | (0 << WGM12) | (1 << CS12) | (0 << CS11) | 
	(1 << CS10);    
	TCNT1 = 0x00;	// Timer/Counter1 초기화

	// 0.5 Sec 주기로 Compare Match Interrupt를 발생 시키기 위하여는
	// TCNT1 Rg 의 현재 값보다 INTERRUPT_PERIOD 만큼 큰 값
	// (TCNT1 + INTERRUPT_PERIOD)으로 OCR1A Rg를 설정 하여야 한다.
	// OCR1A =  TCNT1 + INTERRUPT_PERIOD(7814) 으로 설정 한다. 
	OCR1A = TCNT1 + INTERRUPT_PERIOD; 
	// Timer1 Clear Compare Match A Interrupt Flag
	TIFR |= (1 << OCF1A);
	// Timer1 Compare Match A Interrupt Enable
	TIMSK |= (1 << OCIE1A);	
}

// Timer/Counter1 Compare Match A Interrupt Service Routine
SIGNAL (TIMER1_COMPA_vect)
{ 
	// 0.5Sec 후에 Interrupt 가 발생 하도록 OCR11 값을 설정 한다.
	OCR1A = TCNT1 + INTERRUPT_PERIOD; 
	TIMSK &= ~(1 << OCIE1A); // Compare Match A Interrupt Disable

	// Left shift command를 실행 한다.
	if(ledOperationMode == SHIFT_LEFT){
		if(LEDPORT == 0x00)LEDPORT = 0x01;
		else LEDPORT = (LEDPORT << 1);
	}
	// Right shift command를 실행 한다.
	if(ledOperationMode == SHIFT_RIGHT){
		if(LEDPORT == 0x0)LEDPORT = 0x80;
		else LEDPORT = (LEDPORT >> 1);
		
	}
	// Blinking command를 실행 한다.
	if(ledOperationMode == BLINKING){
		// LED를 현재 On 상태 이면 Off, Off 상태 이면 On 상태로 한다.
		PORTF = ~PORTF;	
	}

	TIMSK|= (1 << OCIE1A);   // Compare Match A Interrupt Enable
}
