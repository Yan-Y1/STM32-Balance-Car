#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//MPU6050宏定义
#define MPU6050_ADDRESS 0xD0

//指定配置寄存器的地址
#define SMPLRT_DIV 0x19 //采样率寄存器
#define CONFIG 0x1A //MPU6050 配置寄存器
#define GYRO_CONFIG 0x1B // 陀螺仪配置寄存器
#define ACCEL_CONFIG 0x1C //加速度仪配置寄存器
#define PWR_MGMT_1 0x6B //电源管理寄存器
#define PWR_MGMT_2 0x6C //电源管理寄存器

//指定XYZ 的数据的寄存器的地址
#define ACCEL_XOUT_H 0x3B //x加速度的高8位寄存器
#define ACCEL_XOUT_L 0x3C //x加速度的低8位寄存器#define ACCEL_XOUT_H 0x3B //x加速度的高8位寄存器

#define ACCEL_YOUT_H 0x3D //y加速度的高8位寄存器
#define ACCEL_YOUT_L 0x3E //y加速度的低8位寄存器

#define ACCEL_ZOUT_H 0x3F //z加速度的高8位寄存器
#define ACCEL_ZOUT_L 0x40 //z加速度的低8位寄存器


#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define GYRO_XOUT_H 0x43 //x角速度的高8位寄存器
#define GYRO_XOUT_L 0x44 //x角速度的低8位寄存器
#define GYRO_YOUT_H 0x45 //y角速度的高8位寄存器
#define GYRO_YOUT_L 0x46 //y角速度的低8位寄存器
#define GYRO_ZOUT_H 0x47 //z角速度的高8位寄存器
#define GYRO_ZOUT_L 0x48 //z角速度的低8位寄存器

#define WHO_AM_I 0x75

//存放MPU6050的XYZ 数据结果的结构体
typedef struct{
	//加速度
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;

	//陀螺仪
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;


}MPU6050_DataStruct;

//初始化MPU6050
void MPU6050_Init(void);

//向MPU6050指定的寄存器写入数据
void MPU6050_WriteReg(uint8_t regAddr,uint8_t data);

//向MPU6050指定的寄存器读取数据
uint8_t MPU6050_ReadReg(uint8_t regAddr);

//连续读取xyz 的加速度和角速度的值
void MPU6050_ReadRegArray(uint8_t regAddr,uint8_t *data,uint8_t length);

//获取MPU6050的数据
void getMPU6050_Data(MPU6050_DataStruct *data);

//获取MPU6050设备的ID
uint8_t Get_MPU6050_ID(void);

#endif
