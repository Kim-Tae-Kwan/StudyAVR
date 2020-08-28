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

Byte Termianl_Title[]="\r\nPlease enter a new password : \r\n";
//---------------------------------------


Byte Password[16]="9875";

Byte Buf[16];
unsigned int Buf_idx=0;

unsigned int Mode=None;

Byte temp;

unsigned char KeyPad();
void Lcd_Display(Byte str1[],Byte str2[]);
void Password_Check();
void Password_Changing();
void Mode_Id();
void Buf_Empty();
void Motor();
void putch(Byte data);
void putstr(Byte str[]);
Byte getch();


int main(void)
{
	DDRA = 0xFF;
	
	DDRC = 0x81;
	
	DDRD = 0xF8;
	PORTD = 0x00;
	
	DDRE = 0xFE;
	
	UCSR0A = 0x00;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
	
	UBRR0H = 0x00;
	UBRR0L = 7; //Terminal Baud Rate : 57600, AVR Baud Rate : 115200
	
	DDRF = 0x07;
	PORTF = 0xF0;
	
	LcdInit_4bit(); //Lcd 초기화
	
	Lcd_Display(Title,Title2);
	
	while (1)
	{
		Mode_Id();
		if(Mode==Terminal)
		{
			Buf_Empty();
			putstr(Termianl_Title);
			while(1)
			{
				temp=getch();
				if(temp=='\r')
				{
					Password_Changing();
					break;
				}
				Buf[Buf_idx++]= temp;
			}
		}
		else
		{
			temp=KeyPad();
			if(temp=='x') continue;
			else if(temp=='*') //비밀번호 입력 끝.
			{
				if(Mode==None)
					Password_Check();
				else if(Mode==Chage)
					Password_Changing();
			}
			else if(temp=='#')
			{
				Buf_Empty();
				Lcd_Display(Password_In,Buf);
			}
			else
			{
				Buf[Buf_idx++]=temp;
				if(Buf_idx>15) Buf_idx=16;
				Lcd_Display(Password_In,Buf);
			}
		}
		
	}
}

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

void Lcd_Display(Byte *str1,Byte *str2)
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
	if(Buf[0]=='\0')
	{
		Lcd_Display(NULL,"Was not entered");
		_delay_ms(500);
		Lcd_Display(Password_Change,NULL);
	}
	else
	{
		for(int i=0;i<16;i++)
		{
			Password[i]='\0';
		}
		strcpy(Password,Buf);
		Buf_Empty();
		Mode=None;
		Lcd_Display(Chaged_Password,NULL);
		_delay_ms(1000);
		Lcd_Display(Title,Title2);	
	}
	
	
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

void putch(Byte data)
{
	while((UCSR0A & 0x20)==0);
	UDR0 = data;
	UCSR0A |= 0x20;
}

void putstr(Byte *str)
{
	int i=0;
	while(str[i]!='\0')
	{
		putch(str[i++]);
	}
}

Byte getch()
{
	Byte data;
	while((UCSR0A & 0x80)==0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}