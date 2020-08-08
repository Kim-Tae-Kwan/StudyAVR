/*
 * Serial_Terminal.c
 *
 * Created: 2020-08-08 오후 4:35:46
 * Author : PKNU
 */ 

#include <avr/io.h>


int main(void)
{
	void putch(unsigned char data)
	{
		while((UCSR0A & 0x20)==0);
		UDR0=data;
		UCSR0A |=0x20;
	}

	unsigned char getch()
	{
		unsigned char data;
		while((UCSR0A & 0x80)==0);
		data=UDR0;
		UCSR0A |= 0x80;
		return data;
	}
	
	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x06;
	
	UBRR0H=0x00;
	UBRR0L=0x03;
	
	
    while (1) 
    {
		
		
    }
}

