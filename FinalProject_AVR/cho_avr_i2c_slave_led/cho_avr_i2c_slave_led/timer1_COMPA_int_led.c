// Timer1�� Compare Match A Interrupt�� �̿��Ͽ� ������ �ֱ��� Interrupt(Clock)�� �߻���Ű��,
// Interrupt Service Routine���� �ʿ��� ����� ���� �Ѵ�.

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
	// Clock �ֱ� : 16M / 1024 = 64uSec.
	TCCR1B = (0 << WGM13) | (0 << WGM12) | (1 << CS12) | (0 << CS11) | 
	(1 << CS10);    
	TCNT1 = 0x00;	// Timer/Counter1 �ʱ�ȭ

	// 0.5 Sec �ֱ�� Compare Match Interrupt�� �߻� ��Ű�� ���Ͽ���
	// TCNT1 Rg �� ���� ������ INTERRUPT_PERIOD ��ŭ ū ��
	// (TCNT1 + INTERRUPT_PERIOD)���� OCR1A Rg�� ���� �Ͽ��� �Ѵ�.
	// OCR1A =  TCNT1 + INTERRUPT_PERIOD(7814) ���� ���� �Ѵ�. 
	OCR1A = TCNT1 + INTERRUPT_PERIOD; 
	// Timer1 Clear Compare Match A Interrupt Flag
	TIFR |= (1 << OCF1A);
	// Timer1 Compare Match A Interrupt Enable
	TIMSK |= (1 << OCIE1A);	
}

// Timer/Counter1 Compare Match A Interrupt Service Routine
SIGNAL (TIMER1_COMPA_vect)
{ 
	// 0.5Sec �Ŀ� Interrupt �� �߻� �ϵ��� OCR11 ���� ���� �Ѵ�.
	OCR1A = TCNT1 + INTERRUPT_PERIOD; 
	TIMSK &= ~(1 << OCIE1A); // Compare Match A Interrupt Disable

	// Left shift command�� ���� �Ѵ�.
	if(ledOperationMode == SHIFT_LEFT){
		if(LEDPORT == 0x00)LEDPORT = 0x01;
		else LEDPORT = (LEDPORT << 1);
	}
	// Right shift command�� ���� �Ѵ�.
	if(ledOperationMode == SHIFT_RIGHT){
		if(LEDPORT == 0x0)LEDPORT = 0x80;
		else LEDPORT = (LEDPORT >> 1);
		
	}
	// Blinking command�� ���� �Ѵ�.
	if(ledOperationMode == BLINKING){
		// LED�� ���� On ���� �̸� Off, Off ���� �̸� On ���·� �Ѵ�.
		PORTF = ~PORTF;	
	}

	TIMSK|= (1 << OCIE1A);   // Compare Match A Interrupt Enable
}
