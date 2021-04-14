#ifndef __AS608_FUN_H
#define __AS608_FUN_H

#include "delay.h" 	
#include "usart2.h"
#include "as608.h"
#include "oled.h"
#include "timer.h"
#include "usart.h"
#include "sys.h"
#include "beep.h"
#include "key.h"
#include "includes.h"
#include "stm32f4xx.h"
#include <stdio.h>

#define usart2_baund  57600

//¼ָ��
uint32_t Add_FR(void);

//ˢָ��
uint32_t press_FR(void);

//ɾ��ָ��
uint32_t Del_FR(void);

//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure);

#endif
