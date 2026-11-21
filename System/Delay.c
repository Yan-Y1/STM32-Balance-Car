#include "Delay.h"

void delay_by_us(uint32_t us){
	/*
		72MHZ--- 1s ; 72M/1000000-------1us
	*/
	//设置重装载寄存器 LOAD
	SysTick->LOAD=SystemCoreClock/1000000 *us -1;
	//设置VAL 的初始值是0，后续VAL会自动加载LOAD 寄存器的值，然后向下递减
	SysTick->VAL=0;
	/*
	设置CTRL 控制寄存器的值：
		COUNTFLAG=0;
		CLKSOURCE=1, 表示选用AHB总线的时钟，72MHZ
		ENABLE=1,表示使能定时器
		ENABLE=0,表示关闭定时器
	*/
	SysTick->CTRL =0x0000005;
	//VAL 寄存器加载LOAD 寄存器的值之后，VAL会向下递减，直到VAL 为0
	//那么硬件自动将COUNTFLAG设置为1
	while(!(SysTick->CTRL & 0x00010000));
	//关闭定时器的计数器，完全关闭
	SysTick->CTRL=0;
}

//声明毫秒级别延迟函数
void delay_by_ms(uint32_t ms){
	while(ms--){
	delay_by_us(1000);
	}
}


//声明秒级别延迟函数
void delay_by_s(uint32_t s){
	while(s--){
	delay_by_ms(1000);
	}
}



