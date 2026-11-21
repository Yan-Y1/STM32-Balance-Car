#ifndef __ENCONDER_H 
#define __ENCONDER_H

#include "stm32f10x.h"                  // Device header

//采用定时器3来编码器测试右电机
void Timer3_Enconder_Init(void);

//采样定时器4来编码器测速，测试左电机
void Timer4_Enconder_Init(void);

int16_t GetCounter(void);

#endif
