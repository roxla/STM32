#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h" 
#include "sys.h"

extern void LED_Init(void);//≥ı ºªØ
extern void led1_af_init(void);

#define LED0 PFout(9)
#define LED1 PFout(10)
#define LED2 PEout(13)
#define LED3 PEout(14)

#endif
