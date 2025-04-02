#ifndef __SERVO_MOTOR_H
#define __SERVO_MOTOR_H

#include "stm32f4xx_hal.h"

// 舵机句柄结构体
typedef struct
{
    TIM_HandleTypeDef *htim;     // 定时器句柄
    uint32_t channel;           // 定时器通道
    uint16_t pulseMin;          // 舵机最小脉冲宽度
    uint16_t pulseMax;          // 舵机最大脉冲宽度
    uint16_t pulseNeutral;      // 舵机中立脉冲宽度
} Servo_HandleTypeDef;

// 舵机初始化
void Servo_Init(Servo_HandleTypeDef *hServo, TIM_HandleTypeDef *htim, uint32_t channel, uint16_t pulseMin, uint16_t pulseMax, uint16_t pulseNeutral);

// 设置舵机角度
void Servo_SetAngle(Servo_HandleTypeDef *hServo, uint16_t angle);

// 设置舵机角度（自定义角度范围）
void Servo_SetAngleCustomRange(Servo_HandleTypeDef *hServo, uint16_t angle, uint16_t angleMin, uint16_t angleMax);

#endif // __SERVO_MOTOR_H
