/*
 * UART_DMA.c
 *
 *  Created on: Mar 14, 2021
 *      Author: Royic
 */
#include "UART_DMA.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "app_visual_identity.h"
#include "app_OPTO_switch.h"
#include "app_stepper_motor.h"

extern xTaskHandle ServoControlTaskHandle;
extern xTaskHandle StepMotorTaskHandle;

uint8_t RxBuffer[UART_RX_BUF_SIZE] = {0};
uint8_t TxBuffer[UART_RX_BUF_SIZE] = {0};
uint8_t sendCompleteSign = 1;
uint8_t TxLen = 0;
uint8_t USE_UART_DMA = 0;

extern int state1;//开关
extern int rubbish_type;//垃圾类型
extern int current_rubbish_type;//稳定后当前垃圾类型

void DataProcess(UART_HandleTypeDef *huart, uint32_t Len)
{
	//在这里加入数据处理的函数
#ifdef USB_Huart_1
	if(huart == &USB_Huart_1)
    {
        // 检查数据包格式
        if(RxBuffer[0] == 0xb1 && RxBuffer[1] == 0xb1 && RxBuffer[3] == 0x55)
        {
            // 提取rubbish_type
            rubbish_type = RxBuffer[2];
			printf("rubbish=%d\r\n",(int)RxBuffer[2]);
            // 可以在这里添加更多的数据处理逻辑
        }
        else
        {
            // 数据包格式不正确，可以添加错误处理逻辑
        };
    }
#endif
#ifdef USB_Huart_2
	if(huart == &USB_Huart_2)
	{
		// printf("kkkkkkk=%0x %0x %0x %0x %0x \r\n",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3],RxBuffer[4]);
		if(RxBuffer[0] == 0x55 && RxBuffer[2] == 0xff && RxBuffer[3] == 0xff && RxBuffer[4] == 0xff)
		{
			if(RxBuffer[1]== 0x01)
			{
				state1=1;
				vTaskNotifyGiveFromISR( StepMotorTaskHandle, NULL);
			}
			else if(RxBuffer[1]== 0x00)
			{
				state1=0;
				vTaskNotifyGiveFromISR( StepMotorTaskHandle, NULL);

			}
		}
	}
#endif
#ifdef USB_Huart_3
	if(huart == &USB_Huart_3)
	{
		// printf("kkkkkkk=%0x %0x %0x %0x %0x \r\n",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3],RxBuffer[4]);

	}
#endif
}

//到USARTx_IRQHandler中添加，如:
//void USART1_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART1_IRQn 0 */
//  if(__HAL_UART_GET_FLAG(&USB_Huart_1,UART_FLAG_IDLE))
//  {
//	  HAL_UART_IdleCallback(&USB_Huart_1);
//  }
//
//  /* USER CODE END USART1_IRQn 0 */
//  HAL_UART_IRQHandler(&huartx);
//}
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_IDLEFLAG(huart);
	{
		HAL_UART_DMAStop(huart);

		ProcessData(huart);

        StartUartRxDMA(huart);


	}
}

void ProcessData(UART_HandleTypeDef *huart)
{
    uint32_t len = 0;

	len = UART_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);

    if(len > 0)
    {
        if(sendCompleteSign == 1)
        {
#if UART_RXTX_Switch
            memset((void *)TxBuffer, 0, sizeof(TxBuffer));
            memcpy(TxBuffer, RxBuffer, len);
            TxLen = len;
            StartUartTxDMA(huart);	//串口回显
#endif
        }
        {
        	//在这里面加入数据处理的函数
        	DataProcess(huart, len);
        }
    }
}

void USB_DMA_printf(UART_HandleTypeDef *huart, const char *format,...)
{
	uint32_t length;
	va_list args;

	va_start(args, format);
	length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
	va_end(args);

	HAL_UART_Transmit_DMA(huart,TxBuffer,length);
}

void USB_printf(UART_HandleTypeDef *huart, const char *format,...)
{
	uint32_t length;
	va_list args;

	va_start(args, format);
	length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
	va_end(args);

	HAL_UART_Transmit(huart,TxBuffer,length,0xFFFF);
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
	sendCompleteSign = 1;
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
	ProcessData(huart);
	StartUartRxDMA(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}

uint8_t UartTxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
{
	HAL_StatusTypeDef status;
	uint8_t ret = 1;

	if(sendCompleteSign == 0 || len == 0)
	{
		return 0;
	}

    sendCompleteSign = 0;

	status = HAL_UART_Transmit_DMA(huart, (uint8_t*)buf, len);

	if(HAL_OK != status)
	{
		ret = 0;
	}

	return ret;
}

//启动DMA发送
uint8_t StartUartTxDMA(UART_HandleTypeDef *huart)
{
    return UartTxData(huart, TxBuffer, TxLen);
}

uint8_t UartRxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
{
	HAL_StatusTypeDef status;
	uint8_t ret = 1;

	status = HAL_UART_Receive_DMA(huart, (uint8_t*)buf, len);
	if(HAL_OK != status)
	{
		ret = 0;
	}
	else
	{
		/* 开启空闲接收中断 */
	    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}

	return ret;
}

//启动DMA接收
uint8_t StartUartRxDMA(UART_HandleTypeDef *huart)
{
	USE_UART_DMA = 1;
    return UartRxData(huart, RxBuffer, UART_RX_BUF_SIZE);
}
