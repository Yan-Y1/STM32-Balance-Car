#include "sound.h"

//初始化蜂鸣器
void Sound_Init(void){
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	//初始化
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//蜂鸣器响
void Sound_Open(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	


}

//蜂鸣器不响
void Sound_Close(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_6);


}
