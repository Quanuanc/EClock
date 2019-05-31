#include "include/config.h"
#include "include/1602.h"
#include "include/1302.h"
#include "include/DHT.h"
#include "include/key.h"
#include "include/ny3p.h"
#include "include/IR.h"

uint backlightTime = 0;											//背光灯等待关闭时间
uchar showHTTime = 0;											//每2s测量一次温湿度
sbit sound = P2 ^ 0;											//声音传感器，0-检测到声音，1-未检测到
uchar second, minute, hour, week, day, month, year, setNum = 0; //时间变量
bit displayFlag = 0, setFlag = 0;								//切换显示标志，设置时间标志
uchar RH, RL, TH, TL, revise, H, T, H_L, T_L;					//温湿度处理过程中的变量
uint soundWaitTime = 0;											//检测声音等待时间
uchar soundState = 0;											//声音次数
uchar soundNum = 0;												//声音次数临时变量
// uchar testSound = 0;											//测试声音传感器

uchar irTime;
bit irOK, irProcess;
uchar irRecTime[33];
uchar irData[4];
uchar IRKey = 0;

/*************红外码值处理***************/
void IRTimeToData(void)
{
	uchar i, j, k;
	uchar cord, value;

	k = 1;
	for (i = 0; i < 4; i++) //处理4个字节
	{
		for (j = 1; j <= 8; j++) //处理1个字节8位
		{
			cord = irRecTime[k];
			if (cord > 7) //大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
				value |= 0x80;
			if (j < 8)
			{
				value >>= 1;
			}
			k++;
		}
		irData[i] = value;
		value = 0;
	}
	irProcess = 1; //处理完毕标志位置1
}

void IRDataToKey(void) //红外键值散转程序
{
	switch (irData[2]) //判断第三个数码值
	{
	case 0x0c:
		IRKey = 1;
		break;
	case 0x18:
		IRKey = 2;
		break;
	case 0x5e:
		IRKey = 3;
		break;
	default:
		break;
	}

	irProcess = 0; //处理完成标志
}

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
void readHT()
{
	DHTStart();
	if (Data == 0)
	{
		while (Data == 0)
			; //等待拉高
		delay_us(39);					   //拉高后延时80us
		//Delay80us();
		RH = DHTByteRead();				   //接收湿度高八位
		RL = DHTByteRead();				   //接收湿度低八位
		TH = DHTByteRead();				   //接收温度高八位
		TL = DHTByteRead();				   //接收温度低八位
		revise = DHTByteRead();			   //接收校正位
		delay_us(25);					   //结束
		if ((RH + RL + TH + TL) == revise) //校正
		{
			H = (RH * 256 + RL) / 10;   //DHT21湿度数据格式为16bit，并且是实际湿度的10倍
			H_L = (RH * 256 + RL) % 10; //湿度小数点部分
			T = (TH * 256 + TL) / 10;
			T_L = (TH * 256 + TL) % 10; //温度小数点部分
		}
	}
}
/*******显示温湿度函数******/
void showHT()
{
	LcdWrite(0x80 + 3, ' ');
	LcdWrite(0x80 + 4, 'H');
	LcdWrite(0x80 + 5, ':');
	LcdWrite(0x80 + 6, ' ');
	LcdWrite(0x80 + 7, '0' + (H / 10));
	LcdWrite(0x80 + 8, '0' + (H % 10));
	LcdWrite(0x80 + 9, '.');
	LcdWrite(0x80 + 10, '0' + H_L);
	LcdWrite(0x80 + 11, '%');
	LcdWrite(0x80 + 12, ' ');

	LcdWrite(0x80 + 0x40 + 4, 'T');
	LcdWrite(0x80 + 0x40 + 5, ':');
	LcdWrite(0x80 + 0x40 + 6, ' ');
	LcdWrite(0x80 + 0x40 + 7, '0' + (T / 10));
	LcdWrite(0x80 + 0x40 + 8, '0' + (T % 10));
	LcdWrite(0x80 + 0x40 + 9, '.');
	LcdWrite(0x80 + 0x40 + 10, '0' + T_L);
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
			configTimer0();
			backlightTime = 0;
			setNum++;

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

/*********检测声音函数***********/
uchar soundRead()
{
	uchar soundCountTemp = 0; //检测到声音的次数
	if (sound == 0)
	{
		delay_us(10);
		if (sound == 0)
		{
			while (sound == 0)
				;
			// testSound++;
			soundWaitTime = 0;
			soundNum++;
		}
	}

	if (soundWaitTime >= 3600) //检测声音时间，超过1s无声音后，返回检测到的次数
	{
		soundWaitTime = 0;
		soundCountTemp = soundNum;
		soundNum = 0;
	}
	return soundCountTemp;
}

/******************************************主函数************************************************/
void main()
{
	InitDS1302();
	InitLcd1602();
	showTime();
	backlight = 0;

	TMOD = 0x21;
	EA = 1;  //总中断
	ET0 = 1; //定时器0中断
	ET1 = 1; //定时器1中断

	IT1 = 1; //外部中断1下降沿触发
	EX1 = 1; //外部中断1使能
	configTimer0();
	configTimer1();

	while (1)
	{
		setTime();
		if (irOK)
		{
			IRTimeToData();
			irOK = 0;
		}
		if (irProcess)
		{
			IRDataToKey();
		}

		/****************不在设置时间界面时***************/
		if (setFlag == 0)
		{
			/***********按下遥控器3键，切换显示****************/
			if (IRKey == 3)
			{
				IRKey = 0;
				backlight = 0;
				configTimer0();
				backlightTime = 0;

				//LcdWriteCmd(0x01);
				displayFlag = ~displayFlag;
			}
			/****************按下K3，切换显示******************/
			if (K3 == 0)
			{
				delay_ms(10);
				if (K3 == 0)
				{
					backlight = 0;
					configTimer0();
					backlightTime = 0;
					displayFlag = ~displayFlag;
					//LcdWriteCmd(0x01);
				}
				while (!K3)
					;
				delay_ms(10);
				while (!K3)
					;
			}
			/****************响两声，唤醒并播报时间**********/
			soundState = soundRead();
			// LcdWrite(0x80, ' ' + testSound);
			if (soundState >= 2 && soundState <= 4)
			{
				// LcdWrite(0x80 + 15, 'T');
				backlight = 0;
				configTimer0();
				backlightTime = 0;
				NPlay(22);				 // 现在时刻北京时间：
				NPlayTimeHour(hour);	 //播报时
				NPlayTimeMinute(minute); //播报分
			}

			/*****************根据标记flag判断，0显示时间，1显示温湿度*************/
			if (displayFlag == 0)
			{
				showTime();
				/*******************按下K2，播报时间**********************/
				if (K2 == 0)
				{
					delay_ms(10);
					if (K2 == 0)
					{
						backlight = 0;
						configTimer0();
						backlightTime = 0;
						NPlay(22);				 // 现在时刻北京时间：
						NPlayTimeHour(hour);	 //播报时
						NPlayTimeMinute(minute); //播报分
					}
					while (!K2)
						;
					delay_ms(10);
					while (!K2)
						;
				}
				if (IRKey == 2)
				{
					IRKey = 0;
					backlight = 0;
					configTimer0();
					backlightTime = 0;
					//LcdWrite(0x80, 'T');
					NPlay(22);				 // 现在时刻北京时间：
					NPlayTimeHour(hour);	 //播报时
					NPlayTimeMinute(minute); //播报分
				}
			}
			else if (displayFlag == 1)
			{
				showHT();
				if (showHTTime == 35)
					readHT();
				/****************按下K2，播报温湿度***********************/
				if (K2 == 0)
				{
					delay_ms(10);
					if (K2 == 0)
					{
						backlight = 0;
						configTimer0();
						backlightTime = 0;
						NPlay(23); //现在温度是：
						NPlayTemp(T, T_L);
						NPlay(24); //现在湿度是：
						NPlayHumi(H, H_L);
					}
					while (!K2)
						;
					delay_ms(10);
					while (!K2)
						;
				}
				if (IRKey == 2)
				{
					IRKey = 0;
					backlight = 0;
					configTimer0();
					backlightTime = 0;
					//LcdWrite(0x80, 'H');
					NPlay(23); //现在温度是：
					NPlayTemp(T, T_L);
					NPlay(24); //现在湿度是：
					NPlayHumi(H, H_L);
				}
			}
		}
	}
}

void Timer0() interrupt 1
{
	TH0 = 0x4C;
	TL0 = 0x00;
	backlightTime++; //背光灯等待关闭时间
	showHTTime++;	//温湿度读取刷新间隔

	if (showHTTime > 35)
	{
		showHTTime = 0;
	}
	/**************显示流程**********/
	if (backlightTime == 200)
	{
		displayFlag = ~displayFlag;
	}
	if (backlightTime == 400) //backlightTime 为100时，背光灯时间5s
	{
		// testSound = 0;
		// LcdWrite(0x80 + 15, ' ');
		// LcdWrite(0x80, ' ');
		displayFlag = 0;
		backlight = 1;
		backlightTime = 0;
		TR0 = 0;
	}
}

void Timer1() interrupt 3
{
	irTime++;
	soundWaitTime++; //声音检测等待时间
}

/************ 外部中断处理***********/
void EX1IR(void) interrupt 2 //外部中断0服务函数
{
	static uchar i;		  //接收红外信号处理
	static bit startflag; //是否开始处理标志位

	if (startflag)
	{
		if (irTime < 63 && irTime >= 33) //引导码，9ms+4.5ms
			i = 0;
		irRecTime[i] = irTime; //存储每个电平的持续时间，用于以后判断是0还是1
		irTime = 0;
		i++;
		if (i == 33)
		{
			irOK = 1;
			i = 0;
		}
	}
	else
	{
		irTime = 0;
		startflag = 1;
	}
}