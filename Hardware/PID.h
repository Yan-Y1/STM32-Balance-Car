#ifndef __PID_H 
#define __PID_H

#include "stm32f10x.h"                  // Device header
//定义机械中值
extern float MachineBalance;

//平均分了10等分，前后5等分，ARR=7200,每一个等分720
//影响系统的响应速度，因为俯仰角先前是负数，使用KP 需要添加负号
extern int16_t BalanceKP;

//控制速度变化的速度
extern float BalanceKD ;
//根据俯仰角计算前后移动的速度,y轴的角速度
int32_t PID_Balance(int16_t pitch,int16_t gyro_y);

#endif
