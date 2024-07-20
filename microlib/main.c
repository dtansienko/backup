#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "defs.h" // definitions, CPU speed, USART and I2C
#include "micros.h"
#include "USART.h"

uint8_t last_channel_3;
uint32_t current_time;
uint32_t timer_3;
uint16_t receiver_input_channel_3;
uint8_t prev_ch1_val;
uint32_t init_time_ch1;
uint32_t pusle_time_ch1;
static volatile uint8_t ch_change;


int main()
{
	DDRB |= (1<<DDB3);
	PORTB = (0<<PORTB3);

	UART_init();
	rc_init();
	micro_timer_init();

	
	unsigned long lastChangeLed1 = 0;
	
	sei();
	while(1) {
		//_delay_ms(250);
		unsigned long now = micro_get();
		if(ch_change){
			UART_print_uint(receiver_input_channel_3);
			ch_change = 0;
		}	
		
		

		if(now - lastChangeLed1 >= 1000000)
		{
			// Toggle LED
			PORTB ^= (1<<PORTB3);
			
			// Store time
			lastChangeLed1 = now;
		}
	

	}	
}


void rc_init(){
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan

	PCMSK0 |= (1 << PCINT2);  

	//set pins as input	
	DDRC |= (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2);
	
	PORTC = 0xFF;
	// enable interrups in PCINT[14:8]
	//PCICR |= (1<<PCIE1); 
	//what pins will trigger interrup
	//PCMSK1 |= (1<<PCINT13) | (1<<PCINT12) | (1<<PCINT11) | (1<<PCINT10);
	
	//PINC input pins


}

ISR(PCINT0_vect){
	
	current_time = micro_get();
	//UART_print_uint(PINB & 0b00000100);
	if(PINB & 0b00000100){                                       
		if(last_channel_3 == 0){                                  
			last_channel_3 = 1;                                     
			timer_3 = current_time;                                 
    		}
	}
	else if(last_channel_3 == 1){                                
		last_channel_3 = 0;                                        
		receiver_input_channel_3 = current_time - timer_3;         
		ch_change = 1;
	}
	
}


