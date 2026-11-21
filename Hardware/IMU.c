#include "stm32f10x.h"
#include "IMU.h"
#include "MPU6050.h"
#include "Math.h"

#define kp 				20.00f
#define ki 				0.001f
#define cycle_T 		0.005f		//200hz
#define half_T 			0.0025f

int16_t Ax,Ay,Az,Gx,Gy,Gz;
param_imu imu_data;

param_Angle imu_Angle;//定义欧拉角的结构体的变量

MPU6050_DataStruct mpu6050;

float q[4] = {1.0, 0.0, 0.0, 0.0};
float exInt = 0.0 ,eyInt = 0.0 ,ezInt = 0.0 ;

//////////////////////////////////////////////////////////////////////////////////
float fast_sqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);  //0x5f3759df是一个平方根倒数速算法
    y = *(float *) &i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

//////////////////////////////////////////////////////////////////////////////////
void IMU_GetValues(void)
{
	//获取陀螺仪的原始值赋值给全局变量
	getMPU6050_Data(&mpu6050);
	Ax = mpu6050.accel_x;//0~32768 
	Ay = mpu6050.accel_y;
	Az = mpu6050.accel_z;
	Gx = mpu6050.gyro_x;
	Gy = mpu6050.gyro_y;
	Gz = mpu6050.gyro_z;
	imu_data.AX = ((float)Ax)/2048;
	imu_data.AY = ((float)Ay)/2048;
	imu_data.AZ = ((float)Az)/2048;

	imu_data.GX = ((float)Gx)*0.001064;
	imu_data.GY = ((float)Gy)*0.001064;
	imu_data.GZ = ((float)Gz)*0.001064;
}

void IMU_AHRSupdate(param_imu* imu_temp)
{
	float ax = imu_temp->AX;
    float ay = imu_temp->AY;
    float az = imu_temp->AZ;
    float gx = imu_temp->GX;
    float gy = imu_temp->GY;
    float gz = imu_temp->GZ;

    float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];

    // 归一化加速度计测量值
    float norm = sqrtf(ax*ax + ay*ay + az*az);
    if (norm > 0.0001f) {
        ax /= norm;
        ay /= norm;
        az /= norm;
    }

    // 估计方向（在世界坐标系下）
    float vx = 2*(q1*q3 - q0*q2);
    float vy = 2*(q2*q3 + q0*q1);
    float vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // 误差：测量值 vs 估计值 的叉积（即姿态误差）
    float ex = (ay * vz - az * vy);
    float ey = (az * vx - ax * vz);
    float ez = (ax * vy - ay * vx);

    // 积分误差（用于零偏校正）
    exInt += ki * ex;
    eyInt += ki * ey;
    ezInt += ki * ez;

    // 补偿陀螺仪（PI 控制器）
    gx += kp * ex + exInt;
    gy += kp * ey + eyInt;
    gz += kp * ez + ezInt;

    // 四元数微分更新
    q0 += (-q1*gx - q2*gy - q3*gz) * half_T;
    q1 += ( q0*gx - q2*gz + q3*gy) * half_T;
    q2 += ( q0*gy + q1*gz - q3*gx) * half_T;
    q3 += ( q0*gz - q1*gy + q2*gx) * half_T;

    // 归一化
    norm = sqrtf(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    if (norm > 0.0001f) {
        norm = 1.0f / norm;
        q[0] = q0 * norm;
        q[1] = q1 * norm;
        q[2] = q2 * norm;
        q[3] = q3 * norm;
    }
}

void IMU_getEuleranAngles(void)
{
	IMU_GetValues();
    IMU_AHRSupdate(&imu_data);

    // 从四元数提取欧拉角（ZYX顺序）
    imu_Angle.Roll  = atan2f(2*(q[0]*q[1] + q[2]*q[3]), 1 - 2*(q[1]*q[1] + q[2]*q[2])) * 57.295779513f;
    imu_Angle.Pitch = asinf(2*(q[0]*q[2] - q[1]*q[3])) * 57.295779513f;
    imu_Angle.Yaw   = atan2f(2*(q[1]*q[2] + q[0]*q[3]), q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3]) * 57.295779513f;
	
}   
    
