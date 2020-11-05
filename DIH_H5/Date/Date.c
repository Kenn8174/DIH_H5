/*
* Date.c
*
* Created: 05-11-2020 12:52:56
*  Author: Kenneth
*/
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "DS3231/ds3231.h"
#include "I2C/i2cmaster.h"
#include "ColumnScan/ColumnScan.h"
#include "ReadRow/ReadRow.h"
#include "Pre_display/Pre_display.h"

struct DateTime {
	char hours[3];
	char minutes[3];
	char seconds[3];
	char years[3];
	char months[3];
	char days[3];
};

char times[64];
char date[64];

rtc_t today;

void Date(void)
{
	_delay_ms(1000);
	
	unsigned int column = 1;
	unsigned int row = 90;
	
	rtc_t rtc;
	
	rtc.hour = dec2bcd(13);
	rtc.min =  dec2bcd(02);
	rtc.sec =  dec2bcd(00);
	rtc.date = dec2bcd(05);  // 28
	rtc.month = dec2bcd(11); // 10
	rtc.year = dec2bcd(20);  // 2020
	rtc.weekDay = 4;         // Wednesday: 3rd day of week considering Monday as first day.
	
	ds3231_SetDateTime(&rtc);
	
	getDateTime();
	
	while (1)
	{
		column = ColumnScan(column);
		row = ReadRow();
		
		if (column + row == 11)
		{
			main();
		}
	}
}

void getDateTime(void)
{
	struct DateTime dateAndTime;
	
	ds3231_GetDateTime(&today);
	
	//Converts the Binary Coded Decimal (BCD) to Decimal value and converts it to a number
	
	itoa(bcd2dec(today.hour) , dateAndTime.hours, 10); //hour
	itoa(bcd2dec(today.min), dateAndTime.minutes, 10);  //min
	itoa(bcd2dec(today.sec), dateAndTime.seconds, 10);   //sec
	itoa(bcd2dec(today.date), dateAndTime.days, 10);  //day
	itoa(bcd2dec(today.month), dateAndTime.months, 10); //month
	itoa(bcd2dec(today.year), dateAndTime.years, 10);  //year
	
	strcat(times,"Time: ");
	strcat(times,dateAndTime.hours);   //hour
	strcat(times,":");

	strcat(times,dateAndTime.minutes);  //min
	strcat(times,":");
	
	strcat(times,dateAndTime.seconds);   //sec
	strcat(date," Date: ");
	
	strcat(date,dateAndTime.days);   //day
	strcat(date,"/");
	
	strcat(date,dateAndTime.months);   //month
	strcat(date,"/20");
	
	strcat(date,dateAndTime.years);   //year
	
	Pre_display();
	lcd_puts(times);
	lcd_gotoxy(0,1);
	lcd_puts(date);
	
}