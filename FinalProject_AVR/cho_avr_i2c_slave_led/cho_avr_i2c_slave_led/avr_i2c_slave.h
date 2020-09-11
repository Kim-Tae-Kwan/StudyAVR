// avr_i2c_slave.h
//  Author: chowk

#ifndef AVR_I2C_SLAVE_H_
#define AVR_I2C_SLAVE_H_

extern char *pt_out;
extern char i2c_cmd, i2c_in_data, i2c_out_data, led_cmd, cmd_flag;

void i2cInit();
void i2cEventBusError();
void i2cSlaveInit(uint8_t address);
void i2cSlaveShutdown();

#endif /* AVR_I2C_SLAVE_H_ */