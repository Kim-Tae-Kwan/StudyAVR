/*
 * i2c0slave.h
 *
 * Created: 2020-09-09 오후 1:34:38
 *  Author: PKNU
 */ 


#ifndef I2C0SLAVE_H_
#define I2C0SLAVE_H_

#include <inttypes.h>

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega128A__)
#else
#error "unsupported device"
#endif

#ifndef I2C_SLAVE_REGISTER_LENGTH
#define I2C_SLAVE_REGISTER_LENGTH 128
#endif

#define AFTER_SLA_R_ACK                 0xA8 // 168
#define AFTER_TRANSMIT_DATA_ACK         0xB8 // 184
#define AFTER_TRNASMIT_DATA_NACK        0xC0 // 192
#define AFTER_TRANSMIT_LAST_DATA_ACK    0xC8 // 200
#define AFTER_SLA_W_ACK                 0x60 //  96
#define AFTER_RECEIVE_DATA_ACK          0x80 // 128
#define AFTER_RECEIVE_DATA_NACK         0x88 // 136
#define AFTER_RECEIVE_STOP              0xA0 // 160
#define I2C_BUS_ERROR                   0x00 //   0

class i2c0slave_
{
	public :
	i2c0slave_();
	~i2c0slave_();

	void    begin(uint8_t slv_addr);
	void    isr_twi(void);
	void    set_data (uint8_t reg_addr, uint8_t val);
	uint8_t get_data (uint8_t reg_addr);
	protected :

	private :
	volatile uint8_t receive_count;
	volatile uint8_t register_address;
	volatile uint8_t data[I2C_SLAVE_REGISTER_LENGTH];

}; // i2c0slave_

extern i2c0slave_ i2c0slave;



#endif /* I2C0SLAVE_H_ */