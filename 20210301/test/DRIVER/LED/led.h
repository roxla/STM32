#ifndef _LED_H_
#define _LED_H_

#include <stm32f4xx.h>
#include <sys.h>

#define LED1 PFout(9)
#define LED2 PFout(10)
#define LED3 PEout(13)
#define LED4 PEout(14)

// ≥ı ºªØ
void led_init(void);

#endif
