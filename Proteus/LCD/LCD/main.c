/*
 * LCD.c
 *
 * Created: 2020-08-08 오후 2:13:14
 * Author : PKNU
 */ 

#include <avr/io.h>
#include "Lcd.h"


int main(void)
{
	Byte str[] = "IoT BigData";
	Byte str1[]="Hello!!";
	DDRA = 0xFF;
	LcdInit_4bit();
	
    while (1) 
    {
		Lcd_Pos(0,0);
		Lcd_STR(str);
		Lcd_Pos(1,0);
		Lcd_STR(str1);
    }
}
