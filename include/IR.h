#ifndef IR_H_
#define IR_H_

#include <reg52.h>
#include <intrins.h>
#include "config.h"

sbit ir = P3 ^ 3; //红外接口标志

extern void Ir_work(void);
extern void IRTimeToData(void);

#endif