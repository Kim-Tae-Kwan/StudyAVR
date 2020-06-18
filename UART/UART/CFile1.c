/*
 * CFile1.c
 *
 * Created: 2020-06-17 오후 4:51:14
 *  Author: PKNU
 */ 
/*
 * UARTPIEZO.c
 *
 * Created: 2020-06-17 오후 4:05:15
 *  Author: PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

void putch(unsigned char data)
{
   while((UCSR0A & 0x20)==0);
   UDR0 = data;
   UCSR0A |= 0x20;
}

unsigned char getch()
{
   unsigned char data;
   while((UCSR0A & 0x80)==0);
   data=UDR0;
   UCSR0A |= 0x80;
   return data;
}

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};

int main()
{
   unsigned char piano;
   DDRE = 0xFE;

   // 데이터 통신부분
   UCSR0A = 0x00;
   UCSR0B = 0x18;
   UCSR0C = 0x06;

   UBRR0H = 0x00;
   UBRR0L = 0x03;

   // 부저 부분
   TCCR3A = 0x00;
   TCCR3B = 0x19;
   TCCR3C = 0x00;
   TCNT3 = 0x0000;

   while(1)
   {
      piano = getch();
      if(piano == '1')
      {
         ICR3 = 7372800/DoReMi[0]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '2')
      {
         ICR3 = 7372800/DoReMi[1]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '3')
      {
         ICR3 = 7372800/DoReMi[2]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '4')
      {
         ICR3 = 7372800/DoReMi[3]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '5')
      {
         ICR3 = 7372800/DoReMi[4]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '6')
      {
         ICR3 = 7372800/DoReMi[5]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '7')
      {
         ICR3 = 7372800/DoReMi[6]/10;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
      else if(piano == '8')
      {
         ICR3 = 7372800/DoReMi[7]/20;
         TCCR3A = 0x40;
         _delay_ms(500);
         TCCR3A = 0x00;
      }
   }

   return 0;
}