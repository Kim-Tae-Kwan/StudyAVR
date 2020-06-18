/*
 * FND_COUTING.c
 *
 * Created: 2020-06-15 오전 11:45:06
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>
unsigned char FND_DATA_TBL[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x6F};



int main()
{
	DDRA=0xFF;
	DDRF=0x0F;
	DDRD=0xFE;
	unsigned long cnt=88880;
	
	while(1)
	{	
			
			if(PIND==0x01) cnt =0;
			
			PORTF=0x07; // 1자리
			PORTA=FND_DATA_TBL[(((cnt%10000)%1000)%100)/10];
			
			_delay_ms(1);
			
			PORTF=0x0B; // 10자리
			PORTA=FND_DATA_TBL[((cnt%10000)%1000)/100];
			
			_delay_ms(1);
			
			PORTF=0x0D; //100자리
			PORTA=FND_DATA_TBL[(cnt%10000)/1000];
			
			_delay_ms(1);
			
			PORTF=0x0E; //1000자리
			PORTA=FND_DATA_TBL[cnt/10000];
	
			_delay_ms(1);
			
			
			cnt++;
			if(cnt>99999) cnt=0;
	}
	return 0;
}