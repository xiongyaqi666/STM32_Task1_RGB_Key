/*
 * bsp_buzzer.h
 * Optional buzzer control for Task 1.
 */
#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef enum
{
    BUZZER_ON = 0,
    BUZZER_OFF,
    BUZZER_BEEP_INTERVAL
} BuzzerState_t;

void Buzzer_Init(void);
void Buzzer_SetState(BuzzerState_t state);
void Buzzer_Task(void);
BuzzerState_t Buzzer_GetState(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BUZZER_H__ */
