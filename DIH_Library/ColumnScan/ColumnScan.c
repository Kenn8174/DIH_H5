/*
 * ColumnScan.c
 *
 * Created: 29-05-2019 10:28:51
 *  Author: kenneth
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define col4 0b11111110
#define col3 0b11111101
#define col2 0b11111011
#define col1 0b11110111

#define reset_col 0b11110000

char reset_char;

///ColumnScan kører alle Columns (Kolonner) igennem og sætter en efter en til lav, en efter en. Inden den næste Kollonne køres igennem, bliver alle kolonner sat til høj for at resette det.
int ColumnScan(int column){
	
	switch (column)
	{
		case 1:
		reset_char = PORTK & reset_col;
		reset_char |= col4;				// 1111 1110
		PORTK = reset_char;
		return 4;
		break;
		
		case 4:
		reset_char = PORTK & reset_col;
		reset_char |= col3;				// 1111 1011
		PORTK = reset_char;
		return 3;
		break;
		
		case 3:
		reset_char = PORTK & reset_col;
		reset_char |= col2;				// 1110 1111
		PORTK = reset_char;
		return 2;
		break;
		
		case 2:
		reset_char = PORTK & reset_col;
		reset_char |= col1;				// 1011 1111
		PORTK = reset_char;
		return 1;
		break;
	}
	
}