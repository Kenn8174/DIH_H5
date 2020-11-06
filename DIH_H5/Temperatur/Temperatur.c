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

#include "../setup/stdio_setup.h"

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

	lcd_puts("Auto update:");
	lcd_gotoxy(13,0);
	lcd_puts("1");
	
	lcd_gotoxy(0,1);
	
	lcd_puts("Read Once:");
	lcd_gotoxy(13, 1);
	lcd_puts("2");
	
	lcd_gotoxy(15, 1);
}

void Temperatur_decoder(int column, int row)
{
	unsigned int choose = column + row;
	
	switch (choose)
	{
		case 41:
		Temperatur_ReadOnce(1);
		break;
		
		case 42:
		Temperatur_ReadOnce(0);
		break;
		
		default:
		/* Your code here */
		break;
	}
}

void Temperatur_ReadOnce(int loop)
{
	unsigned int column = 1;
	unsigned int row = 90;
	
	
	DHT_WakeUp();														/* Sender et signal ud til DHT11'eren for at se om der er forbindelse */
	int array[5][8];
	if (DHT_Response())													/* Hvis der er forbindelse til en DHT11'ere, så bliver koden i IF'en kørt */
	{
		DHT_Decode_Data(array);											/* Henter dataen fra DHT11'eren og gemmer den i et MULTIDIMENSIONELT array */
		
		int temp = ConvertToDecimal(array, 3);							/* Temperaturen og fugtigheden bliver gemt i en variabel, */
		int hum = ConvertToDecimal(array, 1);							/* men inden skal den lige konventeres til decimal */
		
		Temperatur_display(temp, hum);									/* Displayer temperaturen og fugtigheden på LCD displayet */
		
		PushToAPI(temp, hum);											/* Smider data ud mod Thingspeaks api, så det kan vises på en graf */
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
}

void PushToAPI(int temperatur, int humidity)
{
	char _buffer[150];
	uint8_t Connect_Status;
	while(!ESP8266_Begin());
	ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);						/* 3 = Both (AP and STA) */
	ESP8266_ConnectionMode(SINGLE);										/* 0 = Single; 1 = Multi */
	ESP8266_ApplicationMode(NORMAL);									/* 0 = Normal Mode; 1 = Transperant Mode */
	while(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)			// Mens ESP8266 ikke er tilsluttet til nettet, prøver den at oprette en forbindelse
	{
		ESP8266_JoinAccessPoint(SSID, PASSWORD);
	}
	ESP8266_Start(0, DOMAIN, PORT);										// Connect to API

	memset(_buffer, 0, 150);
	// Sender det nødvendige data, temperatur og fugtighed, til Thingspeaks API
	sprintf(_buffer, "GET /update?api_key=%s&field7=%i&field8=%i", API_WRITE_KEY, temperatur, humidity);
	ESP8266_Send(_buffer);
	_delay_ms(15000);													/* Thingspeak server delay */	
}

void Temperatur_display(int temperatur, int humidity)
{
	char buffer[8];
	Pre_display();
	
	sprintf(buffer, "Temp: %d", temperatur);

	lcd_puts(buffer);
	lcd_gotoxy(0,1);
	
	sprintf(buffer, "Hum : %d", humidity);
	lcd_puts(buffer);
}