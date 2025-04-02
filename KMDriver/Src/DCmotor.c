#include "DCmotor.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"

#define DCDELAY_MS  35000

DCMotor_HandleTypeDef DCmotor1={
                                .dir=DCMOTOR_DIR_L,
                                .in1={
                                    DCmotor1_IN1_GPIO_Port,
                                    DCmotor1_IN1_Pin
                                },
                                .in2={
                                    DCmotor1_IN2_GPIO_Port,
                                    DCmotor1_IN2_Pin
                                }
};

void App_DCmotor_Proc(void)
{
    APP_DCmotor_Start(&DCmotor1,DCMOTOR_DIR_R);
    vTaskDelay(pdMS_TO_TICKS(DCDELAY_MS));
    APP_DCmotor_Start(&DCmotor1,DCMOTOR_DIR_L);
    vTaskDelay(pdMS_TO_TICKS(DCDELAY_MS+3000));
    APP_DCmotor_Stop(&DCmotor1);
}

void APP_DCmotor_Start(DCMotor_HandleTypeDef* DCmotor,uint16_t dir)
{
    if(dir==DCMOTOR_DIR_R)
    {
        HAL_GPIO_WritePin(DCmotor->in1.port,DCmotor->in1.pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DCmotor->in2.port,DCmotor->in2.pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(DCmotor->in1.port,DCmotor->in1.pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(DCmotor->in2.port,DCmotor->in2.pin,GPIO_PIN_RESET);
    }
}

void APP_DCmotor_Stop(DCMotor_HandleTypeDef* DCmotor)
{
    HAL_GPIO_WritePin(DCmotor->in1.port,DCmotor->in1.pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DCmotor->in2.port,DCmotor->in2.pin,GPIO_PIN_RESET);
}
