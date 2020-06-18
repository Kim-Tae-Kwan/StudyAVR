/*
 * UART_ECHO.c
 *
 * Created: 2020-06-17 오후 4:07:28
 *  Author: PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

unsigned int DoReMi[8]={261,587,659,698,783,880,987,1046};

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
void piano(unsigned char data)
{
	if(data=='1')
	{
		ICR3 = 32;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='2')
	{
		ICR3 = 65;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='3')
	{
		ICR3 = 130;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='4')
	{
		ICR3 = 261;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='5')
	{2
		ICR3 = 523;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='6')
	{
		ICR3 = 1046;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	if(data=='7')
	{
		ICR3 = 7372800/DoReMi[6]/2;
		TCCR3A = 0x40;
		_delay_ms(1000);
		TCCR3A=0x00;
	}
	
}

int main()
{
	unsigned char text[]="\r\nWelcome! edgeiLAB\r\n USART 0 TEST PROGRAM. \r\n";
	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;
	unsigned char data=0;
	
	DDRE = 0xfe;
	
	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x06;
	
	UBRR0H=0x00;
	UBRR0L=0x03;
	
	TCCR3A = 0x00;
	TCCR3B = 0x19;
	TCCR3C = 0x00;
	TCNT3 =0x00;
	
	while(text[i]!='\0')
	{
		putch(text[i++]);
	}
	
	i=0;
	while(echo[i]!='\0')
	{
		putch(echo[i++]);
	}
	while(1)
	{
		//putch(getch());
		
		data=getch();
		piano(data);
		
		
			
	}
	
	
	
	return 0;
}