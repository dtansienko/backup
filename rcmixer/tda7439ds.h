#ifndef _TDA7439DS_
#define _TDA7439DS_


//all commands don't use address incrementing 
#define TDA7439_ADDR 0x88

#define MUTE 0x38

#define IN4 0
#define IN3 1
#define IN2 2
#define IN1 3

#define TDA7439_INPUT_SELECTOR 0
#define TDA7439_INPUT_GAIN 1
#define TDA7439_VOLUME 2 
#define TDA7439_BASS_GAIN 3
#define TDA7439_MID_GAIN 4
#define TDA7439_TREBEL_GAIN 5
#define TDA7439_SPKR_ATN_R 6
#define TDA7439_SPKR_ATN_L 7


void tda7439ds_set_volume(uint8_t vol);
void tda7439ds_set_bass(int bass);
void tda7439ds_set_mid(int mid);
void tda7439ds_set_trebel(int trebel);
void tda7439ds_mute();
void tda7439ds_send_command(uint8_t sub_addr, uint8_t data);
void tda7439ds_input_selector(uint8_t input);
void tda7439ds_set_input_gain(uint8_t input_gain);


#endif
