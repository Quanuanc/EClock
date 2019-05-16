#include "include/1302.h"

/* 发送一个字节到 DS1302 通信总线上 */
void DS1302ByteWrite(uchar dat)
{
    uchar mask;

    for (mask = 0x01; mask != 0; mask <<= 1)
    { //低位在前，逐位移出
        if ((mask & dat) != 0)
        { //首先输出该位数据
            DS1302_IO = 1;
        }
        else
        {
            DS1302_IO = 0;
        }
        DS1302_SCLK = 1; //然后拉高时钟
        DS1302_SCLK = 0; //再拉低时钟，完成一个位的操作
    }
    DS1302_IO = 1; //最后确保释放 IO 引脚
}
/* 由 DS1302 通信总线上读取一个字节 */
uchar DS1302ByteRead()
{
    uchar mask;
    uchar dat = 0;

    for (mask = 0x01; mask != 0; mask <<= 1)
    { //低位在前，逐位读取
        if (DS1302_IO != 0)
        { //首先读取此时的 IO 引脚，并设置 dat 中的对应位
            dat |= mask;
        }
        DS1302_SCLK = 1; //然后拉高时钟
        DS1302_SCLK = 0; //再拉低时钟，完成一个位的操作
    }
    return dat; //最后返回读到的字节数据
}
/* 用单次写操作向某一寄存器写入一个字节，reg-寄存器地址，dat-待写入字节 */
void DS1302SingleWrite(uchar reg, uchar dat)
{
    DS1302_RST = 1;                     //使能片选信号
    DS1302ByteWrite((reg << 1) | 0x80); //发送写寄存器指令
    DS1302ByteWrite(dat);               //写入字节数据
    DS1302_RST = 0;                     //除能片选信号
}
/* 用单次读操作从某一寄存器读取一个字节，reg-寄存器地址，返回值-读到的字节 */
uchar DS1302SingleRead(uchar reg)
{
    uchar dat;
    DS1302_RST = 1;                     //使能片选信号
    DS1302ByteWrite((reg << 1) | 0x81); //发送读寄存器指令
    dat = DS1302ByteRead();             //读取字节数据
    DS1302_RST = 0;                     //除能片选信号
    return dat;
}
/* DS1302 初始化，如发生掉电则重新设置初始时间 */
void InitDS1302()
{
    uchar i;
    uchar code InitTime[] = {0x00, 0x30, 0x12, 0x08, 0x10, 0x02, 0x13};//2013 年 10 月 8 日 星期二 12:30:00,顺序:秒，分，时...

    DS1302_RST = 0; //初始化 DS1302 通信引脚
    DS1302_SCLK = 0;
    i = DS1302SingleRead(0); //读取秒寄存器

    if ((i & 0x80) != 0)
    {                               //由秒寄存器最高位 CH 的值判断 DS1302 是否已停止
        DS1302SingleWrite(7, 0x00); //撤销写保护以允许写入数据
        for (i = 0; i < 7; i++)
        { //设置 DS1302 为默认的初始时间
            DS1302SingleWrite(i, InitTime[i]);
        }
    }
}