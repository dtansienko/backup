#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "defs.h" // for F_CPU
#include "micros.h"


volatile unsigned long timer0_overflow_count = 0;


void micro_timer_init(){
	TCCR0B |= ((1<<CS01) | (1<<CS00)); //64 prescale
	TIMSK0 |= (1<<TOIE0);
}



ISR(TIMER0_OVF_vect){
	timer0_overflow_count++;
}

unsigned long micro_get(){

	unsigned long m;
	uint8_t oldSREG = SREG;
	uint8_t t;
	
	//cli();
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		m = timer0_overflow_count;

		t = TCNT0;
		
		if ((TIFR0 & _BV(TOV0)) && (t < 255)) {
			m++;
		}
	}
	//SREG = oldSREG;
	return (((m << 8) + t) * 8);
}


