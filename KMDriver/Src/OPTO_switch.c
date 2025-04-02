#include "OPTO_switch.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "main.h"
#include "stdio.h"

int infrared_state1 = 0;
int reflective_state1 = 0;
int reflective_state2 = 0;
int reflective_state3 = 0;
int reflective_state4 = 0;
// /* 红外对射传感器中断初始化 */
// void Infrared_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     /* 使能GPIOF时钟 */
//     __HAL_RCC_GPIOF_CLK_ENABLE();

//     /* 配置PF0引脚为中断输入 */
//     GPIO_InitStruct.Pin = GPIO_PIN_0;
//     GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // 下降沿触发
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//     /* 配置EXTI0 */
//     HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(EXTI0_IRQn);
// }


/* 回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // if (GPIO_Pin == Infrared_switch1_Pin)
    // {
    //     infrared_state1 = 1; // 表示红外开关检测到物体
    //     printf("infrared_state1 = %d\r\n", infrared_state1);
    // }

    if (GPIO_Pin == Reflect_switch1_Pin)
    {
        // 读取当前的GPIO引脚状态
        GPIO_PinState pinState = HAL_GPIO_ReadPin(Reflect_switch1_GPIO_Port, Reflect_switch1_Pin);
        if (pinState == GPIO_PIN_SET)
        {
            reflective_state1 = 1; // 表示光电开关检测到物体
            // printf("reflective_state1 = %d\r\n", reflective_state1);
        }
        else
        {
            reflective_state1 = 0; // 表示光电开关未检测到物体
            // printf("reflective_state1 = %d\r\n", reflective_state1);
        }
        // 这里可以添加你的处理代码
    }

    if (GPIO_Pin == Reflect_switch2_Pin)
    {
        // 读取当前的GPIO引脚状态
        GPIO_PinState pinState = HAL_GPIO_ReadPin(Reflect_switch2_GPIO_Port, Reflect_switch2_Pin);
        if (pinState == GPIO_PIN_SET)
        {
            reflective_state1 = 1; // 表示光电开关检测到物体
            // printf("reflective_state2 = %d\r\n", reflective_state2);
        }
        else
        {
            reflective_state2 = 0; // 表示光电开关未检测到物体
            // printf("reflective_state2 = %d\r\n", reflective_state2);
        }
        // 这里可以添加你的处理代码
    }

    if (GPIO_Pin == Reflect_switch3_Pin)
    {
        // 读取当前的GPIO引脚状态
        GPIO_PinState pinState = HAL_GPIO_ReadPin(Reflect_switch3_GPIO_Port, Reflect_switch3_Pin);
        if (pinState == GPIO_PIN_SET)
        {
            reflective_state3 = 1; // 表示光电开关检测到物体
            // printf("reflective_state3 = %d\r\n", reflective_state3);
        }
        else
        {
            reflective_state3 = 0; // 表示光电开关未检测到物体
            // printf("reflective_state3 = %d\r\n", reflective_state3);
        }
        // 这里可以添加你的处理代码
    }

    if (GPIO_Pin == Reflect_switch4_Pin)
    {
        // 读取当前的GPIO引脚状态
        GPIO_PinState pinState = HAL_GPIO_ReadPin(Reflect_switch4_GPIO_Port, Reflect_switch4_Pin);
        if (pinState == GPIO_PIN_SET)
        {
            reflective_state4 = 1; // 表示光电开关检测到物体
            // printf("reflective_state4 = %d\r\n", reflective_state4);
        }
        else
        {
            reflective_state4 = 0; // 表示光电开关未检测到物体
            // printf("reflective_state4 = %d\r\n", reflective_state4);
        }
        // 这里可以添加你的处理代码
    }
}

