/*
 * DIH_H5.c
 *
 * Created: 04-11-2020 12:23:13
 * Author : Kenneth
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>				/* Include avr interrupt header file */

#include "Temperatur/Temperatur.h"

#include "Init/Init.h"
#include "Init/USART_RS232_H_file.h"

#include "ColumnScan/ColumnScan.h"
#include "ReadRow/ReadRow.h"

#include "lcd/lcd.h"
#include "Pre_display/Pre_display.h"

#include "I2C/i2cmaster.h"
#include "DS3231/ds3231.h"


int main(void)
{
	Init();
	i2c_init();
	init_ds3231();
	USART_Init(115200);
	sei();															/* Start global interrupt */
	
	
    while (1) 
    {
		Menu();
    }
}

void Menu(void)
{
	_delay_ms(1000);
	unsigned int column = 1;
	unsigned int row = 90;
	
	Menu_choices();
	
	while (1)
	{
		column = ColumnScan(column);
		row = ReadRow();
		
		Menu_decoder(column, row);
		
		_delay_ms(25);
	}
	
}

void Menu_choices(void)
{
	Pre_display();
	
	lcd_puts("Date");
	lcd_gotoxy(10,0);
	lcd_puts(": A\n");
	lcd_puts("Temperatur: B");
	lcd_gotoxy(15, 1);
}

void Menu_decoder(int column, int row)
{
	unsigned int chosen = column + row;
	
	switch (chosen)
	{
		case 44:
		//Afstand();
		//Date();
		main();
		break;
		
		case 34:
		//Temp_menu();
		Temperatur();
		break;
		
		default:
		
		break;
	}
}