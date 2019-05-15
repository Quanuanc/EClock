#include "config.h"

sbit Data = P1 ^ 0;

void DHT11_receive();
void DHT11_delay_ms(uint z);
void DHT11_delay_us(uchar n);
void delay(uint n);
void DHT11_start();
uchar DHT11_rec_byte();