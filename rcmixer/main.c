#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "defs.h" // definitions, CPU speed, USART and I2C
#include "micros.h"
#include "USART.h"
#include "I2C.h"
#include "tda7439ds.h"

//rc pwm cycles to check valid input (Low Pass Filter)
#define RC_BUFFER_SIZE 16
//maximum variance between max val an min val inside ch buffer
#define RC_VARIANCE 4
#define EMPTY 0
#define FULL 1

//tda7439ds variables
uint8_t vol_chg = 0;

//function declarations
uint16_t rc_array_min(uint16_t arr[], uint8_t arr_size);
uint16_t rc_array_max(uint16_t arr[], uint8_t arr_size);
uint8_t check_rc_array(uint16_t arr[], uint8_t arr_size);
long map(long x, long in_min, long in_max, long out_min, long out_max);

//RC PWM variables
uint32_t current_time;

//CH1 variables
uint8_t last_channel_1;
uint32_t timer_1;
uint16_t receiver_input_channel_1;
static volatile uint8_t ch1_change;
uint16_t ch1_buffer[RC_BUFFER_SIZE];
uint8_t ch1_current_buffer_index = 0;
uint8_t ch1_buffer_flag = EMPTY;
uint16_t ch1_error_count = 0;


//CH1 variables
uint8_t last_channel_2;
uint32_t timer_2;
uint16_t receiver_input_channel_2;
static volatile uint8_t ch2_change;
uint16_t ch2_buffer[RC_BUFFER_SIZE];
uint8_t ch2_current_buffer_index = 0;
uint8_t ch2_buffer_flag = EMPTY;

int main()
{
	DDRD |= (1<<DDD7);
	PORTD = (1<<PORTD7);
	
	i2c_init();
	UART_init();
	micro_timer_init();
	
	tda7439ds_input_selector(IN1);
	tda7439ds_set_input_gain(0);
	//release speaker attn from mute
	tda7439ds_send_command(TDA7439_SPKR_ATN_R, 0);
	tda7439ds_send_command(TDA7439_SPKR_ATN_L, 0);
	tda7439ds_set_volume(48);
	tda7439ds_set_bass(3);
	tda7439ds_set_mid(0);
	tda7439ds_set_trebel(0);
	//tda7439ds_send_command(TDA7439_BASS_GAIN, 0);
	//tda7439ds_send_command(TDA7439_BASS_GAIN, 8);
	
	rc_init();
	

	
	unsigned long lastChangeLed1 = 0;
	uint16_t ch1_pwm = 0;
	uint8_t vol = 0;
	sei();
	while(1) {
		//_delay_ms(250);
		unsigned long now = micro_get();
		
		ch1_check();
		//UART_sends("spit buffer ch 1");
		if(ch1_buffer_flag == FULL){
		
			//UART_sends("spit buffer ch 1");
			ch1_pwm = ch1_buffer[0];
			UART_print_uint(ch1_pwm);
			UART_sends("");
		}
		
		if ((ch1_pwm < 2000) || (ch1_pwm > 1100)){
		
			vol = map(ch1_pwm, 1174, 1864, 0, 48);
			if(vol == 0){
				tda7439ds_send_command(TDA7439_SPKR_ATN_R, 120);
				tda7439ds_send_command(TDA7439_SPKR_ATN_L, 120);
			
			} 
			else {
			
				tda7439ds_send_command(TDA7439_SPKR_ATN_R, 0);
				tda7439ds_send_command(TDA7439_SPKR_ATN_L, 0);
			
			}
			tda7439ds_set_volume(vol);
			UART_print_uint(vol);
		}
		
		

		if(now - lastChangeLed1 >= 500000)
		{

			//i2c_start(TDA7439_ADDR);
			// Toggle LED
			PORTD ^= (1<<PORTD7);
			
			// Store time
			lastChangeLed1 = now;
		}
	

	}	
}

void ch1_check(){
	
	
	//check we filled the actual buffer
	if(ch1_change && (ch1_current_buffer_index < RC_BUFFER_SIZE)){
		
		if (ch1_buffer_flag == FULL){
			//clear buffer 
			ch1_buffer_flag = EMPTY;
			
		}
		//add signal to buffer
		ch1_buffer[ch1_current_buffer_index] = receiver_input_channel_1;
		ch1_current_buffer_index++;
	}
	else if (ch1_current_buffer_index == RC_BUFFER_SIZE){
	
		uint8_t diff;
		diff = check_rc_array(ch1_buffer, RC_BUFFER_SIZE);
		
		//check if buffer values are in spec - allaowed variance
		if (diff <= RC_VARIANCE){
			// clear buffer and start over
			ch1_current_buffer_index = 0;
			//indicate you can read buffer
			ch1_buffer_flag = FULL;
		
		} 
		else if (diff > RC_VARIANCE){
			// track errors in channel
			ch1_error_count++;
		
		}
	}
}

uint8_t check_rc_array(uint16_t arr[], uint8_t arr_size){
	uint16_t min, max, diff;
	
	min = rc_array_min(arr, arr_size);
	max = rc_array_max(arr, arr_size);

	diff = max - min;
	return diff;
}

//get maximum value inside array
uint16_t rc_array_max(uint16_t arr[], uint8_t arr_size){

	uint16_t max_element = arr[0];
	
	for (int i = 0; i < arr_size; i++){
	
		if(arr[i] > max_element){
		
			max_element = arr[i]; 

		}
	}
	return max_element;
}

//get minimum value inside array
uint16_t rc_array_min(uint16_t arr[], uint8_t arr_size){

	uint16_t min_element = arr[0];
	
	for (int i = 0; i < arr_size; i++){
	
		if(arr[i] > min_element){
		
			min_element = arr[i]; 

		}
	}
	return min_element;
}


void rc_init(){
	PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan

	PCMSK0 |= (1 << PCINT2) | (1 << PCINT1);  

}



ISR(PCINT0_vect){
	
	current_time = micro_get();
	
	//PINB1 interrupt
	if(PINB & 0b00000010){                                       
		if(last_channel_1 == 0){                                  
			last_channel_1 = 1;                                     
			timer_1 = current_time;                                 
    		}
	}
	else if(last_channel_1 == 1){                                
		last_channel_1 = 0;                                        
		receiver_input_channel_1 = current_time - timer_1;         
		ch1_change = 1;
	}

	//PINB2 interrupt
	if(PINB & 0b00000100){                                       
		if(last_channel_2 == 0){                                  
			last_channel_2 = 1;                                     
			timer_2 = current_time;                                 
    		}
	}
	else if(last_channel_2 == 1){                                
		last_channel_2 = 0;                                        
		receiver_input_channel_2 = current_time - timer_2;         
		ch2_change = 1;
	}
	

	
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


