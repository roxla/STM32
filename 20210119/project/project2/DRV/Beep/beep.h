#ifndef _BEEP_H_
#define _BEEP_H_

#include <stm32f4xx.h>

// ��ʱ
void delay(unsigned int ms);
// ��ʼ��
void beep_init(void);
// ����
void beep_switch(int state);

#endif