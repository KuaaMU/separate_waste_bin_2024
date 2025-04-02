#include "app_servo_motor.h"
#include "servo_motor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
#include "main.h"
#include "stdbool.h"
#include "sonar_SR04.h"
#include "UART_DMA.h"
#include "app_visual_identity.h"
#include "app_sonar.h"
#include "app_OPTO_switch.h"


//舵机转动延迟时间
#define SERVO_DELAY_US 600

// 舵机句柄实例
Servo_HandleTypeDef Servo1;//云台底部舵机
Servo_HandleTypeDef Servo2;//云台上部舵机
Servo_HandleTypeDef Servo3;//云台上部舵机
Servo_HandleTypeDef Servo4;//云台底部舵机

int cloud_terrace_state;


//外部变量
extern xTaskHandle ServoControlTaskHandle;
extern xTaskHandle StepMotorTaskHandle;
extern xTaskHandle DCMotorTaskHandle;

extern bool rubbish_is_detected; // 垃圾检测标志
extern int rubbish_type;//实时检测到的垃圾类型
extern int current_rubbish_type;//检测稳定后的垃圾类型

extern int state1;
//垃圾总量
extern int rubbish_total_num ;
//1可回收垃圾数量
extern int rubbish_recoverable_num ;
//2其他垃圾数量
extern int rubbish_other_num ;
//3厨余垃圾数量
extern int rubbish_kitchen_num ;
//4有害垃圾数量
extern int rubbish_harmful_num ;
// 检测状态结构体
extern DetectionStability ds1;
extern DetectionStability ds2;



void ServoControlTask(void* argument)
{
    App_Servo_Reset();

    for( ;; )
	{
        // App_Servo_Reset();
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        // vTaskSuspendAll();

        App_Servo_Proc();


        // xTaskResumeAll();
	}
}

// 初始化舵机
void App_Servo_Init(void)
{
    // // 初始化舵机1（使用TIM8通道1）PC6
    // Servo_Init(&Servo1, &htim8, TIM_CHANNEL_1, 500, 2500, 1500);

    // // 初始化舵机2（使用TIM8通道2）PC7
    // Servo_Init(&Servo2, &htim8, TIM_CHANNEL_2, 500, 2500, 1500);

    // 初始化舵机3（使用TIM8通道3）PC8
    Servo_Init(&Servo3, &htim8, TIM_CHANNEL_3, 500, 2500, 1500);

    // 初始化舵机4（使用TIM8通道4）PC9
    Servo_Init(&Servo4, &htim8, TIM_CHANNEL_4, 500, 2500, 1500);
}

// 舵机控制程序
void App_Servo_Proc(void)
{
    cloud_terrace_state=current_rubbish_type;
    current_rubbish_type= RUBBISH_TYPE_NONE;


    if (cloud_terrace_state != RUBBISH_TYPE_NONE )
    {
        HAL_UART_DMAStop(&USB_Huart_2);

        // 云台转动
        App_cloud_terrace_state(cloud_terrace_state);
        RubbishWav(cloud_terrace_state);
        // 更新垃圾计数
        updateRubbishCount(cloud_terrace_state);
        //例行归位
        App_Servo_Reset();

        resetStability(&ds1);
        resetStability(&ds2);

        StartUartRxDMA(&USB_Huart_2);
        //通知电机任务继续工作
        if(state1==1)
        {
            rubbish_is_detected=false;
            xTaskNotifyGive(StepMotorTaskHandle);

        }
    }
    cloud_terrace_state= RUBBISH_TYPE_NONE;

}

// 设置舵机角度
void App_Servo_SetAngle(Servo_HandleTypeDef *hServo, uint16_t angle)
{
    Servo_SetAngle(hServo, angle);
}

//云台回正
void App_Servo_Reset(void)
{
    // Servo_SetAngleCustomRange(&Servo1, 0,0,180);
    // Servo_SetAngleCustomRange(&Servo2, 135,0,270);


    Servo_SetAngleCustomRange(&Servo3, 132,0,270);
    vTaskDelay(pdMS_TO_TICKS(500));
    Servo_SetAngleCustomRange(&Servo4, 0,0,180);
    vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
}

//设置云台转动情况
void App_cloud_terrace_state(int state)
{
        // 云台转动
        switch (state)
        {
        case RUBBISH_TYPE_RECOVERABLE:
            // Servo_SetAngleCustomRange(&Servo1, 0,0,180);
            // Servo_SetAngleCustomRange(&Servo2, 135,0,270);
            xTaskNotifyGive(DCMotorTaskHandle);
            Servo_SetAngleCustomRange(&Servo4, 0,0,180);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            // Servo_SetAngleCustomRange(&Servo2, 210,0,270);

            Servo_SetAngleCustomRange(&Servo3, 210,0,270);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            break;
        case RUBBISH_TYPE_OTHER:
            // Servo_SetAngleCustomRange(&Servo1, 90,0,180);
            // Servo_SetAngleCustomRange(&Servo2, 135,0,270);

            Servo_SetAngleCustomRange(&Servo4, 90,0,180);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            // Servo_SetAngleCustomRange(&Servo2, 60,0,270);

            Servo_SetAngleCustomRange(&Servo3, 60,0,270);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            break;
        case RUBBISH_TYPE_KITCHEN:
            //1型
            // Servo_SetAngleCustomRange(&Servo1, 0,0,180);
            // Servo_SetAngleCustomRange(&Servo2, 135,0,270);
            //2型

            Servo_SetAngleCustomRange(&Servo4, 0,0,180);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            // Servo_SetAngleCustomRange(&Servo2, 60,0,270);
            //2型
            Servo_SetAngleCustomRange(&Servo3, 60,0,270);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            break;
        case RUBBISH_TYPE_HARMFUL:
            // Servo_SetAngleCustomRange(&Servo1, 90,0,180);
            // Servo_SetAngleCustomRange(&Servo2, 135,0,270);

            Servo_SetAngleCustomRange(&Servo4, 90,0,180);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            // Servo_SetAngleCustomRange(&Servo2, 210,0,270);

            Servo_SetAngleCustomRange(&Servo3, 210,0,270);
            vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_US));
            break;
        }

}

void APP_cloud_vibrating(void)
{
    Servo_SetAngleCustomRange(&Servo3, 128,0,270);
    vTaskDelay(pdMS_TO_TICKS(50));
    Servo_SetAngleCustomRange(&Servo3, 136,0,270);
    vTaskDelay(pdMS_TO_TICKS(50));
    Servo_SetAngleCustomRange(&Servo4, 20,0,180);
    vTaskDelay(pdMS_TO_TICKS(50));
    Servo_SetAngleCustomRange(&Servo4, 0,0,180);
}
