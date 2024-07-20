#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./defs.h" // definitions, CPU speed, USART and I2C
#include "./USART.h"
#include "./I2C.h"
#include "./SH1106.h"
#include "./5x7font.h"
#include "./micro.h"
#include "./millis.h"

#define second_in_uS 1000000

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
*/


//interrupt variables
volatile uint8_t new_data_flag = FALSE;
volatile uint8_t direction;

//global variables

//inital position of sprite
uint8_t x_pos = 70;
uint8_t y_pos = 4;

int main()
{
	unsigned char data[] = "Hello from atmega  ";
 
	DDRB |= (1<<DDB3);
	PORTB = (1<<PORTB3);
	uint8_t address = 0x78;
	uint8_t i2c_status;
	
	timer_init();
	//millis_init();
	UART_init();
	//i2c_init();
	//init_SH1106();
	display_off_SH1106();
	draw_sprite_SH1106(x_pos, y_pos, FULL_SPRITE, FALSE);
	
	uint32_t lastChangeLed1 = 0;


	while(1) {
		sei();
		UART_print_uint(5745);
		uint32_t now = micros_get();
		if(now - lastChangeLed1 >= 500)
		{
			// Toggle LED
			PORTB ^= (1<<PORTB3);
			
			// Store time
			lastChangeLed1 = now;
		}
	
		//move_sprite();
		//draw_sprite_SH1106(x_pos, y_pos, FULL_CHAR, FALSE);
		//_delay_ms(100);
		//draw_sprite_SH1106(x_pos, y_pos, FULL_CHAR, FALSE);
		//draw_char_SH1106(x_pos, 5, FULL_CHAR, FALSE);
		cli();	
		}
	return 0;
	}	


// Check if x and y are in the borders, else dot increase them

void move_sprite(){

	sei();
	
	if(new_data_flag){
		cli();
		if(direction == UP){
			y_pos--;
		} 
		if(direction == DOWN) y_pos++;
		if(direction == RIGHT) x_pos++;
		if(direction == LEFT) x_pos--;
		//display_off_SH1106();
		draw_sprite_SH1106(x_pos, y_pos, FULL_CHAR, FALSE);
		new_data_flag = FALSE;
		sei();
	}
	cli();	
}

ISR(USART_RX_vect) 
{
	
	new_data_flag = TRUE;
	direction = UDR0;
	UART_sendc(direction);
}






 
