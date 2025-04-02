#include "stepper_motor.h"
#include <stdio.h>
#include "tim.h"
#include "FreeRTOS.h"
#include "task.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
// extern TIM_HandleTypeDef htim9;
// extern TIM_HandleTypeDef htim10;
// extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;

// 步进电机初始化
// 步进电机初始化
void StepperMotor_Init(StepperMotor_HandleTypeDef *hMotor, TIM_HandleTypeDef *htim, uint32_t channel, GPIO_TypeDef *dir_port, uint16_t dir_pin,uint16_t pulse,uint16_t duty)
{
    // 设置电机的方向
    hMotor->dir.port = dir_port;
    hMotor->dir.pin = dir_pin;
    HAL_GPIO_WritePin(hMotor->dir.port, hMotor->dir.pin, GPIO_PIN_SET); // 默认方向

    hMotor->htim = htim;
    hMotor->channel = channel;
    hMotor->pulse = pulse;
    hMotor->duty = duty;
    hMotor->stepsPerRevolution = STEPPER_MOTOR_STEPS_PER_REVOLUTION;

    HAL_TIM_OC_Start_IT(htim, channel);
    TIM_CCxChannelCmd(htim->Instance, channel, TIM_CCx_ENABLE);
    HAL_TIM_PWM_Start(htim, channel);
}

void StepperMotor_Start(StepperMotor_HandleTypeDef *hMotor,StepperMotor_DirTypeDef dir)
{
    HAL_GPIO_WritePin(hMotor->dir.port, hMotor->dir.pin, dir);
    HAL_TIM_OC_Start_IT(hMotor->htim, hMotor->channel);
}



// 设置步进电机的速度（每分钟几个脉冲PPM）
void StepperMotor_SetSpeed(StepperMotor_HandleTypeDef *hMotor, uint32_t speedPPM, StepperMotor_DirTypeDef dir)
{
    // 获取定时器的输入时钟频率
    uint32_t timerClockFrequency;

    if(hMotor->htim == &htim1 || hMotor->htim == &htim8 ) {
        timerClockFrequency = HAL_RCC_GetPCLK1Freq();
    } else {
        timerClockFrequency = HAL_RCC_GetPCLK2Freq();
    }

    // 检查时钟频率是否有效
    if (timerClockFrequency == 0) {
        // 处理错误情况
        return;
    }

    // 计算定时器的预分频值
    uint32_t prescalerValue = hMotor->htim->Init.Prescaler + 1;
    // 计算定时器的计数周期（单位：秒）
    float timerClockPeriodSec = 1.0f / (timerClockFrequency / prescalerValue);

    // 计算每分钟的脉冲数
    uint32_t pulsesPerMinute = speedPPM;

    // 计算每个脉冲的时间间隔
    float pulseIntervalSec = 60.0f / pulsesPerMinute;

    // 计算定时器的比较值
    uint32_t pulse = (uint32_t)(pulseIntervalSec / timerClockPeriodSec)/2;

    hMotor->pulse = pulse;
    // 设置方向
    HAL_GPIO_WritePin(hMotor->dir.port, hMotor->dir.pin, dir);

    // __HAL_TIM_SET_COMPARE(hMotor->htim, hMotor->channel, pulse);
    // HAL_TIM_OC_Start_IT(hMotor->htim, hMotor->channel);
}


// 停止步进电机
void StepperMotor_Stop(StepperMotor_HandleTypeDef *hMotor)
{
    HAL_TIM_PWM_Stop(hMotor->htim, hMotor->channel);
    HAL_TIM_OC_Stop_IT(hMotor->htim, hMotor->channel);
}

// 步进28BYJ电机初始化
void StepperMotor_28BYJ_Init(StepperMotor_HandleTypeDef *hMotor,
                            GPIO_TypeDef *port1, uint16_t pin1,
                            GPIO_TypeDef *port2, uint16_t pin2,
                            GPIO_TypeDef *port3, uint16_t pin3,
                            GPIO_TypeDef *port4, uint16_t pin4,
                            uint16_t pulse)
{
    hMotor->in1.port = port1;
    hMotor->in1.pin = pin1;
    hMotor->in2.port = port2;
    hMotor->in2.pin = pin2;
    hMotor->in3.port = port3;
    hMotor->in3.pin = pin3;
    hMotor->in4.port = port4;
    hMotor->in4.pin = pin4;
    hMotor->pulse = pulse;

    // 初始化GPIO引脚为输出模式
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = pin1;
    HAL_GPIO_Init(port1, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pin2;
    HAL_GPIO_Init(port2, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pin3;
    HAL_GPIO_Init(port3, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = pin4;
    HAL_GPIO_Init(port4, &GPIO_InitStruct);

    // 默认所有引脚低电平
    HAL_GPIO_WritePin(port1, pin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port2, pin2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port3, pin3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port4, pin4, GPIO_PIN_RESET);
}


// 设置28BYJ步进电机的速度（RPM）
void StepperMotor_28BYJ_SetSpeed(StepperMotor_HandleTypeDef *hMotor, uint32_t speedRPM)
{
    // 计算延时来设置速度
    // 这里的计算取决于您的步进电机和驱动板的具体参数
    // 例如，28BYJ-48通常有64步/圈，所以每步的延时是 60s/speedRPM/64
    uint32_t delayBetweenSteps = 60000 / (speedRPM * 64);
    hMotor->pulse = delayBetweenSteps;
}

// 转动指定的步数
void StepperMotor_28BYJ_Step(StepperMotor_HandleTypeDef *hMotor, uint32_t steps, uint32_t delayBetweenSteps)
{
    // 一个简单的8步序列
    uint8_t sequence[8] = {1, 2, 3, 4, 8, 12, 9, 6};
    for (uint32_t i = 0; i < steps; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            // 根据序列设置GPIO引脚
            HAL_GPIO_WritePin(hMotor->in1.port, hMotor->in1.pin, (sequence[j] & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(hMotor->in2.port, hMotor->in2.pin, (sequence[j] & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(hMotor->in3.port, hMotor->in3.pin, (sequence[j] & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(hMotor->in4.port, hMotor->in4.pin, (sequence[j] & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

            vTaskDelay(delayBetweenSteps);
        }

    }
}

// 停止28BYJ步进电机
void StepperMotor_28BYJ_Stop(StepperMotor_HandleTypeDef *hMotor)
{
    // 将所有GPIO引脚设置为低电平
    HAL_GPIO_WritePin(hMotor->in1.port, hMotor->in1.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hMotor->in2.port, hMotor->in2.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hMotor->in3.port, hMotor->in3.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hMotor->in4.port, hMotor->in4.pin, GPIO_PIN_RESET);
}

