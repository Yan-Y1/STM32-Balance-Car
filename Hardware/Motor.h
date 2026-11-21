#ifndef __MOTOR_H 
#define __MOTOR_H

#include "stm32f10x.h"                  // Device header

#include "PWM.h"

void Motor_Init(void);
	
void leftSetSpeed(int32_t speed);

void rightSetSpeed(int32_t speed);


#endif


