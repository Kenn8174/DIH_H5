/*
 * DHT11_Master.c
 *
 * Created: 04-11-2020 12:49:28
 *  Author: Kenneth
 */ 

#include "DHT11_Master.h"
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>

void DHT_WakeUp(void)
{
	clear_bit(DHT_PORT, DHT_BIT);											/* Sætter biten til OUTPUT */
	set_bit(DHT_DDR, DHT_BIT);												/* Sætter biten til logisk HØJ */
	_delay_ms(18);															/* Venter 18 mikrosekunder for at give bedsked til DHT11'eren at der sker noget */
	set_bit(DHT_PORT, DHT_BIT);												/* Sætter biten til logisk LAV */
	clear_bit(DHT_DDR, DHT_BIT);											/* Sætter biten til INPUT */
}

int DHT_Response(void)
{
	double counter = 0;
	
	while(bit_is_set(DHT_PIN, DHT_BIT))										/* Venter på at DHT11'eren giver svar */
	{
		_delay_us(2);
		counter += 2;
		
		if (counter >= 60)													/* Hvis det tager over 60 mikrosekunder og DHT11'eren ikke svarer, sker der en connection timeout, derefter bliver der returneret 0 */
		{
			return 0;
		}
	}
	
	while(bit_is_clear(DHT_PIN, DHT_BIT))									/* Venter på at DHT11'eren er færdig med at give et svar */
	{
		
	}
	
	while(bit_is_set(DHT_PIN, DHT_BIT))										/* Når DHT11'eren har svaret bliver der retuneret 1 */
	{
		return 1;
	}
}

//Decodes the data that the DHT11 sends to the MCU
void DHT_Decode_Data(int array[8])
{
	while(bit_is_set(DHT_PIN, DHT_BIT))										/* Venter på at DHT11'ereb sender data */
	{
		if(bit_is_clear(DHT_PIN, DHT_BIT))									/* Stopper while løkken når der ikke er mere at sende */
		{			
			break;
		}
	}
	
	int i;
	int j;
													
	for (i = 0; i < 5; i++)													/* Looper igennem hvert enkelt byte */
	{
													
		for (j = 0; j < 8; j++)												/* Looper igennem hvert enkelt bit */
		{
			int timer = 0;
			
			while(bit_is_clear(DHT_PIN, DHT_BIT))							/* Mens biten er logisk LAV */
			{
				
			}
						
			while(bit_is_set(DHT_PIN, DHT_BIT))								/* Mens biten er logisk HØJ, begynd timer */
			{
				_delay_us(2);
				timer += 2;
			}
			
			if (timer < 23 || timer == NULL)								/* Hvis biten er logisk høj i mindre end 23 mikrosekunder */
			{
				array[i][j] = 0;
			}
			
			if (timer > 60)													/* Hvis biten er logisk høj i mere end 60 us*/
			{
				array[i][j] = 1;
			}
		}
	}
}

//Converts the bytes in the array to Decimal
int ConvertToDecimal(int array[8], int byte)
{
	int multiplier = 1, output = 0;
	//Runs through each bit in the selected byte LSB First
	for(int i = 7; i >= 0; i--)
	{
		output += (multiplier * array[byte - 1][i]);
		//Decides what the bit will be multiplied by: 1->2->4->8->16->32->64->128
		multiplier *= 2;
	}
	return output;
}