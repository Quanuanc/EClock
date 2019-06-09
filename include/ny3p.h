#ifndef NY3P_H_
#define NY3P_H_

#include <reg52.h>
#include <intrins.h>
#include "config.h"

sbit N_busy = P2^2;
sbit N_data = P2^3;
sbit N_rst = P2^4;

void NPlay(uchar n);
void NPlayTimeHour(uchar hour);
void NPlayTimeMinute(uchar minute);
void NPlayTemp(uchar T, uchar T_L);
void NPlayHumi(uchar H, uchar H_L);

#endif