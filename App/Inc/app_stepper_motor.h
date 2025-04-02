#ifndef __APP_STEPPER_MOTOR_H
#define __APP_STEPPER_MOTOR_H

#include "stepper_motor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

// 初始化两个步进电机
void App_StepperMotors_Init(void);

//步进电机任务进程
void StepMotorControlTask(void * argument);
// 步进电机的处理函数
void App_StepperMotor_Proc(void);

void App_StepperMotor_28BJY_Proc(void);
// 测试步进电机
void App_StepperMotor_Test(void);

void APP_StepperMotor_SetSpeed_Min(StepperMotor_HandleTypeDef *hMotor, uint32_t speedPPM, StepperMotor_DirTypeDef dir);
void APP_StepperMotor_SetSpeed_Sec(StepperMotor_HandleTypeDef *hMotor, uint32_t speedPPS, StepperMotor_DirTypeDef dir);
// // 设置步进电机的速度（RPM）
// void App_StepperMotor_SetSpeed(StepperMotor_HandleTypeDef *hMotor, uint32_t speedRPM);

// // 转动指定的步数
// void App_StepperMotor_Step(StepperMotor_HandleTypeDef *hMotor, uint32_t steps, uint32_t delayBetweenSteps);

// // 停止步进电机
// void App_StepperMotor_Stop(StepperMotor_HandleTypeDef *hMotor);

#endif // __APP_STEPPER_MOTOR_H
