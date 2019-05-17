#include "include/1602.h"
#include "include/1302.h"
#include "include/DHT11.h"
#include "include/key.h"

uchar second, minute, hour, week, day, month, year;
bit displayFlag = 0, setFlag = 0;

/*******读取时间函数**********/
uchar readSecond()
{
	uchar dat;
	dat = DS1302SingleRead(0);
	second = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return second;
}
uchar readMinute()
{
	uchar dat;
	dat = DS1302SingleRead(1);
	minute = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return minute;
}
uchar readHour()
{
	uchar dat;
	dat = DS1302SingleRead(2);
	hour = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return hour;
}
uchar readDay()
{
	uchar dat;
	dat = DS1302SingleRead(3);
	day = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return day;
}
uchar readMonth()
{
	uchar dat;
	dat = DS1302SingleRead(4);
	month = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return month;
}
uchar readWeek()
{
	uchar dat;
	dat = DS1302SingleRead(5);
	week = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return week;
}
uchar readYear()
{
	uchar dat;
	dat = DS1302SingleRead(6);
	year = ((dat & 0x70) >> 4) * 10 + (dat & 0x0f);
	return year;
}
uchar readTime()
{
	readSecond();
	readMinute();
	readHour();
	readDay();
	readMonth();
	readWeek();
	readYear();
	return second, minute, hour, week, day, month, year;
}
/*******显示时间函数**********/
void showSecond()
{
	uchar ten, unit;
	ten = second / 10;
	unit = second % 10;
	LcdWrite(0x80 + 0x40 + 10, 0x30 + ten);
	LcdWrite(0x80 + 0x40 + 11, 0x30 + unit);
}

void showMinute()
{
	uchar ten, unit;
	ten = minute / 10;
	unit = minute % 10;
	LcdWrite(0x80 + 0x40 + 7, 0x30 + ten);
	LcdWrite(0x80 + 0x40 + 8, 0x30 + unit);
	LcdWrite(0x80 + 0x40 + 9, ':');
}
void showHour()
{
	uchar ten, unit;
	ten = hour / 10;
	unit = hour % 10;
	LcdWrite(0x80 + 0x40 + 4, 0x30 + ten);
	LcdWrite(0x80 + 0x40 + 5, 0x30 + unit);
	LcdWrite(0x80 + 0x40 + 6, ':');
}
void showDay()
{
	uchar ten, unit;
	ten = day / 10;
	unit = day % 10;
	LcdWrite(0x80 + 11, 0x30 + ten);
	LcdWrite(0x80 + 12, 0x30 + unit);
}
void showMonth()
{
	uchar ten, unit;
	ten = month / 10;
	unit = month % 10;
	LcdWrite(0x80 + 8, 0x30 + ten);
	LcdWrite(0x80 + 9, 0x30 + unit);
	LcdWrite(0x80 + 10, '-');
}
void showYear()
{
	uchar ten, unit;
	ten = year / 10;
	unit = year % 10;
	LcdWrite(0x80 + 3, '2');
	LcdWrite(0x80 + 4, '0');
	LcdWrite(0x80 + 5, 0x30 + ten);
	LcdWrite(0x80 + 6, 0x30 + unit);
	LcdWrite(0x80 + 7, '-');
}
void showTime()
{
	readTime();
	showSecond();
	showMinute();
	showHour();
	showDay();
	showMonth();
	showYear();
}
/*******显示温湿度函数******/
void showHT()
{
	uchar DHT[4];
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;

	LcdWrite(0x80 + 5, 'H');
	LcdWrite(0x80 + 6, ':');

	LcdWrite(0x80 + 0x40 + 5, 'T');
	LcdWrite(0x80 + 0x40 + 6, ':');

	delay_ms(20);

	DHT11_start();
	if (Data == 0)
	{
		while (Data == 0)
			;					   //等待拉高
		delay_us(40);			   //拉高后延时80us
		R_H = DHT11_rec_byte();	//接收湿度高八位
		R_L = DHT11_rec_byte();	//接收湿度低八位
		T_H = DHT11_rec_byte();	//接收温度高八位
		T_L = DHT11_rec_byte();	//接收温度低八位
		revise = DHT11_rec_byte(); //接收校正位

		delay_us(25); //结束

		if ((R_H + R_L + T_H + T_L) == revise) //校正
		{
			RH = R_H;
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}

		/*数据处理，方便显示*/
		DHT[0] = '0' + (RH / 10);
		DHT[1] = '0' + (RH % 10);

		DHT[2] = '0' + (TH / 10);
		DHT[3] = '0' + (TH % 10);
	}

	LcdWrite(0x80 + 8, DHT[0]);
	LcdWrite(0x80 + 9, DHT[1]);
	LcdWrite(0x80 + 10, ' ');
	LcdWrite(0x80 + 11, '%');

	LcdWrite(0x80 + 0x40 + 8, DHT[2]);
	LcdWrite(0x80 + 0x40 + 9, DHT[3]);
	LcdWrite(0x80 + 0x40 + 10, 0xdf); //显示符号°
	LcdWrite(0x80 + 0x40 + 11, 'C');
}

/*************主函数****************/
void main()
{
	InitDS1302();
	InitLcd1602();
	showTime();
	while (1)
	{
		/*按下K3，切换到显示温湿度*/
		if (K3 == 0)
		{
			delay_ms(10);
			if (K3 == 0)
			{
				displayFlag = ~displayFlag;
				LcdWriteCmd(0x01);
			}
			while (!K3)
				;
			delay_ms(10);
			while (!K3)
				;
		}
		/*根据标记flag判断，双数显示时间，单数显示温湿度*/
		if (displayFlag == 0)
		{
			showTime();
		}
		else
		{
			showHT();
		}
	}
}