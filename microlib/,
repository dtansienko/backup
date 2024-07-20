#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "./defs.h"
#include "./USART.h"


void UART_suint16(uint16_t data){
	uint8_t tthousand = 0;
	uint8_t thousand = 0;
	uint8_t hun = 0;
	uint8_t dec = 0;
	
	while(data >=10000) {
		data -= 10000;
		tthousand++;
	}	
		
	while(data >=1000) {
		data -= 1000;
		tthousand++;
	}	
	
	while(data >=100) {
		data -= 100;
		hun++;
	}	
	
	while(data >=10) {
		data -= 10;
		dec++;
	}	
	
	uint8_t digitPrinted = 0;
	
	
	if(tthousand != 0) {
		UART_sends(tthousand);
		digitPrinted = 1;
	}
	
	if( (digitPrinted == 1) || (thousand != 0)) {
		UART_sends(thousand);
		digitPrinted = 1;
	}
	
	if( (digitPrinted == 1) || (hun != 0)) {
		UART_sends(hun);
		digitPrinted = 1;
	}
	
	if( (digitPrinted == 1) || (dec != 0)) {
		UART_sends(dec);
		digitPrinted = 1;
	}
	
	UART_sends(data); //send last digit with termination char
	
	


}

void UART_init() {
	
	// hard coded 57600 baud
	UBRR0H = (BAUD_RATE_UBBG >> 8); //8 msb of UBBR
	UBRR0L = (BAUD_RATE_UBBG & 0xFF); //8 lsb of UBBR
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);       /* Set frame format: 8data, 1stop bit  */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); /* Enable  trans, rec and interrupt*/
	
}

void UART_print_uint(uint32_t data) {
	
	char buffer [33]; // sizeof(int)*8 + 1  = 17 in avr gcc
	UART_sends(itoa(data, buffer, 10));
	
	


}

void UART_sendc(unsigned char data) {

	while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
	UDR0 = data;

}
void UART_sends(char* data) {
	
	while(*data > 0) {
		UART_sendc(*data);
		data ++;
	
	}
	
	//CR+LF
	UART_sendc(CR);
	UART_sendc(LR);

}
