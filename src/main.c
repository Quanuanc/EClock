#include "include/config.h"
#include "include/1602.h"
#include "include/1302.h"
#include "include/DHT11.h"

void showTime();
void showHT();

sbit K1 = P1 ^ 4;

void main()
{
	uint flag = 0;
	InitDS1302(); //初始化实时时钟
	InitLcd1602();

	while (1)
	{
		if (flag % 2 == 0)
		{
			showTime();
			if (K1 == 0)
			{
				flag++;
			}
		}
		else
		{
			showHT();
			if (K1 == 0)
			{
				flag++;
			}
		}
	}
}

void ShowTime()
{
	uchar i;
	uchar psec = 0xAA; //秒备份，初值 AA 确保首次读取时间后会刷新显示
	uchar time[8];	 //当前时间数组
	uchar str[12];	 //字符串转换缓冲区

	for (i = 0; i < 7; i++)
	{ //读取 DS1302 当前时间
		time[i] = DS1302SingleRead(i);
	}
	if (psec != time[0])
	{				  //检测到时间有变化时刷新显示
		str[0] = '2'; //添加年份的高 2 位：20
		str[1] = '0';
		str[2] = (time[6] >> 4) + '0';   //“年”高位数字转换为 ASCII 码
		str[3] = (time[6] & 0x0F) + '0'; //“年”低位数字转换为 ASCII 码
		str[4] = '-';					 //添加日期分隔符
		str[5] = (time[4] >> 4) + '0';   //“月”
		str[6] = (time[4] & 0x0F) + '0';
		str[7] = '-';
		str[8] = (time[3] >> 4) + '0'; //“日”
		str[9] = (time[3] & 0x0F) + '0';
		str[10] = '\0';
		LcdShowStr(3, 0, str); //显示到液晶的第一行

		str[0] = (time[2] >> 4) + '0'; //“时”
		str[1] = (time[2] & 0x0F) + '0';
		str[2] = ':';				   //添加时间分隔符
		str[3] = (time[1] >> 4) + '0'; //“分”
		str[4] = (time[1] & 0x0F) + '0';
		str[5] = ':';
		str[6] = (time[0] >> 4) + '0'; //“秒”
		str[7] = (time[0] & 0x0F) + '0';
		str[8] = '\0';
		LcdShowStr(4, 1, str); //显示到液晶的第二行
		psec = time[0];		   //用当前值更新上次秒数
	}
}

void showHT()
{
	uchar rec_dat[13];
	uchar title[] = "H      T";
	uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
	LcdShowStr(4, 0, title);
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
		rec_dat[0] = '0' + (RH / 10);
		rec_dat[1] = '0' + (RH % 10);
		rec_dat[2] = '%';
		rec_dat[3] = ' ';
		rec_dat[4] = ' ';
		rec_dat[5] = ' ';
		rec_dat[6] = ' ';
		rec_dat[7] = '0' + (TH / 10);
		rec_dat[8] = '0' + (TH % 10);
		rec_dat[9] = 'C';
		rec_dat[10] = ' ';
		rec_dat[11] = ' ';
		rec_dat[12] = ' ';
	}

	LcdShowStr(3, 1, rec_dat);
}
