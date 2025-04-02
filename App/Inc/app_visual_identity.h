#ifndef APP_VISUAL_IDENTITY_H
#define APP_VISUAL_IDENTITY_H

#include <stdint.h>


// 垃圾类型定义
#define RUBBISH_TYPE_NONE 0
#define RUBBISH_TYPE_RECOVERABLE 3
#define RUBBISH_TYPE_OTHER 2
#define RUBBISH_TYPE_KITCHEN 4
#define RUBBISH_TYPE_HARMFUL 1

// 检测超时时间阈值
#define TIMEOUT_THRESHOLD 15000
// 检测时间间隔
#define DETECTION_INTERVAL 100
// 稳定性阈值-舵机
#define STABILITY_THRESHOLD_1 5
// 稳定性阈值-舵机-额外
#define STABILITY_THRESHOLD_1_EXTRA 6
// 稳定性阈值-电机
#define STABILITY_THRESHOLD_2 1
// 窗口大小
#define WINDOW_SIZE_VS 10

// 定义检测稳定性结构体
typedef struct {
    int window[WINDOW_SIZE_VS];
    int windowIndex;
    int stabilityThreshold;
} DetectionStability;

// 初始化函数
void App_Visual_Identity_Init(void);

// 任务函数
void VisualIdentityReadTask(void *argument);

// 检测垃圾函数
void App_Visual_Identity_Proc(void);

// 更新垃圾计数函数
void updateRubbishCount(int type);

// 发送垃圾类型声音函数
void RubbishWav(int type);

// 函数声明
void initDetectionStability(DetectionStability *ds, int stabilityThreshold);
int checkStability(DetectionStability *ds, int currentValue);
void resetStability(DetectionStability *ds);

#endif // APP_VISUAL_IDENTITY_H
