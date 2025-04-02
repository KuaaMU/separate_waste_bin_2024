#include "app_sonar.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "sonar_SR04.h"
#include "UART_DMA.h"
#include "app_visual_identity.h"
#define WINDOW_SIZE 3 // 定义窗口大小，可以根据需要调整

float sonar1_values[WINDOW_SIZE] = {12};
int sonar1_index = 0;

float sonar2_values[WINDOW_SIZE] = {12};
int sonar2_index = 0;

float sonar3_values[WINDOW_SIZE] = {12};
int sonar3_index = 0;

float sonar4_values[WINDOW_SIZE] = {12};
int sonar4_index = 0;


#define SONAR_1_MAX         28
#define SONAR_1_MIN         12.5

#define SONAR_2_MAX         16
#define SONAR_2_MIN         12

#define SONAR_3_MAX         27
#define SONAR_3_MIN         12.17

#define SONAR_4_MAX         30
#define SONAR_4_MIN         10

#define FULL_DEPTH_PERCENT  25

// 定义超声波传感器的句柄
extern Sonar_HandleTypeDef Sonar1; //厨余
extern Sonar_HandleTypeDef Sonar2; //其他
extern Sonar_HandleTypeDef Sonar3; //可回收
extern Sonar_HandleTypeDef Sonar4; //有害

extern xTaskHandle ServoControlTaskHandle;
extern xTaskHandle StepMotorTaskHandle;

uint8_t harmful_F[5] = {0xAA, 0x55, (uint8_t)5, 0x55, 0xAA};
uint8_t other_F[5] = {0xAA, 0x55, (uint8_t)6, 0x55, 0xAA};
uint8_t recover_F[5] = {0xAA, 0x55, (uint8_t)7, 0x55, 0xAA};
uint8_t kitchen_F[5] = {0xAA, 0x55, (uint8_t)8, 0x55, 0xAA};

int Sonar1_full_state = 0;
int Sonar2_full_state = 0;
int Sonar3_full_state = 0;
int Sonar4_full_state = 0;
int full_state = 0;

void App_Sonar_Init(void)
{

}

void SonarReadTask(void const * argument)
{
    vTaskDelay(pdMS_TO_TICKS(10000));
    for(;;)
    {
        // App_Sonar_Test();
        App_Sonar_Proc();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void App_Sonar_Test(void)
{
    Sonar_GetDistance(&Sonar1);
    printf("Sonar1 distance: %.2f cm\r\n",Sonar1.distance);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void App_Sonar_Proc(void)
{
    Sonar1.distance = movingAverage(sonar1_values, sonar1_index, Sonar_GetDistance(&Sonar1));
    sonar1_index = (sonar1_index + 1) % WINDOW_SIZE;
    vTaskDelay(pdMS_TO_TICKS(100));

    Sonar2.distance = movingAverage(sonar2_values, sonar2_index, Sonar_GetDistance(&Sonar2));
    sonar2_index = (sonar2_index + 1) % WINDOW_SIZE;
    vTaskDelay(pdMS_TO_TICKS(100));

    Sonar3.distance = movingAverage(sonar3_values, sonar3_index, Sonar_GetDistance(&Sonar3));
    sonar3_index = (sonar3_index + 1) % WINDOW_SIZE;
    vTaskDelay(pdMS_TO_TICKS(100));

    Sonar4.distance = movingAverage(sonar4_values, sonar4_index, Sonar_GetDistance(&Sonar4));
    sonar4_index = (sonar4_index + 1) % WINDOW_SIZE;
    vTaskDelay(pdMS_TO_TICKS(100));

    // 计算百分比
    Sonar1.percent = mapToRange(Sonar1.distance, SONAR_1_MIN, SONAR_1_MAX, 0, 100);
    Sonar2.percent = mapToRange(Sonar2.distance, SONAR_2_MIN, SONAR_2_MAX, 0, 100);
    Sonar3.percent = mapToRange(Sonar3.distance, SONAR_3_MIN, SONAR_3_MAX, 0, 100);
    Sonar4.percent = mapToRange(Sonar4.distance, SONAR_4_MIN, SONAR_4_MAX, 0, 100);

    //向调试串口发送读取到的距离数据
    printf("--------------DISTANCE-----------------\r\n");
    printf("-Sonar1 distance: %.2f cm\r\n",Sonar1.distance);
    printf("-Sonar2 distance: %.2f cm\r\n",Sonar2.distance);
    printf("-Sonar3 distance: %.2f cm\r\n",Sonar3.distance);
    printf("-Sonar4 distance: %.2f cm\r\n",Sonar4.distance);
    printf("--------------PERCENT-----------------\r\n");
    printf("Sonar1 : %d \r\n",Sonar1.percent);
    printf("Sonar2 : %d \r\n",Sonar2.percent);
    printf("Sonar3 : %d \r\n",Sonar3.percent);
    printf("Sonar4 : %d \r\n",Sonar4.percent);

    //向串口屏发送垃圾桶满仓状态数据
    USB_DMA_printf(&USB_Huart_2, "rubbish_recoverable_storage=%d\xff\xff\xff",100-Sonar3.percent);
    vTaskDelay(pdMS_TO_TICKS(50));
    USB_DMA_printf(&USB_Huart_2, "rubbish_other_storage=%d\xff\xff\xff",100-Sonar2.percent);
    vTaskDelay(pdMS_TO_TICKS(50));
    USB_DMA_printf(&USB_Huart_2, "rubbish_kitchen_storage=%d\xff\xff\xff",100-Sonar1.percent);
    vTaskDelay(pdMS_TO_TICKS(50));
    USB_DMA_printf(&USB_Huart_2, "rubbish_harmful_storage=%d\xff\xff\xff",100-Sonar4.percent);

    if(Sonar1.percent < FULL_DEPTH_PERCENT)
    {
        RubbishWav_F(RUBBISH_TYPE_KITCHEN);
    }

    if(Sonar2.percent < FULL_DEPTH_PERCENT)
    {
        RubbishWav_F(RUBBISH_TYPE_OTHER);
    }

    if(Sonar3.percent < FULL_DEPTH_PERCENT)
    {
        RubbishWav_F(RUBBISH_TYPE_RECOVERABLE);
    }

    if(Sonar4.percent < FULL_DEPTH_PERCENT)
    {
        RubbishWav_F(RUBBISH_TYPE_HARMFUL);
    }
}

int mapToRange(float value, float inputMin, float inputMax, int outputMin, int outputMax) {
    if (value < inputMin) {
        value = inputMin;
    } else if (value > inputMax) {
        value = inputMax;
    }
    // 计算outputMin到outputMax的映射
    return (int)(((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) + outputMin);
}

float movingAverage(float values[], int index, float newValue) {
    values[index] = newValue;
    float sum = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        sum += values[i];
    }
    return sum / WINDOW_SIZE;
}

void RubbishWav_F(int type)
{

    switch (type)
    {
    case RUBBISH_TYPE_RECOVERABLE:
        HAL_UART_Transmit(&huart2, recover_F, 5, 1000);
        break;
    case RUBBISH_TYPE_OTHER:
        HAL_UART_Transmit(&huart2, other_F, 5, 1000);
        break;
    case RUBBISH_TYPE_KITCHEN:
        HAL_UART_Transmit(&huart2, kitchen_F, 5, 1000);
        break;
    case RUBBISH_TYPE_HARMFUL:
        HAL_UART_Transmit(&huart2, harmful_F, 5, 1000);
        break;
    default:
        break;
    }
}
