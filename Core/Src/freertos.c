/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "event_groups.h"
#include "queue.h"
#include "usart.h"
#include "app_stepper_motor.h"
#include "app_servo_motor.h"
#include "app_sonar.h"
#include "app_OPTO_switch.h"
#include "app_visual_identity.h"
#include "DCmotor.h"
#include "..\..\Drivers\CMSIS\RTOS\Template\cmsis_os.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
xTaskHandle SensorTaskHandle;
xTaskHandle ControlTaskHandle;
xTaskHandle ServoControlTaskHandle;
xTaskHandle StepMotorTaskHandle;
xTaskHandle DCMotorTaskHandle;
extern DCMotor_HandleTypeDef DCmotor1;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void StartSensorTask(void* argument);
void StartControlTask(void* argument);
void DCMotorControlTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  BaseType_t xReturn = pdPASS;
  printf("DefaultTask Start\r\n");

  taskENTER_CRITICAL(); // 进入临界�???????

  // 创建任务
  xReturn = xTaskCreate(StartSensorTask,
                        "Start_Sensor_Task",
                        128,
                        NULL,
                        10,
                        &SensorTaskHandle);
  if (xReturn == pdPASS)
    printf("Start_Sensor_Task任务创建成功\r\n");


  xReturn = xTaskCreate(StartControlTask,
                        "Start_Control_Task",
                        128,
                        NULL,
                        10,
                        &ControlTaskHandle);
  if (xReturn == pdPASS)
    printf("Start_Control_Task任务创建成功\r\n");

  vTaskDelete(defaultTaskHandle); // 删除�???????始任�???????
  taskEXIT_CRITICAL();            // �???????出临界区
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

//传感器任务进程�?��?�创�???????
void StartSensorTask(void* argument)
{
    BaseType_t xReturn = pdPASS;
    printf("SensorTask Start\r\n");

    taskENTER_CRITICAL(); // 进入临界�???????
    xReturn = xTaskCreate(SonarReadTask,
                        "Sonar_Read_Task",
                        256,
                        NULL,
                        5,
                        NULL);
  if (xReturn == pdPASS)
    printf("Sonar_Read_Task任务创建成功\r\n");

    xReturn = xTaskCreate(OPTOswitchReadTask,
                        "OPTOswitch_Read_Task",
                        256,
                        NULL,
                        5,
                        NULL);
  if (xReturn == pdPASS)
    printf("OPTOswitch_Read_Task任务创建成功\r\n");

  xReturn = xTaskCreate(VisualIdentityReadTask,
                        "VisualIdentity_Read_Task",
                        256,
                        NULL,
                        5,
                        NULL);
  if (xReturn == pdPASS)
    printf("VisualIdentity_Read_Task任务创建成功\r\n");


  vTaskDelete(NULL); // 删除�???????始任�???????
  taskEXIT_CRITICAL();            // �???????出临界区
}


//控制任务进程—�?�创�???????
void StartControlTask(void* argument)
{
    BaseType_t xReturn = pdPASS;
    printf("ControlTask Start\r\n");

    taskENTER_CRITICAL(); // 进入临界�???????
    xReturn = xTaskCreate(ServoControlTask,
                        "Servo_Control_Task",
                        256,
                        NULL,
                        7,
                        &ServoControlTaskHandle);
    if (xReturn == pdPASS)
      printf("Servo_Control_Task任务创建成功\r\n");

    xReturn = xTaskCreate(StepMotorControlTask,
                          "StepMotor_Control_Task",
                          256,
                          NULL,
                          6,
                          &StepMotorTaskHandle);
    if (xReturn == pdPASS)
      printf("StepMotor_Control_Task任务创建成功\r\n");

    xReturn = xTaskCreate(DCMotorControlTask,
                          "DCMotor_Control_Task",
                          256,
                          NULL,
                          6,
                          &DCMotorTaskHandle);
    if (xReturn == pdPASS)
      printf("DCMotor_Control_Task任务创建成功\r\n");

    vTaskDelete(NULL); // 删除�???????始任�???????
    taskEXIT_CRITICAL();// �???????出临界区
}

void DCMotorControlTask(void const * argument)
{
  APP_DCmotor_Start(&DCmotor1,DCMOTOR_DIR_L);
  while (1)
  {
    ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
    App_DCmotor_Proc();
  }
}


// void DiffuseReadTask(void const * argument)
// {
//   App_Diffuse_Test();
// }



/* USER CODE END Application */
