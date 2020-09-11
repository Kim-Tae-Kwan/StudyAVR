// avr_i2c_slave.h
//  Author: chowk

#ifndef AVR_I2C_SLAVE_H_
#define AVR_I2C_SLAVE_H_

#define TWI_ADDRESS 0x14	// Slave address

extern char *pt_out;

void i2cInit();
void i2cEventBusError();
void i2cSlaveInit(uint8_t address);
void i2cSlaveShutdown();

#endif /* AVR_I2C_SLAVE_H_ */