#include "Motor.h"

void Motor_Init(void){
	PWM_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	//左电机方向控制引脚 PA4, PA5
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//右电机方向控制引脚 PA0, PA1
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//==========左边电机转动===================
//IN1-PA4, IN2-PA5


void left_back(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_5);		//IN1-H
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//IN2-L
}


void left_forward(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);	//IN1-L
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		//IN2-H
}

void left_stop(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//IN1-L
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);		//IN2-L
}

//根据指定的速度来运行电机，1-20%占空比前进,-1-以20% 占空比后退
void leftSetSpeed(int32_t speed){
		if(speed>0){
		left_forward();
			TIM_SetCompare1(TIM1,speed);
		}
		else if(speed<0){
		left_back();
			TIM_SetCompare1(TIM1,-speed);
		
		}
		else{
		left_stop();
		TIM_SetCompare1(TIM1,0);
		
		}
}



//=====================右边电机转动===========
//IN1-PA0,IN2-PA1

void right_back(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//IN1-H
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//IN2-L
}

void right_forward(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

void right_stop(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

void rightSetSpeed(int32_t speed){
	if(speed>0){
		right_forward();
		TIM_SetCompare2(TIM1,speed);
	}
	else if(speed<0){
		right_back();
		TIM_SetCompare2(TIM1,-speed);
	}
	else{
		right_stop();
		TIM_SetCompare2(TIM1,0);
	}
}







