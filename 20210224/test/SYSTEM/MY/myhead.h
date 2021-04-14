#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#include <stm32f4xx.h>
#include <beep.h>
#include <led.h>
#include <btn.h>
//#include <systick.h>

#define PFout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOF_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PEout(x)	*(volatile uint32_t *)(0x42000000 + (GPIOE_BASE + 0x14 - 0x40000000)*32 + x*4)

#endif
