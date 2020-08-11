/*
 * Password_System.c
 *
 * Created: 2020-08-11 오후 8:41:12
 * Author : vodja
 */ 

#include <avr/io.h>
#include "Lcd.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define Keypadin PINF

Byte str[] = "Password Insert!!";
Byte str1[]="";
Byte str2[]="Key Pad Number";

Byte IsPassword[13];
int IsPassword_index=0;
Byte Password[]="9875";

Byte IsPassword_false[]="Password False!!";
Byte IsPassword_true[]="Open The Door";
Byte IsPassword_Long[]="Very Long";

int ispassword = 1;
Byte temp;

unsigned char KeyPad()
{
	PORTF = 0xFE;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEE); return '1'; } // 1110 1110 & 1111 0000 = 1110 0000
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDE); return '4';} //1101 1111
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBE); return '7';} // 1011 1111
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7E); return '*';}
	
	PORTF = 0xFD;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xED); return '2';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDD); return '5';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBD); return '8';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7D); return '0';}
	
	PORTF = 0xFB;
	//_delay_ms(100);
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEB); return '3';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDB); return '6';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBB); return '9';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7B); return '#';}
	
	return 'x';
}

void Password_Check()
{
	Lcd_Clear();
	Lcd_Pos(0,0);
	Lcd_STR(str2);
	
	if(strcmp(Password,IsPassword)==0)
	{
		Lcd_Pos(1,0);
		Lcd_STR(IsPassword_true);
	}
	else
	{
		Lcd_Pos(1,0);
		Lcd_STR(IsPassword_false);
	}
	
	for (int j=0;j<IsPassword_index;j++)
	{
		IsPassword[j]='\0';
	}
	
	IsPassword_index=0;
}

int main(void)
{
    DDRA = 0xFF;
    DDRF = 0x07;
    PORTF = 0xF0;
    
    LcdInit_4bit();
    Lcd_Pos(0,0);
    Lcd_STR(str);
    Lcd_Pos(1,0);
    Lcd_STR(str1);
    
    while (1)
    {
	    temp=KeyPad();
	    if(temp=='x') continue;
	    else if(temp=='*') //비밀번호 입력 끝.
		{
			Password_Check();
			continue;
		}
		
	    Lcd_Clear();
	    Lcd_Pos(0,0);
	    Lcd_STR(str2);
	    Lcd_Pos(1,0);
		IsPassword[IsPassword_index++]=temp;
		if(IsPassword_index>15) IsPassword_index=16;
		Lcd_STR(IsPassword);
	    
	    
    }
}

