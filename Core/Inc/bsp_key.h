/*
 * bsp_key.h
 * Software debounce and key event detection.
 */
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define KEY_EVENT_NONE                 0x00U
#define KEY1_EVENT_SHORT               0x01U
#define KEY2_EVENT_LONG_START          0x02U
#define KEY2_EVENT_LONG_RELEASE        0x04U

void BSP_Key_Init(void);
uint8_t BSP_Key_Scan(void);
uint8_t BSP_Key_IsKey2LongActive(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_KEY_H__ */
