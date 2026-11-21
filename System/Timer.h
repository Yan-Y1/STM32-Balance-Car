#ifndef __Timer_H 
#define __Timer_H

#include "stm32f10x.h"                  // Device header

extern int16_t leftSpeed;
extern int16_t rightSpeed;
//初始化定时器
void Timer2_Init(void);


#endif
