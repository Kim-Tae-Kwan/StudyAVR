/*
 * PortTest.c
 *
 * Created: 2020-06-12 오후 2:10:26
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char*argv[])
{	
	DDRA=0xFF; //PORT A 출력설정
	DDRB=0xFF; //PORT A 출력설정
	DDRC=0xFF; //PORT A 출력설정
	DDRD=0xFF; //PORT A 출력설정
	DDRE=0xFF; //PORT A 출력설정
	DDRF=0xFF; //PORT A 출력설정
	DDRG=0x1F; //PORT A 출력설정
	while(1)
	{
		PORTA=0xFF;
		PORTB=0xFF;
		PORTC=0xFF;
		PORTD=0xFF;
		PORTE=0xFF;
		PORTF=0xFF;
		PORTG=0x1F;
		_delay_ms(1000);
		PORTG=0x00;
		_delay_ms(1000);
		
				
	}
	
		
	return 0;
}
