/*
 * uart_hal.h
 *
 *  Created on: 2018年2月3日
 *      Author: Zero
 */

#ifndef STM32F1XX_HAL_DRIVER_INC_UART_HAL_H_
#define STM32F1XX_HAL_DRIVER_INC_UART_HAL_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "data_type.h"

#define UART_DMA	0
#define	REC_SIZE	512
#define UART_MAX_TIME	0xFFFE

typedef struct
{
#if !UART_DMA
	u8		RecByte[1];
#endif
	u8		RecBuff[REC_SIZE];
	u16		TimeCounter;
	u16		RecSize;
	bool	RecIntrFlg;
	bool	UartFlg;
}UartRecType;

#if UART_DMA
	void UART_DMARec_Callback(UART_HandleTypeDef *huart);
#else
	void UART_TIRec_Callback(UART_HandleTypeDef *huart);
#endif

void Set_Uart(UART_HandleTypeDef *huart, u32 BaudRate, u8 WordLength, u8 StopBits, u8 Parity);
u16	 Uart_Send(UART_HandleTypeDef *huart, u8 *buff, u16 size);
u32  Get_Baudrate(UART_HandleTypeDef *huart);
u8   Get_Word_Length(UART_HandleTypeDef *huart);
u8   Get_Stop_Bits(UART_HandleTypeDef *huart);
u8   Get_Parity(UART_HandleTypeDef *huart);
void Uart_Proc(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32F1XX_HAL_DRIVER_INC_UART_HAL_H_ */
