#ifndef __USART_MODEL_H 
#define __USART_MODEL_H

#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include "Delay.h"
#include "PID.h"

#define BUFFER_SIZE 200
//定义字符数组
extern char message[200];

//定义状态表示
extern uint8_t flag;

//初始化DMA 的函数，使用DMA 来转运USART 的数据寄存器中的数据
void USART2_DMA_Init(void);

//初始化USART
void USART_Model_Init(void);

//发送字节
void Send_Byte(uint8_t data,uint32_t timeOut);

//发送数组
void Send_Array(uint8_t *arr,uint16_t length);



//发送字符串
void Send_String(char *str);

#endif
