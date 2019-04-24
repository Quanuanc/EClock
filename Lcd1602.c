#include <reg52.h>

#define LCD1602_DB P1

sbit LCD1602_RS = P2 ^ 0;
sbit LCD1602_RW = P2 ^ 1;
sbit LCD1602_E = P2 ^ 2;

void LcdWaitReady()
{
    unsigned char sta;
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
        //bit7 等于 1 表示液晶正忙，重复检测直到其等于 0 为止
    } while (sta & 0x80);
}
void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    if (y == 0)
    {                    //由输入的屏幕坐标计算显示 RAM 的地址
        addr = 0x00 + x; //第一行字符地址从 0x00 起始
    }
    else
    {
        addr = 0x40 + x; //第二行字符地址从 0x40 起始
    }
    LcdWriteCmd(addr | 0x80); //设置 RAM 地址
}
void LcdShowStr(unsigned char x, unsigned char y,
                unsigned char *str)
{
    LcdSetCursor(x, y); //设置起始地址
    while (*str != '\0')
    {
        LcdWriteDat(*str++);
    }
}
void LcdInit()
{
    LcdWriteCmd(0x38); //16*2 显示，5*7 点阵，8 位数据接口
    LcdWriteCmd(0x0C); //显示器开，光标关闭
    LcdWriteCmd(0x06); //文字不动，地址自动+1
    LcdWriteCmd(0x01); //清屏
}