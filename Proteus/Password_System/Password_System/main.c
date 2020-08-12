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
#define ModePin PIND
#define None 0
#define Chage 1
#define Terminal 2
#define Setting 3

//-------------------------------------> 문자열 변수
Byte Title[] = "Password System";
Byte Title2[] = "Insert Password";

Byte Password_In[]="Password :";
Byte Password_Change[]="Change Password:";

Byte IsPassword_false[]="Password False!!";
Byte IsPassword_true[]="Open The Door";

Byte Chaged_Password[]="Chaged Password!";
//---------------------------------------


Byte Password[16]="9875";

Byte Buf[16];
unsigned int Buf_idx=0;

unsigned int Mode=None;

Byte temp;

unsigned char KeyPad()
{
	PORTF = 0xFE;
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEE); return '1'; } 
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDE); return '4';} 
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBE); return '7';} 
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7E); return '*';}
	
	PORTF = 0xFD;
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xED); return '2';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDD); return '5';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBD); return '8';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7D); return '0';}
	
	PORTF = 0xFB;
	if((Keypadin & 0xF0) == 0xE0) { while(Keypadin == 0xEB); return '3';}
	if((Keypadin & 0xF0) == 0xD0) { while(Keypadin == 0xDB); return '6';}
	if((Keypadin & 0xF0) == 0xB0) { while(Keypadin == 0xBB); return '9';}
	if((Keypadin & 0xF0) == 0x70) { while(Keypadin == 0x7B); return '#';}
	
	return 'x';
}

void Lcd_Display(Byte str1[],Byte str2[])
{
	Lcd_Clear();
	
	Lcd_Pos(0,0);
	Lcd_STR(str1);
	
	Lcd_Pos(1,0);
	Lcd_STR(str2);
}

void Password_Check()
{
	
	if(strcmp(Password,Buf)==0)
	{
		Lcd_Display(Password_In,IsPassword_true);
		Motor();
	}
	else
	{
		Lcd_Display(Password_In,IsPassword_false);
	}
	
	Buf_Empty();
}

void Password_Changing()
{
	for(int i=0;i<16;i++)
	{
		Password[i]='\0';
	}
	strcpy(Password,Buf);
	
}

void Mode_Id()
{
	if(ModePin==0x06) // 변경 버튼
	{
		while(ModePin==0x06);
		Mode=Chage;
		Buf_Empty();
		Lcd_Display(Password_Change,Buf);
						
	}
	else if(ModePin == 0x05) // 터미널 버튼
	{
		while(ModePin==0x05);
		Mode=Terminal;
		Buf_Empty();
	}
	else if(ModePin == 0x03) // 세팅 버튼
	{
		while(ModePin==0x03);
		Mode=Setting;
		Buf_Empty();
	}
}

void Buf_Empty()
{
	for (int j=0;j<Buf_idx;j++)
	{
		Buf[j]='\0';
	}
	Buf_idx=0;
}

void Motor()
{
	PORTC = 0x01;
	_delay_ms(1000);
	PORTC = 0x80;
	_delay_ms(1000);
	PORTC = 0X00;
}



int main(void)
{
    DDRA = 0xFF;
	
    DDRF = 0x07;
    PORTF = 0xF0;
    
	DDRD = 0xF8;
	PORTD = 0x00;
	
	DDRC = 0x81;
	
    LcdInit_4bit(); //Lcd 초기화
	
    Lcd_Display(Title,Title2);
    
    while (1)
    {
		Mode_Id();
		switch(Mode){
			case None :
						temp=KeyPad();
						if(temp=='x') break;
						else if(temp=='*') //비밀번호 입력 끝.
						{
							Password_Check();
						}
						else
						{
							Buf[Buf_idx++]=temp;
							if(Buf_idx>15) Buf_idx=16;
							Lcd_Display(Password_In,Buf);	
						}
						break;
			case Chage :
						temp=KeyPad();
						if(temp=='x') break;
						else if(temp=='*') //비밀번호 입력 끝.
						{
							Password_Changing();
							Mode=None;
							Buf_Empty();
							Lcd_Display(Chaged_Password,NULL);
							_delay_ms(1000);
							Lcd_Display(Title,Title2);
						}
						else
						{
							Buf[Buf_idx++]=temp;
							if(Buf_idx>15) Buf_idx=16;
							Lcd_Display(Password_Change,Buf);
						}
						break;
						
			
		}
		
    }
}

