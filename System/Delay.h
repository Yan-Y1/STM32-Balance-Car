#ifndef __DELAY_H 
#define __DELAY_H

//引用stm32头文件
#include "stm32f10x.h"                  // Device header

//声明微妙级别的函数
void delay_by_us(uint32_t us);

//声明毫秒级别延迟函数
void delay_by_ms(uint32_t ms);

//声明秒级别延迟函数
void delay_by_s(uint32_t s);


#endif

