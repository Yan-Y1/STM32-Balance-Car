#include "PWM.h"                  // Device header

//PWM初始化
void PWM_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	


	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=7200-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	//高级定时器要加这一句
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=3600;//CCR,捕获比较寄存器 ，duty=50
	//OC1输出比较
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_Pulse=3600;//CCR,捕获比较寄存器 ，duty=50
	//OC2 输出比较
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
	TIM_Cmd(TIM1,ENABLE);

}


