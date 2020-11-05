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
	Init();															/* Brugerdefineret initialiseringer */
	//i2c_init();													/* Initialisere I2C, BLIVER IKKE BRUGT */
	//init_ds3231();												/* Initialisere DS3231, BLIVER IKKE BRUGT */
	USART_Init(115200);												/* Initialisere USART og sætter BAUD raten til 115200 */
	sei();															/* Starter global interrupt */
	
	
    while (1) 
    {
		Menu();														/* Kører methoden Menu(), som viser og gør at brugeren har en menu som kan bruges */
    }
}

void Menu(void)
{
	_delay_ms(1000);
	unsigned int column = 1;
	unsigned int row = 90;
	
	Menu_choices();
	
	while (1)													   /* While løkke som kører indtil brugeren har entent trykket på "A" eller "B" knappen */
	{
		column = ColumnScan(column);
		row = ReadRow();
		
		Menu_decoder(column, row);
		
		_delay_ms(25);
	}
	
}

void Menu_choices(void)												/* Viser menuen på LCD displayet */
{
	Pre_display();
	
	lcd_puts("Date");
	lcd_gotoxy(10,0);
	lcd_puts(": A\n");
	lcd_puts("Temperatur: B");
	lcd_gotoxy(15, 1);
}

void Menu_decoder(int column, int row)								/* Decoder, til valg i menuen. Dette er brugt fra det tidligere projekt, hvor det var meningen at man skulle måle afstand, */
{																	/* det kom dsv. ikke til at ske, og i dette projekt, ligesom det tidligere, bliver det kun brugt til temperatur */
	unsigned int chosen = column + row;
	
	switch (chosen)
	{
		case 44:
		//Afstand();
		//Date();
		main();														/* Hvis "A" knappen er valgt bliver brugeren smidt tilbage til menuen */
		break;
		
		case 34:
		//Temp_menu();
		Temperatur();												/* Hvis "B" knappen er valgt bliver brugeren smidt til temperatur menuen */
		break;
		
		default:
		
		break;
	}
}