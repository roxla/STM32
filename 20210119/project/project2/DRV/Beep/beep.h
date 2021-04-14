#ifndef _BEEP_H_
#define _BEEP_H_

#include <stm32f4xx.h>

// 延时
void delay(unsigned int ms);
// 初始化
void beep_init(void);
// 控制
void beep_switch(int state);

#endif