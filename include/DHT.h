#ifndef DHT_H
#define DHT_H

#include <reg52.h>
#include <intrins.h>
#include "config.h"

sbit Data = P1 ^ 0;

void DHTStart();
uchar DHTByteRead();

#endif