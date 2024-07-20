#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./I2C.h"
#include "./USART.h"
#include "./defs.h"
#include "./SH1106.h"
#include "./5x7font.h"




void init_SH1106() {

	send_command_SH1106(SH1106_DISPLAYOFF);                    // 0xAE
	send_command_SH1106(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
	send_command_SH1106(0x80);                                  // the suggested ratio 0x80
	send_command_SH1106(SH1106_SETMULTIPLEX);                  // 0xA8
	send_command_SH1106(0x3F);
	send_command_SH1106(SH1106_SETDISPLAYOFFSET);              // 0xD3
	send_command_SH1106(0x00);                                   // no offset

	send_command_SH1106(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
	send_command_SH1106(SH1106_CHARGEPUMP);                    // 0x8D
	// SH1106_EXTERNALVCC
	send_command_SH1106(0x10);
	send_command_SH1106(SH1106_MEMORYMODE);                    // 0x20
	send_command_SH1106(0x00);                                  // 0x0 act like ks0108
	send_command_SH1106(SH1106_SEGREMAP | 0x1);
	send_command_SH1106(SH1106_COMSCANDEC);
	send_command_SH1106(SH1106_SETCOMPINS);                    // 0xDA
	send_command_SH1106(0x12);
	send_command_SH1106(SH1106_SETCONTRAST);                   // 0x81
	// SH1106_EXTERNALVCC
	send_command_SH1106(0x9F);
	send_command_SH1106(SH1106_SETPRECHARGE);                  // 0xd9
	// SH1106_EXTERNALVCC
	send_command_SH1106(0x22);
	send_command_SH1106(SH1106_SETVCOMDETECT);                 // 0xDB
	send_command_SH1106(0x40);
	send_command_SH1106(SH1106_DISPLAYALLON_RESUME);           // 0xA4
	send_command_SH1106(SH1106_NORMALDISPLAY);                 // 0xA6

	send_command_SH1106(SH1106_DISPLAYON); //--turn on oled panel

	// ?????
	send_command_SH1106(SH1106_SETCOLUMNADDRLOW | 0x0);  // low col = 0
	send_command_SH1106(SH1106_SETCOLUMNADDRHIGH | 0x0);  // hi col = 0

	send_command_SH1106(SH1106_SETSTARTLINE | 0x0); // line #0
	i2c_start(SH1106_I2C_ADDRESS);
	i2c_send(0xC0); //set com direction
	i2c_stop();
}

void send_command_SH1106(uint8_t command){
	i2c_start(SH1106_I2C_ADDRESS);
	i2c_send(SH1106_MESSAGETYPE_COMMAND);
	i2c_send(command);
	i2c_stop();

}

void send_data_SH1106(uint8_t data){
	i2c_start(SH1106_I2C_ADDRESS);
	i2c_send(SH1106_MESSAGETYPE_DATA);
	i2c_send(data);
	i2c_stop();
}

void display_off_SH1106(){

	//sendCommand_SH1106(SH1106_PAGEADDR); //set the cursor on first page
	send_command_SH1106(0x00); // set 4 lower bits of colum register
	send_command_SH1106(SH1106_SETCOLUMNADDRHIGH); //set the higher 4 bits of colum regsiter
							// at start position
	
	uint8_t currentPage = 0;
	send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
	
	while (currentPage < SH1106_NUMBEROF_PAGES){
	
		send_command_SH1106(SH1106_PAGEADDR + (currentPage & 0x0F)); // move cursor to page
		
		for (int i = 0; i< 132; i++){
		
			send_data_SH1106(0x00); // clear 8 bits, it will auto advance   
			
	       }
	       
       	send_command_SH1106(0x00); // set 4 lower bits of colum register
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH); //set the higher 4 bits of colum regsiter
	       
    		currentPage++;
		
	
	}
	send_command_SH1106(0xEE); // end rmw

}

void display_on_SH1106(){

	//sendCommand_SH1106(SH1106_PAGEADDR); //set the cursor on first page
	send_command_SH1106(0x00); // set 4 lower bits of colum register
	send_command_SH1106(SH1106_SETCOLUMNADDRHIGH); //set the higher 4 bits of colum regsiter
							// at start position
				
	uint8_t currentPage = 0;
	
	while (currentPage < SH1106_NUMBEROF_PAGES){
		send_command_SH1106(SH1106_PAGEADDR + (currentPage & 0x0F)); // move cursor to page
		send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
		
		/*i2c_start(SH1106_I2C_ADDRESS);
		i2c_send(0xC0); //set com direction
		i2c_stop(); */
		
		for (int i = 0; i< 132; i++){
			send_data_SH1106(0xFF); // clear 8 bits, it will auto advance   
	       }
	       
    		send_command_SH1106(0xEE); // end rmw
    		currentPage++;
		
	
	}

}
//TODO --check the x borders
//	--check page borders
void draw_column_SH1106(uint8_t x, uint8_t page, uint8_t data){
	
		//not checking for correct data
		
		send_command_SH1106(SH1106_SETCOLUMNADDRLOW + (x & 0x0F));
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH + ((x & 0xF0) >> 4));
		send_command_SH1106(SH1106_PAGEADDR + (page & 0x0F));
		
		i2c_start(SH1106_I2C_ADDRESS);
		i2c_send(0xC0); //set com direction
		i2c_stop();	
		
		send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
		send_data_SH1106(data);
		send_command_SH1106(0xEE); //end RMW
}

void draw_sprite_SH1106(uint8_t x, uint8_t y, uint8_t character, uint8_t flip){

	if ((x < SH1106_MIN_X) | ((x + SH1106_sprite_width) > SH1106_MAX_X)) return;
	if ((y >> 3) > 7) return; //devide by 8 and check if you're on screen

	
	uint8_t upper_page = y >> 3;	
	
	uint8_t lower_page = (y + 8) >> 3;
	
	uint16_t char_array_buffer = (character - 32) * 5;
	
	if ((lower_page - upper_page) == 1 ) {
		
		uint8_t upper_bit_row = y - (8 * upper_page);
		//UART_sends("upper_bit_row");
		//UART_print_uint(upper_bit_row);
		
		
		
		//uint8_t upper_page_data = character & (0xFF << upper_bit_row);
		//uint8_t lower_page_data = character & (0xFF >> (8 - upper_bit_row));
		
		send_command_SH1106(SH1106_SETCOLUMNADDRLOW + (x & 0x0F));
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH + ((x & 0xF0) >> 4));	
		send_command_SH1106(SH1106_PAGEADDR + (upper_page & 0x0F));
		
		char* p = font5x7;
		p += char_array_buffer;
		
		send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
		
		
		for (int i = 0; i < 5; i++){
			//UART_sends("upper value");
			//UART_print_uint((0xFF & (*p << upper_bit_row)));
			
			if (flip) send_data_SH1106( ~(*p) );
			else send_data_SH1106( 0xFF & (*p << upper_bit_row));
			p++;
		
		}
		
		send_command_SH1106(0xEE); //end RMW
		
		p = font5x7;
		p += char_array_buffer;
		
		send_command_SH1106(SH1106_SETCOLUMNADDRLOW + (x & 0x0F));
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH + ((x & 0xF0) >> 4));	
		send_command_SH1106(SH1106_PAGEADDR + (lower_page & 0x0F));
		
	
		
		send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
		
		for (int i = 0; i < 5; i++){
			//UART_sends("lower value");
			//UART_print_uint((0xFF00 & (*p << upper_bit_row)) >> 4);
			
			if (flip) send_data_SH1106( ~(*p) );
			else send_data_SH1106((0xFF00 & (*p << upper_bit_row)) >> 8);
			p++;
		
		}
		
		send_command_SH1106(0xEE); //end RMW
	}
	
	
	
	
	
	
	
	
	

	
	

}

void draw_char_SH1106(uint8_t x, uint8_t page, uint8_t character, uint8_t flip){
	
	//check page
	if (x < SH1106_MIN_X) return;
	if (page > SH1106_PAGE_COUNT) return;
	if (character > 160) return;
	
	uint8_t space_flag = FALSE;
	
	
	// check if the character can fit, if not move to next page
	if ((x + SIZE_OF_ASCII_CHAR) > SH1106_MAX_X) {
		
		// we don't have another page
		if((page - 1) == SH1106_PAGE_COUNT) return;
		
		space_flag = TRUE;
		send_command_SH1106(SH1106_PAGEADDR + page); 
		
		//set begging of the line 
		send_command_SH1106(SH1106_SETCOLUMNADDRLOW + (SH1106_MIN_X & 0x0F));
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH + ((SH1106_MIN_X & 0xF0) >> 4));
	
	}
	
	// the value of the char is the ASCII number. We need a buffer because we
	// have only ASCII values starting from 32
	uint16_t char_array_buffer = (character - 32) * 5;
	
	// no space, continue normal
	if (space_flag == FALSE) {
	
		send_command_SH1106(SH1106_SETCOLUMNADDRLOW + (x & 0x0F));
		send_command_SH1106(SH1106_SETCOLUMNADDRHIGH + ((x & 0xF0) >> 4));	
		send_command_SH1106(SH1106_PAGEADDR + ((page - 1) & 0x0F));
	}
	

	
	char* p = font5x7;
	p += char_array_buffer;
	
	send_command_SH1106(SH1106_READMODIFYWRITE_START); //RMW
	
	for (int i = 0; i < 5; i++){
		
		if (flip) send_data_SH1106( ~(*p));
		else send_data_SH1106(*p);
		p++;
	
	}
	
	send_command_SH1106(0xEE); //end RMW



}

