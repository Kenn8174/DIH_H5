/*
 * ReadRow.c
 *
 * Created: 29-05-2019 10:29:21
 *  Author: kenne
 */ 

//#include <stdio.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
//#include "stdio_setup.h"


///ReadRow
/** ReadRow tjekker om en knap i en bestemt række bliver trykket på. Derefter returner den et nummer som så bliver brugt til at decoding af hvad der er blevet trykket på.
(f.eks. Den øverste række er 40, dvs. knap 43 er Række 4 (40) og Kolonne 3 (4) som så er [3] på Keyboardet */
int ReadRow(){
	_delay_ms(10);
	if (!(PINK & (1<<PINK4)))
	{
		return 10;
	}
	if (!(PINK & (1<<PINK5)))
	{
		return 20;
	}
	if (!(PINK & (1<<PINK6)))
	{
		return 30;
	}
	if (!(PINK & (1<<PINK7)))
	{
		return 40;
	}
	return 90;
}