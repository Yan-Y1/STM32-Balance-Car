#include "Timer.h"
#include "PID.h"
#include "IMU.h"
#include "Motor.h"

//初始化定时器
void Timer2_Init(void){
	//1.开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//2.配置时钟来源，系统内部时钟，72MHZ
	TIM_InternalClockConfig(TIM2);
	
	//3.配置定时器2的时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//系统时钟到达时基单元后是72MHZ,预先不做分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数，也可以向下计数
	//现在要产生1HZ<==>1s一次中断，freq=72M/(PSC+1)/(ARR+1)=1HZ
	//改为10ms(100Hz)采样：72M/72/10000=100Hz，与老师测得的17、18一致
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//ARR,自动重装载寄存器
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;//PSC,预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0x00;//高级定时器使用
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//防御性编程，它确保定时器初始化完成后不会立即触发一次虚假的更新中断
	/*
		作用是清除定时器的更新事件标志位（UIF-Update Interrupt Flag）
		这个标志位在以下情况会被硬件置位
		计数器溢出（从ARR回到0）
		或者初始化时，自动重载寄存器（ARR）被影子装载后
		注意：TIM_ClearFlag 只清除状态标志位（SR）,不影响中断使能
		而TIM_ClearITPendingBit() 是专门清除中断pending位的（在NVIC层面）
	*/
	
	
	//清除标志位
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	
	//配置定时器中断
	//TIM_IT_Updata: 表示更新中断，将来走中断函数
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	//启动定时器2，使能定时器2
	TIM_Cmd(TIM2,ENABLE);
}

int16_t leftSpeed;
int16_t rightSpeed;

//指定TIM2的中断函数
void TIM2_IRQHandler(void){
	//判断中断的状态
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){

		//1. 采样编码器（10ms 周期得到速度）
		leftSpeed=TIM_GetCounter(TIM4);
		TIM_SetCounter(TIM4,0);

		rightSpeed=TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM3,0);

		//2. 更新姿态解算（IMU 内部 dt=5ms，这里 10ms 调一次）
		IMU_getEuleranAngles();

		//3. 平衡 PID，限幅到 2000（约 28% 占空比）防跑飞满速
		int32_t speed = PID_Balance(imu_Angle.Pitch, mpu6050.gyro_y);
		if (speed >= 7199) speed = 7199;
		else if (speed <= -7199) speed = -7199;

		//4. 输出到两个电机（当前只有直立环，两轮同速）
		leftSetSpeed(speed);
		rightSetSpeed(speed);

		//清除更新中断的标志位，清除的是Pending位，这个位在NVIC中，也就是系统中
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}






























