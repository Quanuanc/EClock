#include "include/1602.h"

/* 等待液晶准备好 */
void LcdWaitReady()
{
    uchar sta;
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7 等于 1 表示液晶正忙，重复检测直到其等于 0 为止
}
/* 向 LCD1602 液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(uchar cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
/* 向 LCD1602 液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(uchar dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}

/* 初始化 1602 液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38); //16*2 显示，5*7 点阵，8 位数据接口
    LcdWriteCmd(0x0C); //显示器开，光标关闭
    LcdWriteCmd(0x06); //文字不动，地址自动+1
    LcdWriteCmd(0x01); //清屏
}
/* 向1602RAM写入一个符号 
 * 入参：addr:地址；dat：写入的内容
 */
void LcdWrite(uchar addr, uchar dat)
{
    LcdWriteCmd(addr);
    LcdWriteDat(dat);
}