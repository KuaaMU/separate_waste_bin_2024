#include "app_visual_identity.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "stdbool.h"
#include "OPTO_switch.h"
#include <stdio.h>
#include "string.h"
#include "UART_DMA.h"

// 垃圾总量
int rubbish_total_num = 0;
// 1可回收垃圾数量
int rubbish_recoverable_num = 0;
// 2其他垃圾数量
int rubbish_other_num = 0;
// 3厨余垃圾数量
int rubbish_kitchen_num = 0;
// 4有害垃圾数量
int rubbish_harmful_num = 0;


// 检测状态
bool rubbish_is_detected = false;
// 垃圾类型
int current_rubbish_type = RUBBISH_TYPE_NONE; // 稳定后
int rubbish_type = RUBBISH_TYPE_NONE;         // 实时检测
// 垃圾类型数据
uint8_t harmful[5] = {0xAA, 0x55, (uint8_t)0x01, 0x55, 0xAA};
uint8_t other[5] = {0xAA, 0x55, (uint8_t)0x02, 0x55, 0xAA};
uint8_t recover[5] = {0xAA, 0x55, (uint8_t)0x03, 0x55, 0xAA};
uint8_t kitchen[5] = {0xAA, 0x55, (uint8_t)0x04, 0x55, 0xAA};

// 检测稳定性结构体实例
DetectionStability ds1;
DetectionStability ds2;

// 外部变量声明
extern int state1;
extern int state2;
extern int infrared_state1; // 红外传感器状态
extern int reflective_state1;
extern int reflective_state2;
extern int reflective_state3;
extern int reflective_state4;
extern xTaskHandle ServoControlTaskHandle;
extern xTaskHandle StepMotorTaskHandle;

// 垃圾总量
extern int rubbish_total_num;
// 1可回收垃圾数量
extern int rubbish_recoverable_num;
// 2其他垃圾数量
extern int rubbish_other_num;
// 3厨余垃圾数量
extern int rubbish_kitchen_num;
// 4有害垃圾数量
extern int rubbish_harmful_num;

// 检测状态
extern bool rubbish_is_detected;
// 垃圾类型
extern int current_rubbish_type; // 稳定后
extern int rubbish_type;         // 实时检测


// 初始化检测稳定性结构体
void initDetectionStability(DetectionStability *ds, int stabilityThreshold) {
    for (int i = 0; i < WINDOW_SIZE_VS; i++) {
        ds->window[i] = 0; // 初始化为0，表示未检测到
    }
    ds->windowIndex = 0;
    ds->stabilityThreshold = stabilityThreshold;
}

// 检测稳定性函数
int checkStability(DetectionStability *ds, int currentValue) {
    // 更新窗口
    ds->window[ds->windowIndex] = currentValue;
    ds->windowIndex = (ds->windowIndex + 1) % WINDOW_SIZE_VS;

    // 检查窗口中的值是否达到稳定性阈值
    int count = 0;
    for (int i = 0; i < WINDOW_SIZE_VS; i++) {
        if (ds->window[i] == currentValue) {
            count++;
        }
    }

    if (count >= ds->stabilityThreshold) {
        return 1; // 检测稳定
    } else {
        return 0; // 检测不稳定
    }
}

// 复位稳定性检测结构体
void resetStability(DetectionStability *ds) {
    for (int i = 0; i < WINDOW_SIZE_VS; i++) {
        ds->window[i] = 0; // 初始化为0，表示未检测到
    }
    ds->windowIndex = 0;
    ds->stabilityThreshold = STABILITY_THRESHOLD_1; // 重置稳定性阈值
}

// 初始化函数
void App_Visual_Identity_Init(void) {
    initDetectionStability(&ds1, STABILITY_THRESHOLD_1); // 设置稳定性阈值
    initDetectionStability(&ds2, STABILITY_THRESHOLD_2); // 设置稳定性阈值
}

// 任务函数
void VisualIdentityReadTask(void *argument) {
    vTaskDelay(pdMS_TO_TICKS(10000));
    StartUartRxDMA(&USB_Huart_1);
    USB_DMA_printf(&USB_Huart_2, "rubbish_total_num=%d\xff\xff\xff", rubbish_total_num);
    USB_DMA_printf(&USB_Huart_2, "rubbish_recoverable_num=%d\xff\xff\xff", rubbish_recoverable_num);
    USB_DMA_printf(&USB_Huart_2, "rubbish_other_num=%d\xff\xff\xff", rubbish_other_num);
    USB_DMA_printf(&USB_Huart_2, "rubbish_kitchen_num=%d\xff\xff\xff", rubbish_kitchen_num);
    USB_DMA_printf(&USB_Huart_2, "rubbish_harmful_num=%d\xff\xff\xff", rubbish_harmful_num);

    for (;;) {
        // 检测垃圾
        App_Visual_Identity_Proc();
        // 检测到垃圾，打开改变云台位置
        vTaskDelay(pdMS_TO_TICKS(DETECTION_INTERVAL)); // 假设每200ms检测一次
    }
}

// 检测垃圾函数
void App_Visual_Identity_Proc(void) {
    static uint32_t timeout_cnt = 0;
    // 检查稳定性
    if (checkStability(&ds1, rubbish_type)) {
        current_rubbish_type = rubbish_type;
        printf("---current_rubbish_type=%d\r\n", current_rubbish_type);

        if (current_rubbish_type != RUBBISH_TYPE_NONE) {
            ds1.stabilityThreshold = STABILITY_THRESHOLD_1_EXTRA;
            xTaskNotifyGive(ServoControlTaskHandle);
        } else {
            ds1.stabilityThreshold = STABILITY_THRESHOLD_1;
        }

        USB_DMA_printf(&USB_Huart_2, "rubbish_state0=%d\xff\xff\xff", current_rubbish_type);

        timeout_cnt = 0;
    }

    // 通知电机启动的判断逻辑
    if (checkStability(&ds2, rubbish_type)) {
        if (rubbish_type != RUBBISH_TYPE_NONE) {
            rubbish_is_detected = true;
            xTaskNotifyGive(StepMotorTaskHandle);
        } else {
            rubbish_is_detected = false;
            xTaskNotifyGive(StepMotorTaskHandle);
        }
    }

    // 如果没有检测到垃圾，并且state1为1，则检查是否超时
    if (!rubbish_is_detected && state1 == 1) {
        timeout_cnt++;
        if (timeout_cnt > TIMEOUT_THRESHOLD) {
            state1 = 0;
            timeout_cnt = 0;
        }
    }
}

// 更新垃圾计数函数
void updateRubbishCount(int type) {
    rubbish_total_num++;
    USB_DMA_printf(&USB_Huart_2, "rubbish_state0=%d\xff\xff\xff", current_rubbish_type);
    vTaskDelay(pdMS_TO_TICKS(50));
    USB_DMA_printf(&USB_Huart_2, "rubbish_total_num=%d\xff\xff\xff", rubbish_total_num);
    vTaskDelay(pdMS_TO_TICKS(50));
    switch (type) {
        case RUBBISH_TYPE_RECOVERABLE:
            rubbish_recoverable_num++;
            USB_DMA_printf(&USB_Huart_2, "rubbish_recoverable_num=%d\xff\xff\xff", rubbish_recoverable_num);
            vTaskDelay(pdMS_TO_TICKS(50));
            break;
        case RUBBISH_TYPE_OTHER:
            rubbish_other_num++;
            USB_DMA_printf(&USB_Huart_2, "rubbish_other_num=%d\xff\xff\xff", rubbish_other_num);
            vTaskDelay(pdMS_TO_TICKS(50));
            break;
        case RUBBISH_TYPE_KITCHEN:
            rubbish_kitchen_num++;
            USB_DMA_printf(&USB_Huart_2, "rubbish_kitchen_num=%d\xff\xff\xff", rubbish_kitchen_num);
            vTaskDelay(pdMS_TO_TICKS(50));
            break;
        case RUBBISH_TYPE_HARMFUL:
            rubbish_harmful_num++;
            USB_DMA_printf(&USB_Huart_2, "rubbish_harmful_num=%d\xff\xff\xff", rubbish_harmful_num);
            vTaskDelay(pdMS_TO_TICKS(50));
            break;
        default:
            break;
    }
}

// 发送垃圾类型声音函数
void RubbishWav(int type) {
    switch (type) {
        case RUBBISH_TYPE_RECOVERABLE:
            HAL_UART_Transmit(&huart2, recover, 5, 1000);
            break;
        case RUBBISH_TYPE_OTHER:
            HAL_UART_Transmit(&huart2, other, 5, 1000);
            break;
        case RUBBISH_TYPE_KITCHEN:
            HAL_UART_Transmit(&huart2, kitchen, 5, 1000);
            break;
        case RUBBISH_TYPE_HARMFUL:
            HAL_UART_Transmit(&huart2, harmful, 5, 1000);
            break;
        default:
            break;
    }
}
