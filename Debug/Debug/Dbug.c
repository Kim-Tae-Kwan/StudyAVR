/*
 * Dbug.c
 *
 * Created: 2020-06-15 오후 12:23:04
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>

unsigned char A = 0xAA;
unsigned char B = 0xBB;
unsigned char C = 0xCC;
unsigned char *ADD = 0x130;

int main()
{
	DDRA = 0xFF;
	DDRF = 0xFF;
	
	ADD = 0x140;
	*ADD = 0x11;
	*(ADD+1)=0x22;
	*(ADD+2)=0x33;
	*(ADD+4)=0x44;
	
	A = 0xBB;
	B=C-A;
	
	PORTA = A;
	PORTB = B;
	
	while(1)
	{
		
		
	}

	
	return 0;
}