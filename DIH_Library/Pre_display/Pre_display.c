/*
 * lcd_preDisplay.c
 *
 * Created: 04-06-2019 09:10:48
 *  Author: kenne
 */ 

#include "../lcd/lcd.h"

///Pre_display bliver kørt hver gang at LCD displayed skal bruges, dvs. den laver en clear screen og sætter LCD_DISP_ON_CURSOR_BLINK.
void Pre_display()
{
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	lcd_clrscr();
}