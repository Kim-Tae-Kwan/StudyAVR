// cho_slave_i2c.c
// S/W Environment : AVR Studio + WINAVR Compiler
// Target : M128
// Crystal: 16Mhz
//
// Author : chowk

// i2c 통신을 이용한 LED control
// LED control command format
// i2c_cmd = 0xa1: Left shift command
// i2c_cmd = 0xa2: Right shift command
// i2c_cmd = 0xa3: Blinking command
// i2c_cmd = 0xa4 , val = LED Value: Set LED command
// i2c_cmd = 0x20: Get LED status command

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
#include "timer1_COMPA_int_led.h"

#define LEDPORT PORTF
#define DDRLED DDRF

void init_devices(void);
static int put_char(char c, FILE *stream);

static int put_char(char c, FILE *stream)
{
	tx0_char(c);
	return 0;
}

void port_init(void)
{
	LEDPORT = 0x00;		// Initialize Output Port
	DDRLED  = 0xff;		// PORTF : Output Port
}

//Initialize all peripherals
void init_devices(void)
{
	cli();				// Disable all interrupts
	port_init();
	timer1_init();
	i2cInit();			// Init i2c slave
	uart_init();		// UART 0 초기화
	fdevopen(put_char,0);
	ledOperationMode = 0;
	i2c_cmd = 0;
	cmd_flag = 0;
	sei();				// Re-enable interrupts
}




int main (void)
{
	char data;
	init_devices();
	printf("I2C LED control\n\r");

	while(1){
		// 입력과 출력은Interrupt 처리 Routine 에서 처리 된다.
		// Input Buffer로 부터 문자를 갖어 온다.
		//led_cmd = (i2c_cmd & 0xf0) >> 4;
		data = getchar_buffer();
		if(data != 0)tx0_char(data);
		
		if(cmd_flag != 0){
			//printf(" cmd: %x  ",i2c_cmd);
			switch(i2c_cmd) {
				// i2c_cmd = 0xa1: Left shift command
				case 0xa1:
					LEDPORT = 0x01;
					ledOperationMode = SHIFT_LEFT;
				break;
				// i2c_cmd = 0xa2: Right shift command
				case 0xa2:
					LEDPORT = 0x80;
					ledOperationMode = SHIFT_RIGHT;
				break;
				// i2c_cmd = 0xa3: Blinking command
				case 0xa3:
					LEDPORT = 0x00;
					ledOperationMode = BLINKING;
				break;
				// i2c_cmd = 0xa4: Set LED command
				case 0xa4:
					LEDPORT = i2c_in_data;
					// 주기적인 동작을 정지 시킨다.
					ledOperationMode = SET_LED;
				break;
				// i2c_cmd = 0x20: Get LED ststus command
				case 0x20:
					// LED 상태 전송은 TWI_vect routine 에서 직접 처리함.
					//ledOperationMode = GET_LED_STATUS;
				break;
				default:
				break;
			}
			cmd_flag = 0;
		}
			//printf("C: %c  %x",data,data);
	}
	return 0;
}
