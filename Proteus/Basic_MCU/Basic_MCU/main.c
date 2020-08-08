/*
 * Basic_MCU.c
 *
 * Created: 2020-08-08 오전 9:29:16
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRC = 0xFF; // 0b1111_1111
	//PORTC = 0xFE; // 0b1111_0000
	
	DDRD = 0xF0;
	PORTD = 0x0F;
	char temp = 0xFE;
	int chage = 0;
	
    while (1) 
    {
		
		//if(PIND == 0x0E) PORTC = 0xF1;
		//else if(PIND == 0x0D) PORTC = 0xF2;
		//else if(PIND == 0x0B) PORTC = 0xF4;
		//else if(PIND == 0x07) PORTC = 0xF8;
		
		PORTC = temp;
		if(temp == 0x7F)
			chage=1;
		else if (temp==0xFE)
		{
			chage=0;
		}
		
		if(chage==0)
			temp = (temp <<1)+1;
		else if (chage==1)
		{
			temp = (temp >>1) | 0x80 ;
		}
		_delay_ms(1000);
		
    }
}

