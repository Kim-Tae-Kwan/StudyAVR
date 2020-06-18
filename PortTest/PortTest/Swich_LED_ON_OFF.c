/*
 * LED_LEFT_RIGHT.c
 *
 * Created: 2020-06-15 오전 9:38:13
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main()
{	
	DDRA=0xFF;
	DDRD=0x00;
	
	while(1)
	{	
		
		
		if(PIND==0x01) PORTA=0x01;
		else if(PIND==0x02) PORTA=0x02;
		else if(PIND==0x04) PORTA=0x04;
		else if(PIND==0x08) PORTA=0x08;
		else if(PIND==0x10) PORTA=0x10;
		else if(PIND==0x20) PORTA=0x20;
		else if(PIND==0x40) PORTA=0x40;
		else if(PIND==0x80) PORTA=0x80;
		else continue;
		
		
	
		
	}
	
	return 0;
}