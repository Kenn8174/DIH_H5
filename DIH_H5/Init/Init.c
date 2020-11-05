/*
 * Init.c
 *
 * Created: 04-11-2020 12:24:15
 *  Author: Kenneth
 */ 

#include <stdio.h>
#include <avr/io.h>

void Init(void)
{
	/* Matrix Keyboard */
	// Column sat til høj
	DDRK |= (1<<PK0);
	DDRK |= (1<<PK1);
	DDRK |= (1<<PK2);
	DDRK |= (1<<PK3);

	// Row sat til input
	PORTK |= (1<<PK4);
	PORTK |= (1<<PK5);
	PORTK |= (1<<PK6);
	PORTK |= (1<<PK7);
}