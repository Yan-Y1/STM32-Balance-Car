#include "MPU6050.h"                  // Device header

//初始化MPU6050
void MPU6050_Init(void){

	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);

	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;//设置成开漏输出模式，原本是低电平引脚输出高电平，外部上拉电阻拉高电平
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//配置I2C
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;	//设置成自动应答位
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;	//指定7bit的地址
	I2C_InitStruct.I2C_ClockSpeed=100000;	//时钟频率
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_16_9;	//占空比配置
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;	//I2C模式
	I2C_InitStruct.I2C_OwnAddress1=0x00;	//自己的地址，因为为主机为发送，不是从机，所以可以不指定自身地址
	I2C_Init(I2C2,&I2C_InitStruct);

	//使能I2C2
	I2C_Cmd(I2C2,ENABLE);

	//通过MPU6050_WriteReg 向指定的寄存器写入配置值
	MPU6050_WriteReg(SMPLRT_DIV,0x02);
	MPU6050_WriteReg(CONFIG,0x06);
	MPU6050_WriteReg(GYRO_CONFIG,0x18);
	MPU6050_WriteReg(ACCEL_CONFIG,0x18);
	MPU6050_WriteReg(PWR_MGMT_1,0x01);
	MPU6050_WriteReg(PWR_MGMT_2,0x00);
	delay_by_ms(10);
}

/*
  *     @arg I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED           : EV1
  *     @arg I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED              : EV1
  *     @arg I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED     : EV1
  *     @arg I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED        : EV1
  *     @arg I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED            : EV1
  *     @arg I2C_EVENT_SLAVE_BYTE_RECEIVED                         : EV2
  *     @arg (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_DUALF)      : EV2
  *     @arg (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_GENCALL)    : EV2
  *     @arg I2C_EVENT_SLAVE_BYTE_TRANSMITTED                      : EV3
  *     @arg (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_DUALF)   : EV3
  *     @arg (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_GENCALL) : EV3
  *     @arg I2C_EVENT_SLAVE_ACK_FAILURE                           : EV3_2
  *     @arg I2C_EVENT_SLAVE_STOP_DETECTED                         : EV4
  *     @arg I2C_EVENT_MASTER_MODE_SELECT                          : EV5
  *     @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6
  *     @arg I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
  *     @arg I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
  *     @arg I2C_EVENT_MASTER_MODE_ADDRESS10                       : EV9
  *

*/


//向MPU6050指定的寄存器写入数据
void MPU6050_WriteReg(uint8_t regAddr,uint8_t data){
	//发送数据开始信号
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS); //EV5

	//发送MPU6050的地址+写位，1101 000+0=>D0
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);	//等待从机应答
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);	//EV6

	//发送寄存器的地址
	I2C_SendData(I2C2,regAddr);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS); //EV8

	//发送数据
	I2C_SendData(I2C2,data);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS); //EV8_2

	//发送终止信号
	I2C_GenerateSTOP(I2C2,ENABLE);

}

//向MPU6050指定的寄存器读取数据
uint8_t MPU6050_ReadReg(uint8_t regAddr){
	//发送数据开始信号
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//EV5

	//发送MPU6050的地址+写位，1101 000+0=>D0,MPU6050_ADDRESS:0xD0
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);//EV6

	//发送寄存器的地址
	I2C_SendData(I2C2,regAddr);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);	//EV8

	//======再次读取============
	//发送开始信号
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS); //EV5

	//发送MPU6050的地址+读位，1101000 | 0000 0001==>D1  ; I2C_Direction_Receiver :此时是接收模式
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS | 0x01,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//Ev6----已接收到EV6


	//参考手册，在等待EV6 事件后，关闭自动应答以及发送终止信号
	//关闭应答
	I2C_AcknowledgeConfig(I2C2,DISABLE);

	//发送终止信号
	I2C_GenerateSTOP(I2C2,ENABLE);

	//等待之前需要判断的EV7 事件，也就是看RXNE 是否为1
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS); //EV7

	//读取数据
	uint8_t data=I2C_ReceiveData(I2C2);

	//恢复自动应答
	I2C_AcknowledgeConfig(I2C2,ENABLE);

	return data;

}

/*
//连续读取xyz 的加速度和角速度的值
void MPU6050_ReadRegArray(uint8_t regAddr,uint8_t *data,uint8_t length){
	//发送数据开始信号
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//EV5

	//发送MPU6050的地址+写位，1101 000+0=>D0,MPU6050_ADDRESS:0xD0
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);//EV6

	//发送寄存器的地址
	I2C_SendData(I2C2,regAddr);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);	//EV8

	//======再次读取============
	//发送开始信号
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS); //EV5

	//发送MPU6050的地址+读位，1101000 | 0000 0001==>D1  ; I2C_Direction_Receiver :此时是接收模式
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS | 0x01,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//Ev6----已接收到EV6

	//循环读取指定数组长度的寄存器
	for(uint8_t i=0;i<length;i++){
		//当读取的是最后一次，需要先关闭自动应答以及发送终止信号
		if(i==length-1){
			//参考手册，在发送EV6 事件后，关闭自动应答以及发送终止信号
			//关闭自动应答
			I2C_AcknowledgeConfig(I2C2,DISABLE);

			//发送终止信号
			I2C_GenerateSTOP(I2C2,ENABLE);

		}
		//等待之前需要判断EV7事件，也就是看RXNE是否为真
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);//EV7
		data[i]=I2C_ReceiveData(I2C2);

	}
	//恢复自动应答
	I2C_AcknowledgeConfig(I2C2,ENABLE);


}

*/
void MPU6050_ReadRegArray(uint8_t regAddr,uint8_t *data,uint8_t length){
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//EV5

	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);//EV6

	I2C_SendData(I2C2,regAddr);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);	//EV8

	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS); //EV5

	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//EV6

	for(uint8_t i=0;i<length;i++){
		if(i == length -1){
			I2C_AcknowledgeConfig(I2C2,DISABLE); //最后一字节提前关闭ACK
		}
		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);//EV7
		data[i]=I2C_ReceiveData(I2C2);
	}
	I2C_GenerateSTOP(I2C2,ENABLE); //全部读完再发停止
	I2C_AcknowledgeConfig(I2C2,ENABLE);
}

uint8_t Get_MPU6050_ID(void)
{
	return MPU6050_ReadReg(WHO_AM_I);
}

//获取MPU6050的数据
void getMPU6050_Data(MPU6050_DataStruct *data){
//	data->accel_x=(MPU6050_ReadReg(ACCEL_XOUT_H) << 8) | MPU6050_ReadReg(ACCEL_XOUT_L);
//	data->accel_y=(MPU6050_ReadReg(ACCEL_YOUT_H) << 8) | MPU6050_ReadReg(ACCEL_YOUT_H);
//	data->accel_y=(MPU6050_ReadReg(ACCEL_ZOUT_H) << 8) | MPU6050_ReadReg(ACCEL_ZOUT_H);

//	data->gyro_x=(MPU6050_ReadReg(GYRO_XOUT_H) << 8) | MPU6050_ReadReg(GYRO_XOUT_L);
//	data->gyro_y=(MPU6050_ReadReg(GYRO_XOUT_H) << 8) | MPU6050_ReadReg(GYRO_XOUT_L);
//	data->gyro_z=(MPU6050_ReadReg(GYRO_ZOUT_H) << 8) | MPU6050_ReadReg(GYRO_ZOUT_L);

	//连续读取
	//指定一个x加速度高位寄存器地址，然后连续循环每次读取时，寄存器的地址会自动递增连着寄存器的地址连续输出
	uint8_t arr[14]={0};
	MPU6050_ReadRegArray(ACCEL_XOUT_H,arr,sizeof(arr));

	//arr[0]---x加速度高8位，arr[1]--x加速度低8位
	data->accel_x=(arr[0] << 8) | arr[1];
	data->accel_y=(arr[2] << 8) | arr[3];
	data->accel_z=(arr[4] << 8) | arr[5];

	//arr[6]和arr[7]是MPU6050温度传感器的数值，这里不需要
	data->gyro_x=(arr[8]<<8)|arr[9];
	data->gyro_y=(arr[10]<<8)|arr[11];
	data->gyro_z=(arr[12]<<8)|arr[13];




}
