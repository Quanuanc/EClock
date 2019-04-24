#include <reg52.h>

sbit Data=P2^7;   //定义DHT11数据线

extern void LcdInit();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main(){
    unsigned char test[]="Hello World";
    LcdInit();
    LcdShowStr(0,0,test);
}
