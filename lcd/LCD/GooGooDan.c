/*
 * LCD.c
 *
 * Created: 2020-06-16 오후 1:51:21
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Lcd.h"

void lcd_display(unsigned char*str,unsigned char*str2)
{
	Lcd_Pos(0,0);
	_delay_ms(10);
	Lcd_STR(str);
	_delay_ms(10);
	
	Lcd_Pos(1,0);
	_delay_ms(10);
	Lcd_STR(str2);
	_delay_ms(10);
}

void goo_print(int i)
{
	Byte buf[16];
	Byte goo[16];
	sprintf(goo,"%d Dan : ",i);
	
	Lcd_Clear();
	Lcd_Pos(0,0);
	Lcd_STR(goo);
	
	for(int k=1;k<10;k++)
	{
		Lcd_Pos(1,0);
		sprintf(buf,"%d x %d = %d",i,k,i*k);
		Lcd_STR(buf);
		_delay_ms(500);
	}
	_delay_ms(500);
}

int main(void)
{
	Byte str[]="Goo Goo Dan";
	Byte str2[]="INSERT NUMBER";
	int num=0;
	
	
	DDRA = 0xFF;
	DDRD = 0x00;
	
	LcdInit_4bit();
	
    while (1) 
    {
		if(PIND==0x01) num=1;
		else if(PIND==0x02) num=2;
		else if(PIND==0x04) num=3;
		else if(PIND==0x08) num=4;
		else if(PIND==0x10) num=5;
		else if(PIND==0x20) num=6;
		else if(PIND==0x40) num=7;
		else if(PIND==0x80) num=8;
		else num=0;
		
		switch(num){
			case 0:
				lcd_display(str,str2);
				break;
			
			case 1:
				goo_print(2);
				break;
			case 2:
				goo_print(3);
				break;
			case 3:
				goo_print(4);
				break;
			case 4:
				goo_print(5);
				break;
			case 5:
				goo_print(6);
				break;
			case 6:
				goo_print(7);
				break;
			case 7:
				goo_print(8);
				break;
			case 8:
				goo_print(9);
				break;
				
			
		}
		
		
    }
	return 0;
}

