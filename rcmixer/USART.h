#ifndef USART_H
#define USART_H


void UART_suint16(uint16_t data);

void UART_init();

void UART_sendc(unsigned char data);

void UART_print_uint(uint32_t data);

void UART_sends(char* data);

#endif
