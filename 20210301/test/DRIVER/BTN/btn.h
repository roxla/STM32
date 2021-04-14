#ifndef _BTN_H_
#define _BTN_H_

#include <stm32f4xx.h>
#include <sys.h>

#define KEY1 PAin(0)
#define KEY2 PEin(2)
#define KEY3 PEin(3)
#define KEY4 PEin(4)

void btn_init(void);

#endif
