#include "servo_motor.h"

// 舵机初始化
void Servo_Init(Servo_HandleTypeDef *hServo, TIM_HandleTypeDef *htim, uint32_t channel, uint16_t pulseMin, uint16_t pulseMax, uint16_t pulseNeutral)
{
    hServo->htim = htim;
    hServo->channel = channel;
    hServo->pulseMin = pulseMin;
    hServo->pulseMax = pulseMax;
    hServo->pulseNeutral = pulseNeutral;

    // TIM_OC_InitTypeDef sConfigOC = {0};
    // sConfigOC.OCMode = TIM_OCMODE_PWM1;
    // sConfigOC.Pulse = pulseNeutral;
    // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    // sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    // HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel);

    HAL_TIM_PWM_Start(htim, channel);
}

// 设置舵机角度
void Servo_SetAngle(Servo_HandleTypeDef *hServo, uint16_t angle)
{
    // 将角度转换为0-270度的脉冲宽度
    uint32_t angleRange = 270;
    uint32_t pulseRange = (uint32_t)(hServo->pulseMax - hServo->pulseMin);
    uint32_t pulse = (uint32_t)(hServo->pulseMin + ((angle * pulseRange) / angleRange));

    // 更新PWM信号的脉宽
    __HAL_TIM_SET_COMPARE(hServo->htim, hServo->channel, pulse);
}


// 设置舵机角度
// angleMin: 自定义的最小角度
// angleMax: 自定义的最大角度
void Servo_SetAngleCustomRange(Servo_HandleTypeDef *hServo, uint16_t angle, uint16_t angleMin, uint16_t angleMax)
{
    // 确保角度在指定的范围内
    if (angle < angleMin)
    {
        angle = angleMin;
    }
    else if (angle > angleMax)
    {
        angle = angleMax;
    }

    // 计算角度范围
    uint32_t angleRange = (uint32_t)(angleMax - angleMin);
    // 计算脉冲宽度范围
    uint32_t pulseRange = (uint32_t)(hServo->pulseMax - hServo->pulseMin);
    // 计算当前角度对应的脉冲宽度
    uint32_t pulse = (uint32_t)(hServo->pulseMin + ((angle - angleMin) * pulseRange) / angleRange)*2;

    // 更新PWM信号的脉宽
    __HAL_TIM_SET_COMPARE(hServo->htim, hServo->channel, pulse);
}
