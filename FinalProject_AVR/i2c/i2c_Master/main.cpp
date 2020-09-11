/*
 * i2c_Master.cpp
 *
 * Created: 2020-09-09 오후 1:37:59
 * Author : PKNU
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "uart"
#include "i2c0.h"

int main(int argc, char **argv)
{
	// setup
	uart1.begin(57600);
	i2c0.begin();

	uart1.println("master");

	for(;;)
	{
		if (uart1.available())
		{
			uint8_t a = uart1.read();
			i2c0.write_REG(0x20,0x10,a); // 0x20 == slave address
			_delay_us(4);
			i2c0.write_REG(0x20,0x11,a+10);
			_delay_us(4);

			uint8_t buff[5];
			i2c0.read_REG(0x20,0x10,buff,2);
			uart1.println(buff[0]); // register 0x10 data
			uart1.println(buff[1]); // register 0x11 data
		}
	}

	return 0;
}

