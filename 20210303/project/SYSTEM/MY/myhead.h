#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#include <stm32f4xx.h>
#include <beep.h>
#include <led.h>
#include <btn.h>
#include <sys.h>
#include <systick.h>
#include <tim.h>
#include <pwm.h>
#include <usart.h>
#include <stdlib.h>
#include <sr04.h>

#pragma import(__use_no_semihosting_swi)
#include <stdio.h>

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
	// �ض������������1
	USART_SendData(USART1, ch);
	// �ȴ����ڷ��ͳɹ�
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return ch;
}

void _sys_exit(int return_code) 
{
	label: goto label; /* endless loop */
}

#endif
