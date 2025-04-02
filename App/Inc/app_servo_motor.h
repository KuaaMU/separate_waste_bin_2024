#ifndef __APP_SERVO_MOTOR_H
#define __APP_SERVO_MOTOR_H

#include "servo_motor.h"



// 初始化舵机
void App_Servo_Init(void);

//舵机进程函数
void ServoControlTask(void * argument);

//测试舵机控制
void App_Servo_Test(void);

// 舵机控制主循环
void App_Servo_Proc(void);

// 设置舵机角度
void App_Servo_SetAngle(Servo_HandleTypeDef *hServo, uint16_t angle);

//云台回正
void App_Servo_Reset(void);

//设置云台转动情况
void App_cloud_terrace_state(int state);

//云台震动
void APP_cloud_vibrating(void);


#endif // __APP_SERVO_MOTOR_H
