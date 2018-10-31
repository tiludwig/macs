/*
 *  lcd.h
 *
 *  Created by Michael Köhler on 22.12.16.
 *  Copyright 2016 Skie-Systems. All rights reserved.
 *
 *  lib for OLED-Display with SSD1306-Controller
 *  first dev-version only for I2C-Connection
 *  at ATMega328P like Arduino Uno or ATMega168PA/88PA/48PA
 *
 */

#ifndef LCD_H
#define LCD_H

#ifndef YES
#define YES				1
#define NO				0
#else
#error "Check #defines for YES and NO in other sources !"
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 303
#error "This library requires AVR-GCC 3.3 or later, update to newer AVR-GCC compiler !"
#endif

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define SDA_Pin			PC4
#define SDC_Pin			PC5
#define LCD_PORT		PORTC
#define LCD_PORT_DDR	DDRC

#define LCD_I2C_ADDR	0x78	// refer lcd-manual, 0x78 for 8-bit-adressmode, 0x3c for 7-bit-adressmode
#define LCD_INIT_I2C	YES		// init I2C via lcd-lib
#define F_I2C			400000UL// clock i2c
#define PSC_I2C			1		// pprescaler i2c
#define SET_TWBR		(F_CPU/F_I2C-16UL)/(PSC_I2C*2UL)

#define LCD_DISP_OFF	0xAE
#define LCD_DISP_ON		0xAF

void i2c_init(void);						// init hw-i2c
void lcd_send_i2c_start(void);				// send i2c_start_condition
void lcd_send_i2c_stop(void);				// send i2c_stop_condition
void lcd_send_i2c_byte(uint8_t byte);		// send data_byte

void lcd_init(uint8_t dispAttr);
void lcd_home(void);

void lcd_command(uint8_t cmd);				// transmit command to display
void lcd_data(uint8_t data);				// transmit data to display
void lcd_gotoxy(uint8_t x, uint8_t y);		// set curser at pos x, y. x means character, y means line (page, refer lcd manual)
void lcd_clrscr(void);						// clear screen
void lcd_putc(char c);						// print character on screen
void lcd_puts(const char* s);				// print string, \n-terminated, from ram on screen
void lcd_puts_p(const char* progmem_s);		// print string from flash on screen
void lcd_invert(uint8_t invert);			// invert display
void lcd_set_contrast(uint8_t contrast);	// set contrast for display
#endif /*  LCD_H  */
