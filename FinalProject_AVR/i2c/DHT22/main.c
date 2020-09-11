#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD16x2_4bit.h"

#define DHT11_ERROR1 1
#define DHT11_ERROR2 2
#define DHT11_ERROR3 3
#define DHT11_ERROR4 4
#define DHT11_ERROR5 5
#define DHT11_ERROR6 6

#define DHT11_DDR DDRF
#define DHT11_PORT PORTF
#define DHT11_PIN PINF
#define DHT11_INPUTPIN PF1

char data[100];
float temp, humi;

void dht11_getdata();
uint16_t getdata(float *temperature, float *humidity);

/* get data from dht11 */
uint16_t getdata(float *temperature, float *humidity) {
	uint8_t bits[5];
	uint8_t i,j = 0;
	
	memset(bits, 0, sizeof(bits));
	
	//reset port
	DHT11_DDR |= (1<<DHT11_INPUTPIN); //output
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_ms(100);
	
	//send request
	DHT11_PORT &= ~(1<<DHT11_INPUTPIN); //low
	_delay_us(500);
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	DHT11_DDR &= ~(1<<DHT11_INPUTPIN); //input
	_delay_us(40);
	
	
	//check start condition 1
	if((DHT11_PIN & (1<<DHT11_INPUTPIN))) { //Data PIN == High 이면
		return DHT11_ERROR1;
	}
	_delay_us(80);
	//check start condition 2
	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR2;
	}
	_delay_us(80);
	
	//read the data
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			while(!(DHT11_PIN & (1<<DHT11_INPUTPIN))); //wait for an high input
			_delay_us(30);
			if(DHT11_PIN & (1<<DHT11_INPUTPIN)) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			while(DHT11_PIN & (1<<DHT11_INPUTPIN)); //wait until input get low
		}
		bits[j] = result;
	}
	
	//reset port
	DHT11_DDR |= (1<<DHT11_INPUTPIN); //output
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_ms(100);
	
	//check checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) 
	{ 
		*temperature = (float)(bits[2]<<8 | bits[3])/10.0;
		*humidity = (float)(bits[0]<<8 | bits[1])/10.0;
		//temp = bits[2]<<8 | bits[3];
		//humi = bits[0]<<8 | bits[1];
		return 0;
	}
	
	return DHT11_ERROR3;
}

void dht11_getdata(){
	
	uint16_t buf = getdata(&temp,&humi);
	if(buf == DHT11_ERROR1)
	{
		lcd_gotoxy(11,0);		
		lcd_print("Error");
	}
	else if(buf==DHT11_ERROR2)
	{
		lcd_gotoxy(10,0);
		lcd_print("2");
	}
	else if(buf==DHT11_ERROR3)
	{
		lcd_gotoxy(10,0);
		lcd_print("3");
	}
	//else
	//{
		//itoa(buf,data,10);
		//lcd_gotoxy(11,0);
		//lcd_print(data);
		//
	//}
		
	 
}



int main(void)
{
	//char data[5];
	lcdinit();			/* Initialize LCD */
	lcd_clear();			/* Clear LCD */
	lcd_gotoxy(0,0);		/* Enter column and row position */
	lcd_print("Humidity =");
	lcd_gotoxy(0,1);
	lcd_print("Temp = ");
	
	while(1)
	{
	
		dht11_getdata();
		
		dtostrf(temp, 3, 1, data);
		//itoa(temp,data,10);
		lcd_gotoxy(11,1);
		lcd_print(data);
		
		dtostrf(humi, 3, 1, data);
		//itoa(humi,data,10);
		lcd_gotoxy(11,0);
		lcd_print(data);
		
		_delay_ms(300);
						
	}
}