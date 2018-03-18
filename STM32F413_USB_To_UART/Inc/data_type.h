/*
 * data_type.h
 *
 *  Created on: 2018年2月3日
 *      Author: Zero
 */

#ifndef STM32F1XX_HAL_DRIVER_SRC_DATA_TYPE_H_
#define STM32F1XX_HAL_DRIVER_SRC_DATA_TYPE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;

typedef enum {false = 0, true = 1} bool;

#ifdef __cplusplus
}
#endif

#endif /* STM32F1XX_HAL_DRIVER_SRC_DATA_TYPE_H_ */
