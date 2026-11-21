//包含头文件
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "sound.h"
#include "MPU6050.h"
#include "IMU.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "PID.h"
#include "USART_Model.h"

/*
//===================测试定时器2 读取轮子的转速==========
int main(){
	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	Timer3_Enconder_Init();
	Timer4_Enconder_Init();
	Timer2_Init();
	
	OLED_ShowString(1,1,"L:");
	OLED_ShowString(2,1,"R:");
	
	leftSetSpeed(3600);
	rightSetSpeed(3600);
	
	while(1){
		OLED_ShowSignedNum(1,3,leftSpeed,5);
		OLED_ShowSignedNum(2,3,rightSpeed,5);
	}
}



*/
/*
//==============手动测量小车平衡时的俯仰角=================
int main(){

	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	//采用定时器3来编码器测试右电机
	Timer3_Enconder_Init();
	//采样定时器4来编码器测速，测试左电机
	Timer4_Enconder_Init();
	Timer2_Init();
	
	OLED_ShowString(1,1,"L:       R:      ");
	OLED_ShowString(2,1,"Pitch:");
	
	leftSetSpeed(0);
	rightSetSpeed(0);
	
	
	while(1){
		OLED_ShowSignedNum(1,3,leftSpeed,5);
		OLED_ShowSignedNum(1,11,rightSpeed,5);
		//计数欧拉角
		IMU_getEuleranAngles();
		OLED_ShowSignedNum(2,7,imu_Angle.Pitch,5);//显示俯仰角
	}
}







*/



/*==========测PID============
int main(){

	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	//采用定时器3来编码器测试右电机
	Timer3_Enconder_Init();
	//采样定时器4来编码器测速，测试左电机
	Timer4_Enconder_Init();
	Timer2_Init();
	
	OLED_ShowString(1,1,"L:       R:       ");
	OLED_ShowString(2,1,"P:       GY:      ");
	
	leftSetSpeed(0);
	rightSetSpeed(0);
	
	
	while(1){
		OLED_ShowSignedNum(1,3,leftSpeed,5);
		OLED_ShowSignedNum(1,11,rightSpeed,5);
		//计数欧拉角
		IMU_getEuleranAngles();
		OLED_ShowSignedNum(2,3,imu_Angle.Pitch,5);//显示俯仰角
		OLED_ShowSignedNum(2,13,mpu6050.gyro_y,5);//Y轴加速度
		
		//调用PID_Balance 来根据俯仰角来计算速度
		int32_t speed=PID_Balance(imu_Angle.Pitch,mpu6050.gyro_y);
		if(speed>=7200){
			speed=7199;
		
		}
		else if (speed<=-7200){
		
		speed=-7199;
		}
		leftSetSpeed(speed);
		rightSetSpeed(speed);
	}
}



*/


int main(){

	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	
	Timer3_Enconder_Init();
	Timer4_Enconder_Init();
	Timer2_Init();
	USART_Model_Init();

	//OLED_ShowSignedNum 实际占用 1+Length 列：第1列是符号，数字从 Column+1 开始
	OLED_ShowString(1, 1, "L:      R:");	
	OLED_ShowString(2, 1, "P:      GY");	
	OLED_ShowString(3, 1, "M:    P:");		
	OLED_ShowString(4, 1, "D:");			

	// 先关电机，让 MPU6050 上电收敛（陀螺仪零偏稳定）
	leftSetSpeed(0);
	rightSetSpeed(0);
	delay_by_ms(2000);

	uint8_t oledCounter = 0;
	while (1) {
		// OLED 每 10 次空循环刷一次，避免频繁占用 CPU
		if (++oledCounter >= 10) {
			oledCounter = 0;

			// MachineBalance 是浮点，放大10倍后拆成整数位和小数位显示
			// 符号必须按放大后的值判断：整数部分为0时 OLED_ShowSignedNum 会显示"+"
			int16_t mbTenth = (int16_t)(MachineBalance * 10 + (MachineBalance >= 0 ? 0.5f : -0.5f));
			int16_t mbAbs = mbTenth < 0 ? -mbTenth : mbTenth;
			OLED_ShowChar(3, 3, mbTenth < 0 ? '-' : '+');
			OLED_ShowNum(3, 4, (uint16_t)(mbAbs / 10), 1);
			OLED_ShowChar(3, 5, '.');
			OLED_ShowNum(3, 6, (uint16_t)(mbAbs % 10), 1);
			OLED_ShowSignedNum(3, 9, BalanceKP, 4);
			int16_t kdTenth = (int16_t)(BalanceKD * 10 + (BalanceKD >= 0 ? 0.5f : -0.5f));
			int16_t kdAbs = kdTenth < 0 ? -kdTenth : kdTenth;
			OLED_ShowChar(4, 3, kdTenth < 0 ? '-' : '+');
			OLED_ShowNum(4, 4, (uint16_t)(kdAbs / 10), 1);
			OLED_ShowChar(4, 5, '.');
			OLED_ShowNum(4, 6, (uint16_t)(kdAbs % 10), 1);

			OLED_ShowSignedNum(2, 3, imu_Angle.Pitch, 5);
			OLED_ShowSignedNum(2, 11, mpu6050.gyro_y, 5);

			OLED_ShowSignedNum(1, 3, leftSpeed, 5);
			OLED_ShowSignedNum(1, 11, rightSpeed, 5);
		}
	}








}

/*
int main(){
	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	//采用定时器3来编码器测试右电机
	Timer3_Enconder_Init();
	//采样定时器4来编码器测速，测试左电机
	Timer4_Enconder_Init();
	Timer2_Init();
	
	OLED_ShowString(1,1,"L:       R:      ");
	OLED_ShowString(2,1,"P:       GY:     ");
	
	leftSetSpeed(0);
	rightSetSpeed(0);
	
	
	while(1){
		//计数欧拉角
		IMU_getEuleranAngles();
		OLED_ShowSignedNum(2,3,imu_Angle.Pitch,5);//显示俯仰角
		OLED_ShowSignedNum(2,13,mpu6050.gyro_y,5);//Y轴加速器
	
		
		OLED_ShowSignedNum(1,3,leftSpeed,5);
		OLED_ShowSignedNum(1,11,rightSpeed,5);
	
		//调用PID_Balance 来根据俯仰角和Y轴角速度计算速度
		int32_t speed=
	
	
	
	
	
	
	
	
	
	
	}

}






*/




/*
int main(void){
	OLED_Init();
	MPU6050_Init();

	//获取id
	
	OLED_ShowString(1,1,"ID:");
	OLED_ShowHexNum(1,5,Get_MPU6050_ID(),2);
	
	OLED_ShowString(1,1,"Pitch:");
	OLED_ShowString(2,1,"Roll:");
	OLED_ShowString(3,1,"Yaw:");

	while(1){
		
		//计算欧拉角
		IMU_getEuleranAngles();
		OLED_ShowSignedNum(1,7,imu_Angle.Pitch,5);
		OLED_ShowSignedNum(2,7,imu_Angle.Roll,5);
		OLED_ShowSignedNum(3,7,imu_Angle.Yaw,5);
		
		
		
		
		
		
		
		
		

	}
}
*/
