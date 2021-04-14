#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stm32f4xx.h>
#include <sys.h>

/* 
	systick的寄存器设置默认是AHB/8。通过设置systick的CTRL寄存器的bit2来设置时钟 
	bit2 值：
		0:使用内核时钟：使用HCLK/8 作为Systick时钟；AHB/8
		1：使用HCLK作为Systick时钟；AHB
*/

int32_t delay_s(uint32_t ns);
int32_t delay_ms(uint32_t nms);
int32_t delay_us(uint32_t nus);

#endif
