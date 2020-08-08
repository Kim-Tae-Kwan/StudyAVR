/*
 * KeyPad.c
 *
 * Created: 2020-08-08 오전 11:25:45
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define Keypadin PIND

unsigned char KeyPad()
{
	PORTD = 0xFE;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEE); return 1; } // 1110 1110 & 1111 0000 = 1110 0000
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDE); return 4;} //1101 1111
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBE); return 7;} // 1011 1111
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7E); return '*';}
	
	PORTD = 0xFD;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xED); return 2;}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDD); return 5;}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBD); return 8;}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7D); return 0;}
	
	PORTD = 0xFB;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEB); return 3;}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDB); return 6;}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBB); return 9;}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7B); return '#';}
		
	return 'x';
}

int main(void)
{
	DDRA = 0xFF;
	PORTA = 0xFF;
	
	DDRC = 0xFF;
	PORTC = 0xFF;
	
	//Motor 
	DDRF = 0x01;
	PORTF = 0x01;
	//KeyPad
	DDRD = 0x07;
	PORTD = 0xF0;
	
	unsigned char Password[] = {1,2,3,4};
	unsigned char temps[4];
	unsigned char temp;
	int i=0;
    /* Replace with your application code */
    while (1) 
    {
		temp = KeyPad();
		if(temp =='x') continue;
		else if(temp == '*') 
		{
			for (int j=0;j<4;j++)
			{
				temps[j]=0;
			}	
			continue;
		}
		
		
		
		temps[i++]=temp;
		
		if(i==4)
		{
			for (int j=0;j<4;j++)
			{
				if(Password[j] != temps[j])
				{
					PORTF = 0x01;
					break;
				}
				if(j==3)
				{
					PORTF = 0x00;
					_delay_ms(300);
					PORTF = 0x01;
				}
			}
			i=0;
		}
			
    }
}

