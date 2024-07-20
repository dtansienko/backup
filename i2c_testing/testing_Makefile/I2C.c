#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./defs.h"



//hard code 400khz i2c frequency
void i2c_init() {

	TWBR = 1; // set Baud Rate register for desired divions
	TWSR |=  (0<<TWPS1) | (0<<TWPS0); //set prescale
}

uint8_t i2c_start(uint8_t address) {
	uint8_t status;

	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); //enable TWI and generate start condition
	while(!(TWCR&(1<<TWINT))); //wait untill HARDWARE sets TWINT to 1
	
	
	//if (!(TWSR & 0xF8) == 0x08) return I2C_FAIL; // 0x08 is the code for start condition been sent
	TWDR = address; //load address
	TWCR=(1<<TWEN)|(1<<TWINT); // trasmit
	while(!(TWCR&(1<<TWINT))); //wait untill HARDWARE sets TWINT to 1

	if (!(TWSR & 0xF8) == 0x18) {
		status = I2C_ACK;
		}
		
	if (!(TWSR & 0xF8) == 0x20){
		status = I2C_NACK;
		}
	
	return status;

}
uint8_t i2c_send(uint8_t data) {

	uint8_t status;
	TWDR = data; // load data regsiter
	TWCR = (1<<TWINT)|(1<<TWEN); // trasmit
	while(!(TWCR&(1<<TWINT))); // wait for clearing the bit
	
	


}

void i2c_stop() {			/* I2C stop function */

    TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
    while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
}
