#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "stm32f4xx_hal.h"

// 定义步进电机的步数每转
#define STEPPER_MOTOR_STEPS_PER_REVOLUTION 200

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} IN_TypeDef;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} DIR_TypeDef;

// 步进电机句柄结构体
typedef struct
{
    IN_TypeDef in1;
    IN_TypeDef in2;
    IN_TypeDef in3;
    IN_TypeDef in4;
    DIR_TypeDef dir;
    TIM_HandleTypeDef *htim;     // 定时器句柄
    uint32_t channel;           // 定时器通道
    uint16_t pulse;             // PWM脉冲数
    uint16_t duty;              // PWM占空比
    uint32_t stepsPerRevolution; // 每转步数
} StepperMotor_HandleTypeDef;

// 步进电机操作结果的枚举
typedef enum
{
    DIR_LEFT = 0,
    DIR_RIGHT

} StepperMotor_DirTypeDef;


// 42步进电机初始化
void StepperMotor_Init(StepperMotor_HandleTypeDef *hMotor, TIM_HandleTypeDef *htim, uint32_t channel, GPIO_TypeDef *dir_port, uint16_t dir_pin,uint16_t pulse,uint16_t duty);

// 启动42步进电机
void StepperMotor_Start(StepperMotor_HandleTypeDef *hMotor,StepperMotor_DirTypeDef dir);

// 设置42步进电机的速度（RPM）
void StepperMotor_SetSpeed(StepperMotor_HandleTypeDef *hMotor, uint32_t speedRPM,StepperMotor_DirTypeDef dir);

// 转动指定的步数
void StepperMotor_Step(StepperMotor_HandleTypeDef *hMotor, uint32_t steps, uint32_t delayBetweenSteps);

// 停止42步进电机
void StepperMotor_Stop(StepperMotor_HandleTypeDef *hMotor);

// 步进28BYJ电机初始化
void StepperMotor_28BYJ_Init(StepperMotor_HandleTypeDef *hMotor,
                            GPIO_TypeDef *port1,uint16_t pin1,
                            GPIO_TypeDef *port2,uint16_t pin2,
                            GPIO_TypeDef *port3,uint16_t pin3,
                            GPIO_TypeDef *port4,uint16_t pin4,
                            uint16_t pulse);

// 设置28BYJ步进电机的速度（RPM）
void StepperMotor_28BYJ_SetSpeed(StepperMotor_HandleTypeDef *hMotor, uint32_t speedRPM);

// 转动指定的步数
void StepperMotor_28BYJ_Step(StepperMotor_HandleTypeDef *hMotor, uint32_t steps, uint32_t delayBetweenSteps);

// 停止28BYJ步进电机
void StepperMotor_28BYJ_Stop(StepperMotor_HandleTypeDef *hMotor);

#endif // __STEPPER_MOTOR_H
