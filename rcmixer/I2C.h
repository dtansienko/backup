#ifndef I2C_H
#define I2C_H

void i2c_init();
uint8_t i2c_start(uint8_t address);
uint8_t i2c_send(uint8_t data);
void i2c_stop();

#endif
