#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "./defs.h" // for F_CPU
#include "./micro.h"

//when the counter compares with the value set, 100uS have passed

static volatile uint32_t millisec;


void timer_init(){
	TCCR0A |= (1<<WGM01); // set CTC mode
	TCCR0B |= ((1<<CS01) | (1<<CS00)); //set counting clock to F_CPU / 8 -> 1MHZ
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 125; // set counter compare val,we count from 0-99 w 1MHZ CLK
}



uint32_t micros_get(){
	uint32_t ms;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		ms = millisec;
	}
	return ms;
}

ISR(TIMER0_COMPA_vect){
	++millisec;
	
}
