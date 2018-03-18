/*
 * uart_hal.c
 *
 *  Created on: 2018年2月3日
 *      Author: Zero
 */

/** @brief	UART interrupt function 模板 移植至stm32fxxx_it.c
  *
  *		void USART2_IRQHandler(void)
  *		{
  * 	#if UART_DMA
  * 		UART_DMARec_Callback(&huart2);
  * 	#else
  *			UART_TIRec_Callback(&huart2);
  * 	#endif
  *			HAL_UART_IRQHandler(&huart2);
  * 	}
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "uart_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

extern UART_HandleTypeDef huart2;
UartRecType Uart2;

#if UART_DMA

void UART_DMARec_Callback(UART_HandleTypeDef *huart)
{
	u32 temp;
	if(huart->Instance == USART2)
	{
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			HAL_UART_DMAStop(huart);
			temp = huart->hdmarx->Instance->CNDTR;
			Uart2.RecSize =  REC_SIZE - temp;
			Uart2.TimeCounter = 0;
			while(HAL_UART_Receive_DMA(huart, Uart2.RecBuff, REC_SIZE) != HAL_OK)
			{
				if(Uart2.TimeCounter++ > UART_MAX_TIME)
				{
					Uart2.UartFlg = true;
					break;
				}
			}
			Uart2.RecIntrFlg = true;
		}
	}
}

#else

void UART_TIRec_Callback(UART_HandleTypeDef *huart)
{
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		Uart2.RecIntrFlg = true;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		Uart2.RecBuff[Uart2.RecSize++] = Uart2.RecByte[0];
		Uart2.TimeCounter = 0;
		while(HAL_UART_Receive_IT(&huart2, Uart2.RecByte, 1) != HAL_OK)
		{
			if(Uart2.TimeCounter++ > UART_MAX_TIME)
			{
				Uart2.UartFlg = true;
				break;
			}
		}
	}
}

#endif

void Set_Uart(UART_HandleTypeDef *huart, u32 BaudRate, u8 WordLength, u8 StopBits, u8 Parity)
{
	huart->Init.BaudRate = BaudRate;
	switch(WordLength)
	{
		case 8: huart->Init.WordLength = UART_WORDLENGTH_8B; break;
		case 9:	huart->Init.WordLength = UART_WORDLENGTH_9B; break;
		default:break;
	}

	switch(StopBits)
	{
		case 0: huart->Init.StopBits = UART_STOPBITS_1; break;
		case 2: huart->Init.StopBits = UART_STOPBITS_2; break;
		default:break;
	}

	switch(Parity)
	{
		case 0: huart->Init.Parity = UART_PARITY_NONE; break;
		case 1: huart->Init.Parity = UART_PARITY_ODD; break;
		case 2: huart->Init.Parity = UART_PARITY_EVEN; break;
		default:break;
	}

	if (HAL_UART_Init(huart) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart2, Uart2.RecByte, 1);
}

u32 Get_Baudrate(UART_HandleTypeDef *huart)
{
	u32 baud_rate = 0;
	baud_rate = huart->Init.BaudRate;
	return baud_rate;
}

u8 Get_Word_Length(UART_HandleTypeDef *huart)
{
	u8 word_length = 0;
	switch(huart->Init.WordLength)
	{
		case UART_WORDLENGTH_8B: word_length = 8; break;
		case UART_WORDLENGTH_9B: word_length = 9; break;
		default:break;
	}
	return word_length;
}

u8 Get_Stop_Bits(UART_HandleTypeDef *huart)
{
	u8 stop_bits = 0;
	switch(huart->Init.StopBits)
	{
		case UART_STOPBITS_1: stop_bits = 0; break;
		case UART_STOPBITS_2: stop_bits = 2; break;
		default:break;
	}
	return stop_bits;
}

u8 Get_Parity(UART_HandleTypeDef *huart)
{
	u8 parity = 0;
	switch(huart->Init.Parity)
	{
		case UART_PARITY_NONE: parity = 0; break;
		case UART_PARITY_ODD: parity = 1; break;
		case UART_PARITY_EVEN: parity = 2; break;
		default:break;
	}
	return parity;
}

u16 Uart_Send(UART_HandleTypeDef *huart, u8 *buff, u16 size)
{
	if(HAL_UART_Transmit(huart, buff, size, 0xffff) != HAL_OK)
	{
		return false;
	}
	return size;
}

void Uart_Proc()
{
	if(Uart2.RecIntrFlg)
	{
		Uart2.RecIntrFlg = false;
		HAL_UART_Transmit(&huart2, Uart2.RecBuff, Uart2.RecSize, 0xffff);
		CDC_Transmit_FS((u8*)&Uart2.RecBuff, Uart2.RecSize);
		Uart2.RecSize = 0;
	}
	if(Uart2.UartFlg)
	{
		Uart2.TimeCounter = 0;
		Uart2.UartFlg = false;
#if UART_DMA
		while(HAL_UART_Receive_DMA(&huart2, Uart2.RecBuff, REC_SIZE) != HAL_OK)
#else
		while(HAL_UART_Receive_IT(&huart2, Uart2.RecByte, 1) != HAL_OK)
#endif
		{
			if(Uart2.TimeCounter++ > UART_MAX_TIME)
			{
				Uart2.UartFlg = true;
				break;
			}
		}
	}
}
