#include "Encoder.h"                  // Device header

//采用编码器测速
/*
	A相-->PA6,TIM3_CH-->TI1FP1-->编码器
	B相-->PA7,TIM3_CH-->TI1FP1-->编码器

*/


void Timer3_Enconder_Init(void){
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//配置输入捕获
	TIM_ICInitTypeDef TIM_ICInitStruct;
	//TIM_ICStructInt---可以输入捕获的结构体变量中的成员变量全部初始化
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;  //指定通道1
	TIM_ICInitStruct.TIM_ICFilter=0xF;			//指定过滤器，数字滤波
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;  //指定通道2
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	//指定编码器接口配置
	//参数一：表示指定使用哪个定时器来测速
	//参数二：表示计数方式，TIM_EncoderMode_TI12 在TI1 和TI2 上都计数
	//参数三：表示IC1滤波后极性不反转
	//参数四：表示IC2滤波后极性补反转
	TIM_EncoderInterfaceConfig(TIM3,
							   TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising
							   );
	
	
		
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65536-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//开启定时器
	TIM_Cmd(TIM3,ENABLE);
}


//采用编码器测速
/*
	A相-->PB6,TIM4_CH
	B相-->PB7,TIM4_CH

*/

//采样定时器4来编码器测速，测试左电机
void Timer4_Enconder_Init(void){
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//配置输入捕获
	TIM_ICInitTypeDef TIM_ICInitStruct;
	//TIM_ICStructInt---可以输入捕获的结构体变量中的成员变量全部初始化
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;  //指定通道1
	TIM_ICInitStruct.TIM_ICFilter=0xF;			//指定过滤器，数字滤波
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;  //指定通道2
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	
	//指定编码器接口配置
	//参数一：表示指定使用哪个定时器来测速
	//参数二：表示计数方式，TIM_EncoderMode_TI12 在TI1 和TI2 上都计数
	//参数三：表示IC1滤波后极性不反转
	//参数四：表示IC2滤波后极性补反转
	TIM_EncoderInterfaceConfig(TIM4,
							   TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising
							   );
	
	
		
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65536-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	//开启定时器
	TIM_Cmd(TIM4,ENABLE);
}



















int16_t GetCounter(void){
	int16_t Temp=TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}



















