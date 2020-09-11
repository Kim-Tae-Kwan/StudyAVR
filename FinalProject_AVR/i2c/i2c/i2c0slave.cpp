/*
 * i2c0slave.cpp
 *
 * Created: 2020-09-09 오후 1:35:19
 *  Author: PKNU
 */ 

#include "i2c0slave.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define SLAVE_ACK_RESPONSE \
TWCR |= (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)

#define SLAVE_NACK_RESPONSE \
TWCR |= (1<<TWINT)|(1<<TWEN)|(1<<TWIE)

//
// Constructor & Destructor
//
i2c0slave_::i2c0slave_()
{
	// variables initialization
}
i2c0slave_::~i2c0slave_() {}

//
// Public
//
void i2c0slave_::begin(uint8_t slv_addr)
{
	TWAR = (slv_addr<<1)|0;
	SLAVE_ACK_RESPONSE;
	sei();
}

void i2c0slave_::isr_twi(void)
{
	switch(TWSR)
	{
		case AFTER_SLA_R_ACK:
		// data ready
		TWDR = data[register_address];
		SLAVE_ACK_RESPONSE;
		break;

		case AFTER_TRANSMIT_DATA_ACK:
		if (register_address < I2C_SLAVE_REGISTER_LENGTH - 1)
		{
			// next data ready
			TWDR = data[++register_address];
			SLAVE_ACK_RESPONSE;
		}
		else SLAVE_NACK_RESPONSE;
		break;

		case AFTER_SLA_W_ACK:
		receive_count = 0;
		SLAVE_ACK_RESPONSE;
		break;

		case AFTER_RECEIVE_DATA_ACK:
		if (receive_count == 0)
		{
			// first data is register address
			register_address = TWDR & 0x7F; // constrain 0 ~ 127
			++receive_count;
			SLAVE_ACK_RESPONSE;
		}
		else
		{
			data[register_address] = TWDR;
			SLAVE_ACK_RESPONSE;
		}
		break;

		case AFTER_RECEIVE_DATA_NACK:
		if (register_address < I2C_SLAVE_REGISTER_LENGTH)data[register_address] = TWDR;
		SLAVE_NACK_RESPONSE;
		break;

		case I2C_BUS_ERROR:
		TWCR |= (1<<TWSTO)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);
		break;

		default:
		SLAVE_NACK_RESPONSE;
		break;
	}
}

void i2c0slave_::set_data(uint8_t reg_addr, uint8_t val)
{
	if (reg_addr < I2C_SLAVE_REGISTER_LENGTH)   data[reg_addr] = val;
}

uint8_t i2c0slave_::get_data(uint8_t reg_addr)
{
	if (reg_addr < I2C_SLAVE_REGISTER_LENGTH)   return data[reg_addr];
	else return -1;
}

//
// i2c0slave_ [Object name] = i2c0slave_();
//
i2c0slave_ i2c0slave = i2c0slave_();

ISR(TWI_vect)
{
	i2c0slave.isr_twi();
}