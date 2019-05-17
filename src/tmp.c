	if (K1 == 0)
	{
		delay_ms(10);
		if (K1 == 0)
		{
			setNum++;

			switch (setNum)
			{
			case 1:
				setFlag = 1;
				DS1302SingleWrite(7, 0x00); //去除写保护
				DS1302SingleWrite(0, 0x80); //时钟停下
				LcdWrite(0 + 0x40, setNum);
				LcdWriteCmd(0 + 0x40 + 9);
				LcdWriteCmd(0x0f);
				break;
			case 2:
				LcdWriteCmd(0 + 0x40 + 6);
				break;
			case 3:
				LcdWriteCmd(0 + 0x40 + 3);
				break;
			case 4:
				LcdWriteCmd(0 + 9);
				break;
			case 5:
				LcdWriteCmd(0 + 6);
				break;
			case 6:
				LcdWriteCmd(0 + 3);
				break;
			
			case 7:
				setNum = 0;
				setFlag = 0;

				LcdWriteCmd(0x0c);								//不显示光标
				DS1302SingleWrite(0, 0x00);								//时钟继续
				DS1302SingleWrite(0, (second / 10) << 4 | second % 10); //将调节后的秒写入DS1302
				DS1302SingleWrite(1, (minute / 10) << 4 | minute % 10); //将调节后的分写入DS1302
				DS1302SingleWrite(2, (hour / 10) << 4 | hour % 10);		//将调节后的时写入DS1302
				DS1302SingleWrite(5, (week / 10) << 4 | week % 10);		//将调节后的星期写入DS1302
				DS1302SingleWrite(3, (day / 10) << 4 | day % 10);		//将调节后的日写入DS1302
				DS1302SingleWrite(4, (month / 10) << 4 | month % 10);   //将调节后的月写入DS1302
				DS1302SingleWrite(6, (year / 10) << 4 | year % 10);		//
																	//DS1302SingleWrite(0,0x00);//时钟继续走这一句不能加在这，否则每次调 完时后秒会归O
				DS1302SingleWrite(7, 0x80);								//写保护关
																	//LcdWriteCmd(0x0c);
				break;
			}
		}
		while (!set)
			;
		delay_ms(10);
		while (!set)
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
				//show_1302(7,0x00);//去除写保护
				showSecond();
				LcdWriteCmd(0 + 0x40 + 9);
				break;
			case 2:
				minute++;
				if (minute == 60)
					minute = 0;
				showMinute();
				LcdWriteCmd(0 + 0x40 + 6);
				break;
			case 3:
				hour++;
				if (hour == 24)
					hour = 0;
				showHour();
				LcdWriteCmd(0 + 0x40 + 3);
				break;
			case 4:
				day++;
				showDay();
				LcdWriteCmd(0 + 9);
				break;
			case 5:
				month++;
				showMonth();
				LcdWriteCmd(0 + 6);
				break;
			case 6:
				year++;
				showYear();
				LcdWriteCmd(0 + 3);
				break;
			
				// default:break;
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
				LcdWriteCmd(0 + 0x40 + 9);
				break;
			case 2:
				minute--;
				if (minute < 0)
					minute = 59;
				showMinute();
				LcdWriteCmd(0 + 0x40 + 6);
				break;
			case 3:
				hour--;
				if (hour < 0)
					hour = 23;
				showHour();
				LcdWriteCmd(0 + 0x40 + 3);
				break;
			case 4:
				day--;
				showDay();
				LcdWriteCmd(0 + 9);
				break;
			case 5:
				month--;
				showMonth();
				LcdWriteCmd(0 + 6);
				break;
			case 6:
				year--;
				showYear();
				LcdWriteCmd(0 + 3);
				break;
			
				//  default:break;
			}
		}
	}