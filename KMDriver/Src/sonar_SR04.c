#include "sonar_SR04.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "main.h"


// 定义超声波传感器的句柄
Sonar_HandleTypeDef Sonar1 = {&htim3, TIM_CHANNEL_1,TIM_CHANNEL_2,
                                            GPIOA, GPIO_PIN_7,
                                            GPIOA, GPIO_PIN_6};
Sonar_HandleTypeDef Sonar2 = {&htim4, TIM_CHANNEL_1,TIM_CHANNEL_2,
                                            GPIOD, GPIO_PIN_11,
                                            GPIOD, GPIO_PIN_12};
Sonar_HandleTypeDef Sonar3 = {&htim5, TIM_CHANNEL_3,TIM_CHANNEL_4,
                                            GPIOA, GPIO_PIN_4,
                                            GPIOA, GPIO_PIN_2};
Sonar_HandleTypeDef Sonar4 = {&htim12, TIM_CHANNEL_1,TIM_CHANNEL_2,
                                            GPIOB, GPIO_PIN_13,
                                            GPIOB, GPIO_PIN_14};

void Sonar_Init(Sonar_HandleTypeDef *sonar)
{
    // GPIO_InitTypeDef GPIO_InitStruct = {0};

    // // 使能GPIO时钟
    // __HAL_RCC_GPIOA_CLK_ENABLE();
    // __HAL_RCC_GPIOB_CLK_ENABLE();
    // __HAL_RCC_GPIOD_CLK_ENABLE();

    // // 配置触发GPIO为输出模式
    // GPIO_InitStruct.Pin = sonar->trig_pin;
    // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // 根据实际情况选择正确的GPIO端口

    // // 配置回波GPIO为输入模式
    // GPIO_InitStruct.Pin = sonar->echo_pin;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStruct.Alternate = 0; // 根据实际情况选择正确的Alternate Function
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // 根据实际情况选择正确的GPIO端口

    // 启动定时器PWM输出
    // HAL_TIM_IC_Start(sonar->htim, sonar->channel);
}

void Sonar_clearData(Sonar_HandleTypeDef *sonar)
{
    __HAL_TIM_SET_COUNTER(sonar->htim, 0);

    if(sonar->dir_channel == TIM_CHANNEL_1)
    {
    __HAL_TIM_CLEAR_FLAG(sonar->htim, TIM_FLAG_CC1);
    __HAL_TIM_CLEAR_FLAG(sonar->htim, TIM_FLAG_CC2);
    }
    else if (sonar->dir_channel == TIM_CHANNEL_3)
    {
    __HAL_TIM_CLEAR_FLAG(sonar->htim, TIM_FLAG_CC3);
    __HAL_TIM_CLEAR_FLAG(sonar->htim, TIM_FLAG_CC4);
    }
}



float Sonar_GetDistance(Sonar_HandleTypeDef *sonar)
{

    Sonar_clearData(sonar);

    HAL_TIM_IC_Start(sonar->htim, sonar->dir_channel);
    HAL_TIM_IC_Start(sonar->htim, sonar->indir_channel);

    // 触发超声波传感器
    HAL_GPIO_WritePin(sonar->trig_port, sonar->trig_pin, GPIO_PIN_SET);
    delay_us(20); // 触发时间
    HAL_GPIO_WritePin(sonar->trig_port, sonar->trig_pin, GPIO_PIN_RESET);

    uint32_t expireTime = HAL_GetTick() + 50;
    while(expireTime > HAL_GetTick())
    {
        if(sonar->dir_channel == TIM_CHANNEL_1)
        {
            if(__HAL_TIM_GET_FLAG(sonar->htim, TIM_FLAG_CC1) && __HAL_TIM_GET_FLAG(sonar->htim, TIM_FLAG_CC2))
            {

                break;
            }
        }
        else if (sonar->dir_channel == TIM_CHANNEL_3)
        {
            if(__HAL_TIM_GET_FLAG(sonar->htim, TIM_FLAG_CC3) && __HAL_TIM_GET_FLAG(sonar->htim, TIM_FLAG_CC4))
            {

                break;
            }
        }
    }

    HAL_TIM_IC_Stop(sonar->htim, sonar->dir_channel);
    HAL_TIM_IC_Stop(sonar->htim, sonar->indir_channel);

    uint32_t dir_val = __HAL_TIM_GET_COMPARE(sonar->htim,sonar->dir_channel);
    uint32_t indir_val = __HAL_TIM_GET_COMPARE(sonar->htim,sonar->indir_channel);

    float pulasewidth = (indir_val - dir_val)*1e-6f;

    //单位CM
    sonar->distance = pulasewidth*340.0f*100.0f/(2.0f*2);

    // 等待回波
    // 这里应该添加代码来测量回波时间

    return sonar->distance; // 返回测量的距离
}
