// cho_slave_i2c.c
// S/W Environment : AVR Studio + WINAVR Compiler
// Target : M128
// Crystal: 16Mhz
//
// Author : chowk

#  ifndef atmega128
#define atmega128
#  endif
#define FOSC 16000000// Clock Speed

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "avr_i2c_slave.h"
#include "cho_uart_init.h"
#include "cho_circular_buffer.h"

void init_devices(void);
static int put_char(char c, FILE *stream);


static int put_char(char c, FILE *stream)
{
	tx0_char(c);
	return 0;
}

//Initialize all peripherals
void init_devices(void)
{
	cli();				// Disable all interrupts
	i2cInit();			// Init i2c slave
	uart_init();		// UART 0 초기화
	fdevopen(put_char,0);
	sei();				// Re-enable interrupts
}

int main (void)
{
	char data;
	
	init_devices();
	printf("I2C testing.\n\r");
	while(1){
		// 수신한 문자는 nterrupt 처리 Routine 에서 Buffer에 저장 된다.
		// Input Buffer로 부터 수신한 문자를 갖어 온다.
		data = getchar_buffer();

		// Buffer로 부어 갖어온 문자가 유효한 경우
		if(data != 0){
			// 모니터에 문자를 출력 한다.
			tx0_char(data);
			// 문자가 CR(Carriage return) Code인 경우 LF(Line feed) COde를 출력 한다. 
			if(data == CR) tx0_char(LF);
			//printf("C: %c  %x",data,data);
		}
	}
	return 0;
}


