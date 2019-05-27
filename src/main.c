#include "include/1602.h"
#include "include/1302.h"
#include "include/DHT.h"
#include "include/key.h"
#include "include/sound.h"
#include "include/ny3p.h"

uchar second, minute, hour, week, day, month, year, setNum = 0;
bit displayFlag = 0, setFlag = 0;
uint timer0Count = 0; //定时器溢出次数

uint waitTime = 0;
uchar soundState = 0;

int H, T;

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

	showSecond();
	showMinute();
	showHour();
	showDay();
	showMonth();
	showYear();
}
/*******显示温湿度函数******/
void readHT()
{
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
	delay_ms(20);
	DHTStart();
	if (Data == 0)
	{
		while (Data == 0)
			;					//等待拉高
		delay_us(40);			//拉高后延时80us
		R_H = DHTByteRead();	//接收湿度高八位
		R_L = DHTByteRead();	//接收湿度低八位
		T_H = DHTByteRead();	//接收温度高八位
		T_L = DHTByteRead();	//接收温度低八位
		revise = DHTByteRead(); //接收校正位

		delay_us(25); //结束
	}
	delay_ms(20);
	DHTStart();
	if (Data == 0)
	{
		while (Data == 0)
			;					//等待拉高
		delay_us(40);			//拉高后延时80us
		R_H = DHTByteRead();	//接收湿度高八位
		R_L = DHTByteRead();	//接收湿度低八位
		T_H = DHTByteRead();	//接收温度高八位
		T_L = DHTByteRead();	//接收温度低八位
		revise = DHTByteRead(); //接收校正位

		delay_us(25);						   //结束
		if ((R_H + R_L + T_H + T_L) == revise) //校正
		{
			RH = R_H;
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}

		/*DHT11 数据处理，方便显示*/
		// DHT[0] = '0' + (RH / 10);
		// DHT[1] = '0' + (RH % 10);

		// DHT[2] = '0' + (TH / 10);
		// DHT[3] = '0' + (TH % 10);

		/*DHT21 数据处理*/
		H = (RH * 256 + RL) / 10; //DHT21湿度数据格式为16bit，并且是实际湿度的10倍
		T = (TH * 256 + TL) / 10;
	}
}

void showHT()
{
	uchar DHT[4];
	DHT[0] = '0' + (H / 10);
	DHT[1] = '0' + (H % 10);

	DHT[2] = '0' + (T / 10);
	DHT[3] = '0' + (T % 10);

	LcdWrite(0x80 + 5, 'H');
	LcdWrite(0x80 + 6, ':');
	LcdWrite(0x80 + 0x40 + 5, 'T');
	LcdWrite(0x80 + 0x40 + 6, ':');

	LcdWrite(0x80 + 8, DHT[0]);
	LcdWrite(0x80 + 9, DHT[1]);
	LcdWrite(0x80 + 10, ' ');
	LcdWrite(0x80 + 11, '%');

	LcdWrite(0x80 + 0x40 + 8, DHT[2]);
	LcdWrite(0x80 + 0x40 + 9, DHT[3]);
	LcdWrite(0x80 + 0x40 + 10, 0xdf); //显示符号°
	LcdWrite(0x80 + 0x40 + 11, 'C');
}
/***********键盘扫描函数*************/
void setTime()
{
	if (K1 == 0 && displayFlag == 0)
	{
		delay_ms(10);
		if (K1 == 0)
		{
			backlight = 0;
			setNum++;

			TH0 = 0x4C;
			TL0 = 0x00;
			TR0 = 1;
			timer0Count = 0;

			switch (setNum)
			{
			case 1:
				setFlag = 1;
				DS1302SingleWrite(7, 0x00); //去除写保护
				DS1302SingleWrite(0, 0x80); //时钟停下
				LcdWrite(0 + 0x40, setNum);
				LcdWriteCmd(0x80 + 0x40 + 11);
				LcdWriteCmd(0x0f);
				break;
			case 2:
				LcdWriteCmd(0x80 + 0x40 + 8);
				break;
			case 3:
				LcdWriteCmd(0x80 + 0x40 + 5);
				break;
			case 4:
				LcdWriteCmd(0x80 + 12);
				break;
			case 5:
				LcdWriteCmd(0x80 + 9);
				break;
			case 6:
				LcdWriteCmd(0x80 + 6);
				break;
			case 7:
				setNum = 0;
				setFlag = 0;

				LcdWriteCmd(0x0c);
				DS1302SingleWrite(0, 0x00);
				DS1302SingleWrite(0, (second / 10) << 4 | second % 10);
				DS1302SingleWrite(1, (minute / 10) << 4 | minute % 10);
				DS1302SingleWrite(2, (hour / 10) << 4 | hour % 10);
				DS1302SingleWrite(5, (week / 10) << 4 | week % 10);
				DS1302SingleWrite(3, (day / 10) << 4 | day % 10);
				DS1302SingleWrite(4, (month / 10) << 4 | month % 10);
				DS1302SingleWrite(6, (year / 10) << 4 | year % 10);

				DS1302SingleWrite(7, 0x80);
				break;
			}
		}
		while (!K1)
			;
		delay_ms(10);
		while (!K1)
			;
	}
	if (setNum != 0)
	{
		if (K3 == 0)
		{
			delay_ms(10);
			while (!K3)
				;
			switch (setNum)
			{
			case 1:
				second++;
				if (second == 60)
					second = 0;
				showSecond();
				LcdWriteCmd(0x80 + 0x40 + 11);
				break;
			case 2:
				minute++;
				if (minute == 60)
					minute = 0;
				showMinute();
				LcdWriteCmd(0x80 + 0x40 + 8);
				break;
			case 3:
				hour++;
				if (hour == 24)
					hour = 0;
				showHour();
				LcdWriteCmd(0x80 + 0x40 + 5);
				break;
			case 4:
				day++;
				if (day == 32)
					day = 1;
				showDay();
				LcdWriteCmd(0x80 + 12);
				break;
			case 5:
				month++;
				if (month == 13)
					month = 1;
				showMonth();
				LcdWriteCmd(0x80 + 9);
				break;
			case 6:
				year++;
				showYear();
				LcdWriteCmd(0x80 + 6);
				break;
			}
		}
		if (K2 == 0)
		{
			delay_ms(10);
			while (!K2)
				;
			switch (setNum)
			{
			case 1:
				second--;
				if (second < 0)
					second = 59;
				showSecond();
				LcdWriteCmd(0x80 + 0x40 + 11);
				break;
			case 2:
				minute--;
				if (minute < 0)
					minute = 59;
				showMinute();
				LcdWriteCmd(0x80 + 0x40 + 8);
				break;
			case 3:
				hour--;
				if (hour < 0)
					hour = 23;
				showHour();
				LcdWriteCmd(0x80 + 0x40 + 5);
				break;
			case 4:
				day--;
				showDay();
				LcdWriteCmd(0x80 + 12);
				break;
			case 5:
				month--;
				showMonth();
				LcdWriteCmd(0x80 + 9);
				break;
			case 6:
				year--;
				showYear();
				LcdWriteCmd(0x80 + 6);
				break;
			}
		}
	}
}

/******检测声音响多少次*********/
uchar soundRead()
{
	uchar soundStateTemp = 0;
	if (sound == 0)
	{
		delay_ms(1);
		if (sound == 0)
		{
			while (sound == 0)
				;
			waitTime = 0;
			soundState++;
			LcdWrite(0x80 + 15, '0' + soundState);
		}
		while (!sound)
			;
		delay_ms(1);
		while (!sound)
			;
	}
	if (waitTime >= 100)
	{
		waitTime = 0;
		soundStateTemp = soundState;
		soundState = 0;
	}
	return soundStateTemp;
}

/*************主函数****************/
void main()
{
	uchar soundCount;
	InitDS1302();
	InitLcd1602();
	readTime();
	showTime();
	sound = 1;
	backlight = 0; //打开1602背光

	TMOD = 0x11;
	EA = 1;

	TH0 = 0x4C;
	TL0 = 0x00;
	ET0 = 1;
	TR0 = 1;

	TH1 = (65536 - 9216) / 256;
	TL1 = (65536 - 9216) % 256;
	ET1 = 1;
	TR1 = 1;

	while (1)
	{
		
		setTime();
		readTime();
		/*按下K3，切换到显示温湿度*/
		if (setFlag == 0)
		{
			if (K3 == 0)
			{
				delay_ms(10);
				if (K3 == 0)
				{
					backlight = 0;
					TH0 = 0x4C;
					TL0 = 0x00;
					TR0 = 1;
					timer0Count = 0;
					displayFlag = ~displayFlag;
					LcdWriteCmd(0x01);
				}
				while (!K3)
					;
				delay_ms(10);
				while (!K3)
					;
			}

			if (K2 == 0)
			{
				delay_ms(10);
				if (K2 == 0)
				{
					backlight = 0;
					TH0 = 0x4C;
					TL0 = 0x00;
					TR0 = 1;
					timer0Count = 0;

					// readHT();
					// NPlay(23); // 现在温度是：
					// NPlayTemp(T);
					// NPlay(24); // 现在湿度是：
					// NPlayHumi(H);
				}
				while (!K2)
					;
				delay_ms(10);
				while (!K2)
					;
			}

			soundCount = soundRead();
			if (soundCount == 2)
			{
				LcdWrite(0x80, '0' + soundCount);
				backlight = 0;
				TH0 = 0x4C;
				TL0 = 0x00;
				TR0 = 1;
				timer0Count = 0;

				NPlay(22); //现在时刻北京时间
				NPlayTimeHour(hour);
				NPlayTimeMinute(minute);
			}
			 if (soundCount == 3)
			{
				LcdWrite(0x80, '0' + soundCount);
				backlight = 0;
				TH0 = 0x4C;
				TL0 = 0x00;
				TR0 = 1;
				timer0Count = 0;

				NPlay(23); // 现在温度是：
				NPlayTemp(T);
				NPlay(24); // 现在湿度是：
				NPlayHumi(H);
			}

			/*根据标记flag判断，双数显示时间，单数显示温湿度*/
			if (displayFlag == 0)
				showTime();
			else
			{
				readHT();
				showHT();
			}
		}
	}
}

void Timer0() interrupt 1
{
	TH0 = 0x4C;
	TL0 = 0x00;
	timer0Count++;
	if (timer0Count == 400)
	{
		LcdWrite(0x80, ' ');
		LcdWrite(0x80 + 15, ' ');
		backlight = 1;
		timer0Count = 0;
	}
}

void Timer1() interrupt 3
{
	TH1 = (65536 - 9216) / 256;
	TL1 = (65536 - 9216) % 256;
	waitTime++;
}