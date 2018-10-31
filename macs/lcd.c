/*
 *  lcd.c
 *
 *  Created by Michael Köhler on 22.12.16.
 *  Copyright 2016 Skie-Systems. All rights reserved.
 *
 */
/* Standard ASCII 6x8 font */

#include "lcd.h"

#if PSC_I2C != 1 && PSC_I2C != 4 && PSC_I2C != 16 && PSC_I2C != 64
#error "Wrong prescaler for TWI !"
#elif SET_TWBR < 0 || SET_TWBR > 255
#error "TWBR out of range, change PSC_I2C or F_I2C !"
#endif

static const uint8_t ssd1306oled_font6x8 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, // !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
    0x00, 0x62, 0x64, 0x08, 0x13, 0x23, // %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00, // ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E, // @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C, // A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, // backslash
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x00, 0x01, 0x02, 0x04, 0x00, // '
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, // p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC, // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, // z
	0x00, 0x00, 0x08, 0x77, 0x41, 0x00, // {
	0x00, 0x00, 0x00, 0x63, 0x00, 0x00, // ¦
	0x00, 0x00, 0x41, 0x77, 0x08, 0x00, // }
	0x00, 0x08, 0x04, 0x08, 0x08, 0x04, // ~
};

const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence
0xAE,			// Display OFF (sleep mode)
0x20, 0b00,		// Set Memory Addressing Mode
				// 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
				// 10=Page Addressing Mode (RESET); 11=Invalid
0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
0xC8,			// Set COM Output Scan Direction
0x00,			// --set low column address
0x10,			// --set high column address
0x40,			// --set start line address
0x81, 0x3F,		// Set contrast control register
0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
0xA6,			// Set display mode. A6=Normal; A7=Inverse
0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
0xA4,			// Output RAM to Display
				// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
0xD3, 0x00,		// Set display offset. 00 = no offset
0xD5,			// --set display clock divide ratio/oscillator frequency
0xF0,			// --set divide ratio
0xD9, 0x22,		// Set pre-charge period
0xDA, 0x12,		// Set com pins hardware configuration		
0xDB,			// --set vcomh
0x20,			// 0x20,0.77xVcc
0x8D, 0x14,		// Set DC-DC enable
0xAF,			// Display ON in normal mode

};
void i2c_init(void){
	LCD_PORT |= (1 << SDA_Pin)|(1 << SDC_Pin);		// experimental, pullups for 12c_bus
	LCD_PORT_DDR |= (1 << SDA_Pin)|(1 << SDC_Pin);
	// set clock
	switch (PSC_I2C) {
		case 4:
			TWSR = 0x1;
			break;
		case 16:
			TWSR = 0x2;
			break;
		case 64:
			TWSR = 0x3;
			break;
		default:
			TWSR = 0x00;
			break;
	}
	TWBR = (uint8_t)SET_TWBR;
	// enable
	TWCR = (1 << TWEN);
}
void lcd_send_i2c_start(void){
    // i2c start
    TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
    while((TWCR & (1 << TWINT)) == 0);
    // send adress
    TWDR = LCD_I2C_ADDR;
    TWCR = (1 << TWINT)|( 1 << TWEN);
    while((TWCR & (1 << TWINT)) == 0);
}
void lcd_send_i2c_stop(void){
    // i2c stop
    TWCR = (1 << TWINT)|(1 << TWSTO)|(1 << TWEN);
}
void lcd_send_i2c_byte(uint8_t byte){
    TWDR = byte;
    TWCR = (1 << TWINT)|( 1 << TWEN);
    while((TWCR & (1 << TWINT)) == 0);
}

void lcd_init(uint8_t dispAttr){
	if(LCD_INIT_I2C == YES) i2c_init();
	for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++) {
		lcd_command(pgm_read_byte(&ssd1306_init_sequence[i]));
	}
	lcd_command(dispAttr);
    lcd_clrscr();
}
void lcd_home(void){
	lcd_gotoxy(0, 0);
}

void lcd_command(uint8_t cmd) {
    lcd_send_i2c_start();
    lcd_send_i2c_byte(0x00);	// 0x00 for command, 0x40 for data
    lcd_send_i2c_byte(cmd);
    lcd_send_i2c_stop();
}
void lcd_data(uint8_t data) {
    lcd_send_i2c_start();
    lcd_send_i2c_byte(0x40);	// 0x00 for command, 0x40 for data
    lcd_send_i2c_byte(data);
    lcd_send_i2c_stop();
}
void lcd_gotoxy(uint8_t x, uint8_t y){
	x = x * 6;
    lcd_send_i2c_start();		// start i2c communication
    lcd_send_i2c_byte(0x00);	// 0x00 for command, 0x40 for data
    lcd_send_i2c_byte(0xb0 + y);// set page start to y
	lcd_send_i2c_byte(0x21);	// set column start	
    lcd_send_i2c_byte(x);		// to x
	lcd_send_i2c_byte(0x7f);	// set column end to 127
    lcd_send_i2c_stop();		// stop i2c communication
}
void lcd_clrscr(void){
    lcd_home();
	lcd_send_i2c_start();
    lcd_send_i2c_byte(0x40);	// 0x00 for command, 0x40 for data
    for (uint16_t i = 0; i < 128 * 8 ; i++) {
        lcd_send_i2c_byte(0);	// clear display while printing space
    }
    lcd_send_i2c_stop();
	lcd_home();
}
void lcd_putc(char c){
    uint8_t ch = c - 32;
    lcd_send_i2c_start();
    lcd_send_i2c_byte(0x40);	// 0x00 for command, 0x40 for data
    for (uint8_t i = 0; i < 6; i++)
	{
		lcd_send_i2c_byte(pgm_read_byte(&ssd1306oled_font6x8[ch * 6 + i]));	// print font to ram, print 6 columns
	}
    lcd_send_i2c_stop();
}
void lcd_puts(const char* s){
    while (*s) {
        lcd_putc(*s++);
    }
}
void lcd_puts_p(const char* progmem_s){
    register uint8_t c;
    while ((c = pgm_read_byte(progmem_s++))) {
        lcd_putc(c);
    }
}
void lcd_invert(uint8_t invert){
    lcd_send_i2c_start();
    lcd_send_i2c_byte(0x00);	// 0x00 for command, 0x40 for data
    if (invert == YES) {
        lcd_send_i2c_byte(0xA7);
    } else {
        lcd_send_i2c_byte(0xA6);
    }
    lcd_send_i2c_stop();
}
void lcd_set_contrast(uint8_t contrast){
    lcd_send_i2c_start();
    lcd_send_i2c_byte(0x00);	// 0x00 for command, 0x40 for data
    lcd_send_i2c_byte(0x81);
    lcd_send_i2c_byte(contrast);
    lcd_send_i2c_stop();
}
