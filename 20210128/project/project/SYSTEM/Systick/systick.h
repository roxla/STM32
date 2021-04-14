#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stm32f4xx.h>

void systick_init(void);
void my_delay_ms(u32 nms);
void my_delay_us(u32 nus);
void my_delay_s(u32 s);

#endif
