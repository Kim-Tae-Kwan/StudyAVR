
/*
 * PWMLEDSwitch.c
 *
 * Created: 2020-06-17 오전 11:36:04
 *  Author: PKNU
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int Light=0;
volatile unsigned char Light_flag=1;

int main()
{
   DDRB = 0x80; // PB7 -> OCR2, PB4 -> OCR0 사용
   DDRD = 0x00;
   DDRE = 0x00;

   TCCR2 = (1 << WGM20) | (1 << COM21) | (1 << CS21); // 0x62 // PC PWM 모드, 8분주 PWM 주기:F_CPU/256/2/8
   TCNT2 = 0; // 0x00 // 타이머2 초기화

   EICRA = 0xFF; // 인터럽트 0, 1, 2, 3을 상승엣지 동작 설정
   EICRB = 0xFF; // 인터럽트 4, 5, 6, 7을 상승엣지 동작 설정
   EIMSK = 0xFF; // 인터럽트 허용
   EIFR = 0xFF; // 인터럽트 플래그 클리어

   sei();

   while(1)
   {
      if(Light_flag)
      {
         OCR2 = Light;
         Light_flag=0;
      }
   }

   return 0;
}

SIGNAL(INT0_vect)
{
   cli();
   Light = 0;
   Light_flag = 1;
   sei();
}

SIGNAL(INT1_vect)
{
   cli();
   Light -= 51;
   if(Light<0) Light=0;
   Light_flag = 1;
   sei();
}

SIGNAL(INT2_vect)
{
   cli();
   Light -= 31;
   if(Light<0) Light=0;
   Light_flag = 1;
   sei();
}

SIGNAL(INT3_vect)
{
   cli();
   Light -= 11;
   if(Light<0) Light=0;
   Light_flag = 1;
   sei();
}

SIGNAL(INT4_vect)
{
   cli();
   Light += 11;
   if(Light>255) Light=255;
   Light_flag = 1;
   sei();
}

SIGNAL(INT5_vect)
{
   cli();
   Light += 31;
   if(Light>255) Light=255;
   Light_flag = 1;
   sei();
}

SIGNAL(INT6_vect)
{
   cli();
   Light += 51;
   if(Light>255) Light=255;
   Light_flag = 1;
   sei();
}

SIGNAL(INT7_vect)
{
   cli();
   Light = 255;
   Light_flag = 1;
   sei();
}