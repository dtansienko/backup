#ifndef _SH1106_H
#define _SH1106_H

#define _DIS_ON 0xA5
#define SH1106_DIS_OFF 0xA4



#define SH1106_I2C_ADDRESS 0x78
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_DISPLAYALLON_RESUME 0xA4 
#define SH1106_MESSAGETYPE_COMMAND 0x80 // 
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB
#define SH1106_SETCONTRAST 0x81
#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETCOLUMNADDRLOW 0x00
#define SH1106_SETCOLUMNADDRHIGH 0x10

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
//#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0xB0

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

#define SH1106_MESSAGETYPE_COMMAND 0x80
#define SH1106_MESSAGETYPE_DATA 0x40

#define SH1106_READMODIFYWRITE_START 0xE0
#define SH1106_READMODIFYWRITE_END 0xEE

#define SH1106_MAXSEGMENTSPERWRITE 16
#define SH1106_ROWHEIGHT 8
#define SIZE_OF_ASCII_CHAR 5 // the ASCII chars are 5 bytes


// the min addressable columb address is 2
// " max  " " " is 129
// some columbs are missing the the oled
#define SH1106_MIN_X 2
#define SH1106_MAX_X 129
#define SH1106_PAGE_COUNT 8


//function definitions
void init_SH1106();
void sendCommand_SH1106(uint8_t command);
void sendData_SH1106(uint8_t data);
void display_off_SH1106();
void display_on_SH1106();
void draw_column_SH1106(uint8_t x, uint8_t page, uint8_t data);
void draw_char_SH1106(uint8_t x, uint8_t page, uint8_t character, uint8_t flip);


#endif



#define SH1106_NUMBEROF_PAGES 8
