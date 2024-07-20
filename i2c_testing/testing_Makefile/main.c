#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./defs.h" // definitions, CPU speed, USART and I2C
#include "./USART.h"
#include "./I2C.h"
#include "./SH1106.h"
#include "./5x7font.h"

/*
TODO Code stuff:
- Draw line method, up down how much pixels above (this will give us an option to draw boxes)
- Draw box, outline and inner thickness
- set cursue method - accpet x y coordinates and check if they're correct 
- write line of text - algorithem move the word to new array

*/


/*
TODO General:
- button input 


int main()
{
	unsigned char data[] = "Hello from atmega  ";
 
	DDRB |= (1<<DDB3);
	PORTB = (0<<PORTB3);
	uint8_t address = 0x78;
	uint8_t i2c_status;
	
	UART_init();
	
	char i = 'm ';
	i2c_init();
	
	init_SH1106();
	display_off_SH1106();

	while(1) {
		
		
		for (int x = SH1106_MIN_X; x < (SH1106_MAX_X + 1); x += 6) {
		
			draw_char_SH1106(x, 1, '*', FALSE);
		
		}
		
		
		/*for (uint8_t page = 1; page < 4; page++){
			for (uint8_t x = 2; x < (SH1106_MAX_X + 1); x += 6){
			
				if (character > 127){
				
					uint8_t character = 32;
				 
				 }
				
				draw_char_SH1106(x, page, character);
				character++;
			}
		}*/
		
		
		_delay_ms(1000);
		display_off_SH1106();
		

		
			
		
		}
		//RT_suint16(i2c_status);
		//RT_sends(data);
	return 0;
	}	






ISR(USART_RX_vect) 
{

	unsigned char data;
	data = UDR0;
	UDR0 = data;


}






 
