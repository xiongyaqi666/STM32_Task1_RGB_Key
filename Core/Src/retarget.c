/*
 * retarget.c
 * printf retarget to USART1.
 */
#include "retarget.h"
#include "usart.h"
#include <stdio.h>

#if defined(__CC_ARM)
#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
};
FILE __stdout;

void _sys_exit(int x)
{
    (void)x;
}
#endif

int fputc(int ch, FILE *f)
{
    uint8_t data;

    (void)f;
    data = (uint8_t)ch;
    (void)HAL_UART_Transmit(&huart1, &data, 1, 10);
    return ch;
}

#if defined(__GNUC__)
int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)ptr, (uint16_t)len, 100);
    return len;
}
#endif
