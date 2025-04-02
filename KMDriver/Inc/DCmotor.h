#ifndef __DCMOTOR_H
#define __DCMOTOR_H

#include "stm32f4xx_hal.h"
#include "stepper_motor.h"

#define DCMOTOR_DIR_R 1
#define DCMOTOR_DIR_L 0


// 直流单机句柄结构体
typedef struct
{
    IN_TypeDef in1;
    IN_TypeDef in2;
    uint16_t dir;
    TIM_HandleTypeDef *htim;     // 定时器句柄
    uint32_t channel;           // 定时器通道
    uint16_t pulse;             // PWM脉冲数
    uint16_t duty;              // PWM占空比
} DCMotor_HandleTypeDef;


void APP_DCmotor_Start(DCMotor_HandleTypeDef* DCmotor,uint16_t dir);
void APP_DCmotor_Stop(DCMotor_HandleTypeDef* DCmotor);
void App_DCmotor_Proc(void);


#endif // __SERVO_MOTOR_H
