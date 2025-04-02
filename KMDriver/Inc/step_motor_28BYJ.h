#ifndef __STEPPER_MOTOR_28BYJ_H
#define __STEPPER_MOTOR_28BYJ_H

#include "stm32f4xx_hal.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} IN_TypeDef;

// 步进电机句柄结构体
typedef struct
{
    IN_TypeDef in1;
    IN_TypeDef in2;
    IN_TypeDef in3;
    IN_TypeDef in4;
    uint32_t pulse;
} StepperMotor_28BYJ_HandleTypeDef;

// 步进电机初始化
void StepperMotor_28BYJ_Init(StepperMotor_28BYJ_HandleTypeDef *hMotor, TIM_HandleTypeDef *htim, uint32_t channel, uint16_t pulse);

// 设置步进电机的速度（RPM）
void StepperMotor_28BYJ_SetSpeed(StepperMotor_28BYJ_HandleTypeDef *hMotor, uint32_t speedRPM);

// 转动指定的步数
void StepperMotor_28BYJ_Step(StepperMotor_28BYJ_HandleTypeDef *hMotor, uint32_t steps, uint32_t delayBetweenSteps);

// 停止步进电机
void StepperMotor_28BYJ_Stop(StepperMotor_28BYJ_HandleTypeDef *hMotor);

#endif // __STEPPER_MOTOR_H
