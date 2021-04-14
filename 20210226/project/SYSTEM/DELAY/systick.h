#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stm32f4xx.h>
#include <sys.h>

/* 
	systick�ļĴ�������Ĭ����AHB/8��ͨ������systick��CTRL�Ĵ�����bit2������ʱ�� 
	bit2 ֵ��
		0:ʹ���ں�ʱ�ӣ�ʹ��HCLK/8 ��ΪSystickʱ�ӣ�AHB/8
		1��ʹ��HCLK��ΪSystickʱ�ӣ�AHB
*/

int32_t delay_s(uint32_t ns);
int32_t delay_ms(uint32_t nms);
int32_t delay_us(uint32_t nus);

#endif
