#ifndef _USART_H_
#define _USART_H_

#include <stm32f4xx.h>
#include <sys.h>

void usart1_init(uint32_t baud);
void usart3_init(uint32_t baud);

void usart1_send_str(char *str);
void usart3_send_str(char *str);

#endif
