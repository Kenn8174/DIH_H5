/*
* Temperatur.c
*
* Created: 04-11-2020 12:48:45
*  Author: Kenneth
*/

#include <util/delay.h>

#include <stdbool.h>					/* Include standard boolean library */
#include <string.h>						/* Include string library */
#include <stdio.h>						/* Include standard IO library */
#include <stdlib.h>						/* Include standard library */
#include <avr/io.h>

#include "lcd/lcd.h"

#include "ColumnScan/ColumnScan.h"
#include "ReadRow/ReadRow.h"

#include "DHT11/DHT11_Master.h"
#include "ESP8266/ESP8266_Master.h"

void Temperatur(void)
{
	_delay_ms(1000);
	
	unsigned int column = 1;
	unsigned int row = 90;
	
	Temperatur_choices();
	
	while (1)
	{
		
		column = ColumnScan(column);
		row = ReadRow();
		
		Temperatur_decoder(column, row);
		
		if (column + row == 11)
		{
			main();
		}
		
		_delay_ms(25);
	}
	
}

void Temperatur_choices(void)
{
	Pre_display();

	lcd_puts("Auto: A");
	lcd_gotoxy(0,1);
	lcd_puts("User: B");
	lcd_gotoxy(15, 1);
}

void Temperatur_decoder(int column, int row)
{
	unsigned int choose = column + row;
	
	switch (choose)
	{
		case 44:
		//Temp_auto();
		Temperatur_Read();
		break;
		
		case 34:
		//Temp_user();
		break;
		
		default:
		/* Your code here */
		break;
	}
}

void Temperatur_Read(void)
{
	char buffer[8];
	
	char temp[2];
	
	DHT_WakeUp();
	int array[5][8];
	if (DHT_Response())
	{
		DHT_Decode_Data(array);
	}
	
	Pre_display();
	
	sprintf(buffer, "Temp: %d", ConvertToDecimal(array, 3));

	lcd_puts(buffer);
	lcd_gotoxy(0,1);
	
	sprintf(temp, "%d", ConvertToDecimal(array, 3));
	
	PushToAPI(temp);
	
	unsigned int column = 1;
	unsigned int row = 90;
	
	while(1)
	{
		column = ColumnScan(column);
		row = ReadRow();
		
		if (column + row == 11)
		{
			main();
		}
		
	}
}

void PushToAPI(char temperatur[2])
{	
	char _buffer[150];
	uint8_t Connect_Status;
	
	while(!ESP8266_Begin());
	lcd_puts("test");
	ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);					/* 3 = Both (AP and STA) */
	ESP8266_ConnectionMode(SINGLE);									/* 0 = Single; 1 = Multi */
	ESP8266_ApplicationMode(NORMAL);								/* 0 = Normal Mode; 1 = Transperant Mode */
	if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)			// If not connected to WIFI and API, create a connection
	{
		ESP8266_JoinAccessPoint(SSID, PASSWORD);
	}
	
	if(Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)			// If not connected to the API
	{
		ESP8266_Start(0, DOMAIN, PORT);								// Connect to API
	}

	#ifdef SEND_DEMO												// Demo for sending data to an API
	memset(_buffer, 0, 150);
	// Sends out the url to the API with the Temp and Hum data that was read from the DHT11
	sprintf(_buffer, "GET /update?api_key=%s&field1=%d", API_WRITE_KEY, temperatur);
	printf(_buffer);
	ESP8266_Send(_buffer);
	_delay_ms(15000);												/* Thingspeak server delay */
	#endif
	
	lcd_puts("Data sendt");
}