/*
 * LCD.c
 *
 * Created: 2020-08-08 오후 2:13:14
 * Author : PKNU
 */ 

#include <avr/io.h>
#include "Lcd.h"
#include <util/delay.h>
#include <stdio.h>

#define Keypadin PIND

unsigned char KeyPad()
{
	PORTD = 0xFE;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEE); return '1'; } // 1110 1110 & 1111 0000 = 1110 0000
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDE); return '4';} //1101 1111
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBE); return '7';} // 1011 1111
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7E); return '*';}
	
	PORTD = 0xFD;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xED); return '2';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDD); return '5';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBD); return '8';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7D); return '0';}
	
	PORTD = 0xFB;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEB); return '3';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDB); return '6';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBB); return '9';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7B); return '#';}
	
	return 'x';
}




int main(void)
{
	Byte str[] = "IoT BigData";
	Byte str1[]="Hello!!";
	Byte str2[]="Key Pad Number";
	Byte str3[13];
	
	
	DDRA = 0xFF;
	DDRD = 0x07;
	PORTD = 0xF0;
	
	unsigned char temp;
		
	LcdInit_4bit();
	Lcd_Pos(0,0);
	Lcd_STR(str);
	Lcd_Pos(1,0);
	Lcd_STR(str1);
	
    while (1) 
    {
		temp=KeyPad();
		if(temp=='x') continue;
		
		Lcd_Clear();
		Lcd_Pos(0,0);
		Lcd_STR(str2);
		Lcd_Pos(1,0);
		sprintf(str3,"%c",temp);
		Lcd_STR(str3);
		
		
		
    }
}
