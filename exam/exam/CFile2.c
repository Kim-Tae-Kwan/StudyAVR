/*
 * CFile2.c
 *
 * Created: 2020-06-18 오후 12:00:56
 *  Author: PKNU
 */ 
/*
 * ADC_Cds.c
 *
 * Created: 2020-06-16 오후 4:56:06
 *  Author: PKNU
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

int main()
{
	unsigned int AdData= 0;
	DDRA = 0xFF;
	DDRF = 0x02;
	PORTF = 0x02;
	
	LcdInit_4bit();
	
	ADMUX = 0x00;
	ADCSRA = 0x86;
	
	
	
	while(1)
	{
		ADCSRA |=0x40;
		while((ADCSRA & 0x10)==0x00);
		AdData = ADC;
		Lcd_Pos(0,0);
		Lcd_STR("Cds: ");
		Lcd_CHAR((AdData/1000)%10+'0');
		Lcd_CHAR((AdData/100)%10+'0');
		Lcd_CHAR((AdData/10)%10+'0');
		Lcd_CHAR((AdData)%10+'0');
		_delay_ms(500);
	}




	return 0;
}