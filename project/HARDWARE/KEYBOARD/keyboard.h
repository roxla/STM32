#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "includes.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"

extern void keyboard_init(void);
extern void get_key(void);
extern void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch);
extern u8 getpassword_key(char *pwd);

#endif
