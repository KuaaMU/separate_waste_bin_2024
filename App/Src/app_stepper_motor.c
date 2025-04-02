#include "tim.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "gpio.h"
#include "OPTO_switch.h"
#include "app_stepper_motor.h"
#include "stepper_motor.h"
#include "app_visual_identity.h"



// 定义两个步进电机的句柄
StepperMotor_HandleTypeDef Motor1;//转盘电机1
StepperMotor_HandleTypeDef Motor2;//转盘电机2
StepperMotor_HandleTypeDef Motor3;//转盘电机3
StepperMotor_HandleTypeDef Motor4;//压缩推杆电机2
StepperMotor_HandleTypeDef Motor5;//28BYJ步进电机1



// 定时器句柄实例
extern TIM_HandleTypeDef htim1;

//外部变量

extern bool rubbish_is_detected; // 垃圾检测标志
extern int current_rubbish_type;//检测到的垃圾类型

extern int state1;
extern int state2;
extern int infrared_state1; // 红外传感器状态
extern int reflective_state1;
extern int reflective_state2;
extern int reflective_state3;
extern int reflective_state4;

int step_motor_state = 0;



// extern int Sonar1_full_state = 0;
// extern int Sonar2_full_state = 0;
// extern int Sonar3_full_state = 0;
// extern int Sonar4_full_state = 0;
extern int full_state ;

// 初始化两个步进电机
void App_StepperMotors_Init(void)
{
    //初始化步进电机1（使用TIM1通道1）PE9
    StepperMotor_Init(&Motor1, &htim1, TIM_CHANNEL_1, GPIOE,GPIO_PIN_10,1000,50);

    // // //初始化步进电机2（使用TIM1通道2）PE11
    // // StepperMotor_Init(&Motor2, &htim1, TIM_CHANNEL_2, 0);
    // StepperMotor_Init(&Motor2, &htim1, TIM_CHANNEL_2, GPIOE,GPIO_PIN_12,1000,50);

    // //初始化步进电机3（使用TIM1通道3）PE13
    // StepperMotor_Init(&Motor3, &htim1, TIM_CHANNEL_3, 0);
    // StepperMotor_Init(&Motor3, &htim1, TIM_CHANNEL_3, GPIOE,GPIO_PIN_15,1000,50);

    // //初始化步进电机4（使用TIM1通道4）PE14
    // StepperMotor_Init(&Motor4, &htim1, TIM_CHANNEL_4, 0);

    // StepperMotor_28BYJ_Init(&Motor5,
    //                         Motor5_In1_GPIO_Port, Motor5_In1_Pin,
    //                         Motor5_In2_GPIO_Port, Motor5_In2_Pin,
    //                         Motor5_In3_GPIO_Port, Motor5_In3_Pin,
    //                         Motor5_In4_GPIO_Port, Motor5_In4_Pin,
    //                         2);

}

void StepMotorControlTask(void * argument)
{
    // APP_StepperMotor_SetSpeed_Sec(&Motor1, 8000,DIR_RIGHT);
    // APP_StepperMotor_SetSpeed_Sec(&Motor2, 1000,DIR_RIGHT);
    // StepperMotor_Stop(&Motor1);
    // StepperMotor_Stop(&Motor2);
    // StepperMotor_Stop(&Motor3);
    StepperMotor_Stop(&Motor1);
    for( ;; )
	{
        // StepperMotor_Start(&Motor1,DIR_RIGHT);
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        App_StepperMotor_Proc();
	}

}
// 步进电机处理函数
void App_StepperMotor_Proc(void)
{
    if (state1 == 1 && rubbish_is_detected == false && full_state!=1 )// 如果红外传感器检测到垃圾,并且托盘无垃圾,且垃圾桶没满
    {
        //启动转盘电机
        //  StepperMotor_Start(&Motor3,DIR_RIGHT);
         StepperMotor_Start(&Motor1,DIR_RIGHT);
        //  printf("------start-------");
        // App_StepperMotor_SetSpeed(&Motor2, 10);
    }
    else
    {
        //停止转盘电机
        // StepperMotor_Stop(&Motor3);
        // printf("+++++++++++stop++++++++");
        StepperMotor_Stop(&Motor1);
    }

}
void App_StepperMotor_Test(void)
{

    // StepperMotor_28BYJ_Step(&Motor5,500,2);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    //启动转盘电机
    APP_StepperMotor_SetSpeed_Sec(&Motor1, 200,DIR_RIGHT);
    // App_StepperMotor_SetSpeed(&Motor2, 10);
    vTaskDelay(1000);
    //停止转盘电机
    StepperMotor_Stop(&Motor1);
    // App_StepperMotor_Stop(&Motor2);
    vTaskDelay(1000);

}


void App_StepperMotor_28BJY_Proc(void)
{
        // 一个简单的8步序列
    uint8_t sequence[8] = {1, 2, 3, 4, 8, 12, 9, 6};
    for ( ;state2==0; )
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            if(state2==1) break;
            // 根据序列设置GPIO引脚
            HAL_GPIO_WritePin(Motor5.in1.port, Motor5.in1.pin, (sequence[j] & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Motor5.in2.port, Motor5.in2.pin, (sequence[j] & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Motor5.in3.port, Motor5.in3.pin, (sequence[j] & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Motor5.in4.port, Motor5.in4.pin, (sequence[j] & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

            vTaskDelay(2);
        }
        if(state2==1) break;
    }
}

void APP_StepperMotor_SetSpeed_Min(StepperMotor_HandleTypeDef *hMotor, uint32_t speedPPM, StepperMotor_DirTypeDef dir)
{
    StepperMotor_SetSpeed(hMotor, speedPPM, dir);
}

void APP_StepperMotor_SetSpeed_Sec(StepperMotor_HandleTypeDef *hMotor, uint32_t speedPPS, StepperMotor_DirTypeDef dir)
{
    StepperMotor_SetSpeed(hMotor, speedPPS/60, dir);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 如果定时器中断来自定时器1
    if (htim->Instance == TIM1)
    {
        __IO uint16_t OC_Count = 0;
        // 通道1的中断处理
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            // 获取当前计数值
            OC_Count = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_1);
            // 设置通道1的CCR值
           if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)){ /* 判断此时的电平是否为低电平 */
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,OC_Count + Motor1.pulse - Motor1.duty * Motor1.pulse/100);
			}else{
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,OC_Count + Motor1.duty * Motor1.pulse/100);
			}
        }

        // 通道2的中断处理
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            // 获取当前计数值
            OC_Count = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_2);
            // 设置通道2的CCR值
           if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11)){ /* 判断此时的电平是否为低电平 */
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,OC_Count + Motor2.pulse - Motor2.duty * Motor2.pulse/100);
			}else{
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,OC_Count + Motor2.duty * Motor2.pulse/100);
			}
        }

        // // 通道3的中断处理
        // if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        // {
        //     // 获取当前计数值
        //     OC_Count = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_3);
        //     // 设置通道3的CCR值
        //     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, currentCount + Motor3.pulse);
        // }

        // // 通道4的中断处理
        // if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        // {
        //     // 获取当前计数值
        //     OC_Count = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_4);
        //     // 设置通道4的CCR值
        //     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, currentCount + Motor4.pulse);
        // }
    }
}

