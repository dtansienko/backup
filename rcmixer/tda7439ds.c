#include <avr/io.h>
#include "tda7439ds.h"
#include "I2C.h"


// valid input 0-48
void tda7439ds_set_volume(uint8_t vol) {
	uint8_t actual_vol;
	
	//generate MUTE function
	if (vol == 0){
		tda7439ds_mute();
	}
	
	actual_vol = 48 - vol;
	
	tda7439ds_send_command(TDA7439_VOLUME, actual_vol);

}

//input is -7 <-> 7, 0 is 0db
void tda7439ds_set_bass(int bass) {
	uint8_t actual_bass; 
	//check vaid input
	if((bass > 7) || (bass < -7)){
		return;
	}
	
	
	if (bass < 0){
		actual_bass = 7; 
		actual_bass += bass;
	}
	
	else if (bass > 0){
		actual_bass = 16;
		actual_bass -= bass;
	}
	else if (bass == 0) {
		actual_bass = 7;
	}
	
	tda7439ds_send_command(TDA7439_BASS_GAIN, actual_bass);
}

void tda7439ds_set_mid(int mid) {
	uint8_t actual_mid; 
	//check vaid input
	if((mid > 7) || (mid < -7)){
		return;
	}
	
	
	if (mid < 0){
		actual_mid = 7; 
		actual_mid += mid;
	}
	
	else if (mid > 0){
		actual_mid = 16;
		actual_mid -= mid;
	}
	else if (mid == 0) {
		actual_mid = 7;
	}
	
	tda7439ds_send_command(TDA7439_MID_GAIN, actual_mid);
}

void tda7439ds_set_trebel(int trebel) {
	uint8_t actual_trebel; 
	//check vaid input
	if((trebel > 7) || (trebel < -7)){
		return;
	}
	
	
	if (trebel < 0){
		actual_trebel = 7; 
		actual_trebel += trebel;
	}
	
	else if (trebel > 0){
		actual_trebel = 16;
		actual_trebel -= trebel;
	}
	else if (trebel == 0) {
		actual_trebel = 7;
	}
	
	tda7439ds_send_command(TDA7439_TREBEL_GAIN, actual_trebel);
}



void tda7439ds_set_input_gain(uint8_t input_gain) {
	if (input_gain > 16){
	
		return;
	}
	
	tda7439ds_send_command(TDA7439_INPUT_GAIN, input_gain);
}

void tda7439ds_mute() {

	tda7439ds_send_command(TDA7439_VOLUME, MUTE);

}

void tda7439ds_input_selector(uint8_t input) {

	tda7439ds_send_command(TDA7439_INPUT_SELECTOR, input);

}





void tda7439ds_send_command(uint8_t sub_addr, uint8_t data) {
	
	//generate communication sequence
	i2c_start(TDA7439_ADDR);
	i2c_send(sub_addr);
	i2c_send(data);
	i2c_stop();

}
