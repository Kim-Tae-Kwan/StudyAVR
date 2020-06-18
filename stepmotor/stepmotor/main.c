/*
 * stepmotor.c
 *
 * Created: 2020-06-18 오전 9:31:08
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

unsigned char step[]={0x90,0x80,0xC0,0x40,0x60,0x20,0x30,0x10};
int main(void)
{
	DDRD=0xF0;
    int t=0;
	while (1) 
    {
		for(int i=0;i<12;i++)
		{
			PORTD=step[t];
			t++;
			if(t>7) t=0;
			_delay_ms(10);
		}
		_delay_ms(1000);
    }
	return 0;
}

