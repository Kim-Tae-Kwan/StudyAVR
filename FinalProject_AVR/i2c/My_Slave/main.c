/*
 * My_Slave.c
 *
 * Created: 2020-09-10 오후 1:32:44
 * Author : PKNU
 */ 

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD16x2_4bit.h"

#define I2C_ADDR 0x14
#define CR 0x0D

#define DHT11_ERROR 1


#define DHT11_DDR DDRF
#define DHT11_PORT PORTF
#define DHT11_PIN PINF
#define DHT11_INPUTPIN PF1

char data[100];
char i2c_data[100];
float temp, humi;

//volatile char data;
char i2c_cmd, i2c_in_data, i2c_out_data;
char i2c_state;



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
		return DHT11_ERROR;
	}
	_delay_us(80);
	//check start condition 2
	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
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
	
	return DHT11_ERROR;
}

void dht11_getdata(){
	
	uint16_t buf = getdata(&temp,&humi);
	if(buf == DHT11_ERROR)
	{
		lcd_gotoxy(11,0);
		lcd_print("Error");
	}
	//else if(buf==DHT11_ERROR2)
	//{
		//lcd_gotoxy(10,0);
		//lcd_print("2");
	//}
	//else if(buf==DHT11_ERROR3)
	//{
		//lcd_gotoxy(10,0);
		//lcd_print("3");
	//}
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
	 cli();
	 lcdinit();			/* Initialize LCD */
	 lcd_clear();			/* Clear LCD */
	 lcd_gotoxy(0,0);		/* Enter column and row position */
	 lcd_print("Humidity =");
	 lcd_gotoxy(0,1);
	 lcd_print("Temp = ");
	 
	 
	 DDRA = 0x01;
	 // load address into TWI address register
	 TWAR = I2C_ADDR << 1;
	 // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
	 TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
	 
	 
	 sei();
	
	
    while (1) 
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
		
		if(i2c_in_data=='1')
		{
			PORTA =0x01;
		}
		else if(i2c_in_data=='0')
		{
			PORTA =0x00;
		}
		
    }
}

ISR(TWI_vect)
{
	// TWSR Rg의 상태에 따라 다른 처리를 한다.
	switch(TW_STATUS) { /* Note: TW_STATUS is an macro that masks status bits from TWSR) */
		// SLA+W received, ACK returned.
		case TW_SR_SLA_ACK:			// 0x60
		//putchar2buffer('1');	함수는 Debuging을 위하여 사용 하였다.
		// Slave receive address 가 정상으로 수신된 상태
		i2c_state = 0;
		break;
		// Data received, ACK returned.
		case TW_SR_DATA_ACK:		// 0x80
		//putchar2buffer('2');
		i2c_state++;
		// 첫 Data를 정상으로 수신한 상태. 이 Data는 Command 또는 Data로 사용할 수 있다.
		// 이 Data는 i2c_cmd 변수에 저장 한다.
		if(i2c_state == 1){
			i2c_cmd = TWDR;
			// i2c_cmd = 1 : read_byte_data(int addr,char cmd) 명령의 첫 Byte가 수신된 상태로
			// 다음에 계속되는 read_byte_data 명령의 Timming을 맞 추기 위하여 미리
			// Master에 전송 할 Data를 준비하여 i2c_out_data에 저장 한다.
			if(i2c_cmd == 0x20){
				dtostrf(temp, 3, 1, i2c_data);
				i2c_out_data = i2c_data;
				// Master에 전송 할 문자를 모두 전송의 한 경우 CR Code를 전송 한다.
				//if(i2c_out_data == 0){
					//i2c_out_data = CR;
				//}
			}
		}
		// write_byte_data(int addr,char cmd,char val) 명령의 두번째 Data를 정상으로 수신한 상태.
		if(i2c_state == 2){
			// write_byte_data(int addr,char cmd,char val) 명령의 두번째 Data로,
			// 이 Data는 Input buffer에 저장 한다.
			if(i2c_cmd == 0xa0){
				i2c_in_data = TWDR;
			}
		}
		break;
		// Stop or repeated start condition received while selected.
		case TW_SR_STOP:			// 0xA0
		//putchar2buffer('7');
		i2c_state = 0;
		break;
		// SLA+R received, ACK returned.
		case TW_ST_SLA_ACK:			// 0xA8
		TWDR = i2c_out_data;
		//putchar2buffer('3');
		break;
		// Data transmitted, ACK received.
		case TW_ST_DATA_ACK:		// 0xB8
		TWDR = i2c_out_data;
		//putchar2buffer('4');
		break;
		// Data transmitted, NACK received.
		case TW_ST_DATA_NACK:		// 0xC0
		//TWCR|=(1<<TWINT);		// Clear interrupt flag
		//putchar2buffer('5');
		break;
		// Illegal start or stop condition.
		case TW_BUS_ERROR:			// 0x00
		//putchar2buffer('6');
		//i2cEventBusError();
		break;
		default:
		break;
	}
	// Set TWIE (TWI Interrupt enable), TWEN (TWI Enable),
	// TWEA (TWI Enable Acknowledge), TWINT (Clear TWINT flag by writing a 1)
	TWCR = 0xC5;
}