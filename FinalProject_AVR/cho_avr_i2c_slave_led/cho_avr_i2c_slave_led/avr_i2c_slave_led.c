// avr_i2c_slave_basic.c
// Created: 2019-09-04 오후 3:23:28
//  Author: chowk
 
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdint.h>
#include <stdio.h>

#include "cho_uart_init.h"
#include "cho_circular_buffer.h"
#include "avr_i2c_slave.h"

#define SCL_FREQ  100   // kHz
#define CPU_CLOCK 16000 // kHz
#define LEDPORT PORTF
#define TWI_ADDRESS 0x14	// Slave address

// i2c_cmd: Master로 부터 전송된 명령 Data를 저장
// i2c_in_data: Master로 부터 전송된 Data를 저장
// i2c_out_data: Master로 전송 할 Data를 저장
char i2c_cmd, i2c_in_data, i2c_out_data, led_cmd, cmd_flag;

// i2c(TWI)의 Baudrate를 설정 한다.
void init_baudrate(void){
	TWBR = ((CPU_CLOCK /SCL_FREQ) - 16) / 2;
}

void i2cInit() {
	// Master에 전송 할 문자를 가리키는 Pointer를 초기화
	i2c_cmd = 0;
	
	// Init TWI bit rate
	init_baudrate();
	// Disable watchdog, initialize serial
	wdt_disable();
	// Initialize TWI slave
	i2cSlaveInit(TWI_ADDRESS);
}

// i2c Slave를 초기화 한다.
void i2cSlaveInit(uint8_t address) {
	cli();
	// Slave address를 설정하고, General call recognition을 Enable 한다.
	TWAR = (address << 1) | 0x01;
	// Set TWIE(TWI Interrupt enable), TWEN(TWI Enable),
	// TWEA(TWI Enable Acknowledge), TWINT(Clear TWINT flag by writing a 1)
	TWCR = 0xC5;
	sei();
	return;
}

// i2c Event Bus Error 발생시 실행되는 함수
void i2cEventBusError() {
	printf("i2c Bus Error\r\n");
}

// i2c Slave Shutdown 시 실행하는 함수
void i2cSlaveShutdown() {
	cli();
	TWCR = 0;
	TWAR = 0;
	sei();
	return;
}

// Master 와 송수신시 발생하는 Interrupt 처리 루틴.
// Interrupt 발생 시 TWI Status는 TWSR(TWI Status Register) Rg에 저장 된다.
char i2c_state;	// Master 와 송수신시 현재의 상태(위치)를 저장 한다.
ISR(TWI_vect) {
	// TWSR Rg의 상태에 따라 다른 처리를 한다.
	switch(TW_STATUS) { // Note: TW_STATUS is an macro that masks status bits from TWSR)
		// SLA+W received, ACK returned.  
		case TW_SR_SLA_ACK:			// 0x60
			//putchar2buffer('1');	// 이 함수는 Debuging을 위하여 사용 하였다.
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
				// read_byte_data(int addr,char cmd) 명령의 첫 Byte가 수신된 상태로
				// 다음에 계속되는 read_byte_data 명령의 Timming을 맞 추기 위하여 미리
				// Master에 전송 할 Data를 준비하여 i2c_out_data에 저장 한다.
				if(i2c_cmd == 0x20){
					i2c_out_data = LEDPORT;
				}
			}
			// write_byte_data(int addr,char cmd,char val) 명령의 두번째 Data를 정상으로 수신한 상태.
			if(i2c_state == 2){
				// write_byte_data(int addr,char cmd,char val) 명령의 두번째 Data로,
				// 이 Data는 LED 설정에 사용 한다.
				if(i2c_cmd == 0xa4){
					i2c_in_data = TWDR;
				}
				// 새로운 명령이 수신되었음을 표시한다.
				// 이 Flag는 수신된 명령이 실행되면 Clear 된다.
				cmd_flag = 1;	
			}
			break;
		// Stop or repeated start condition received while selected.
		case TW_SR_STOP:			// 0xA0
			//putchar2buffer('7');
			i2c_state = 0;
		break;
		// SLA+R received, ACK returned.
		// Master에 data를 전송 한다.
		case TW_ST_SLA_ACK:			// 0xA8
			TWDR = i2c_out_data;
			//putchar2buffer('3');
			break;
		// Data transmitted, ACK received.
		case TW_ST_DATA_ACK:		// 0xB8
			//putchar2buffer('4');
			// Either slave selected (SLA_ACK) and data requested or data transmitted, ACK received
			//	and next data requested
			break;
		// Data transmitted, NACK received.
		case TW_ST_DATA_NACK:		// 0xC0
			//TWCR|=(1<<TWINT);		// Clear interrupt flag
			//putchar2buffer('5');
			break;
		// Illegal start or stop condition.
		case TW_BUS_ERROR:			// 0x00
			//putchar2buffer('5');
			i2cEventBusError();
			break;
		default:
			break;
	}
	// Set TWIE (TWI Interrupt enable), TWEN (TWI Enable),
	// TWEA (TWI Enable Acknowledge), TWINT (Clear TWINT flag by writing a 1)
	TWCR = 0xC5;
}
