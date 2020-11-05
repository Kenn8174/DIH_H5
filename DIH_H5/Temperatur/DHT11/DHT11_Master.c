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

//Used for the initial Wakeup/connection between the DHT11 and the MCU
void DHT_WakeUp(void)
{
	clear_bit(DHT_PORT, DHT_BIT);	//Pulls the bit HIGH
	set_bit(DHT_DDR, DHT_BIT);		//Pulls the bit LOW
	_delay_ms(18);					//Delay to let the DHT Detect the signal
	set_bit(DHT_PORT, DHT_BIT);		//Enabels buildin Pullup resistance
	clear_bit(DHT_DDR, DHT_BIT);	//Clears the bit so it will go HIGH via the pullup
}

//Checks if the DHT Responds
int DHT_Response(void)
{
	double counter = 0;
	//Waits for the DHT11 to response
	while(bit_is_set(DHT_PIN, DHT_BIT))
	{
		_delay_us(2);
		counter += 2;
		//If the DHT11 takes over 60 us the connection timeouts
		if (counter >= 60)
		return 0;
	}
	//Waits for the DHT11 response to end
	while(bit_is_clear(DHT_PIN, DHT_BIT))
	{
	}
	//Returns when the DHT11 gets ready to transmitt data
	while(bit_is_set(DHT_PIN, DHT_BIT))
	{
		return 1;
	}
}

//Decodes the data that the DHT11 sends to the MCU
void DHT_Decode_Data(int (*array)[8])
{
	//Waits for the DHT11 to start transmitting
	while(bit_is_set(DHT_PIN, DHT_BIT))
	{
		//Breaks when the DHT11 start transmitting
		if(bit_is_clear(DHT_PIN, DHT_BIT))
		break;
	}
	int i;
	int j;
	//Loop for each byte
	for (i = 0; i < 5; i++){
		//Loop for each bit
		for (j = 0; j < 8; j++){
			int timer = 0;
			//Waits for the LOW part of the transmission to end
			while(bit_is_clear(DHT_PIN, DHT_BIT)){}
			
			//Begins counting the time the bit is HIGH
			while(bit_is_set(DHT_PIN, DHT_BIT)){
				_delay_us(2);
				timer += 2;
			}
			//If the bit is HIGH for less than 23us the bit is Logic 0/LOW
			if (timer < 23 || timer == NULL)
			{
				array[i][j] = 0;
			}
			//If the bit is HIGH for more than 60us the bit is Logic 1/HIGH
			if (timer > 60)
			{
				array[i][j] = 1;
			}
		}
	}
}

//Converts the bytes in the array to Decimal
int ConvertToDecimal(int (*array)[8], int byte)
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