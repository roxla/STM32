#ifndef _SR04_H_
#define _SR04_H_

#include <stm32f4xx.h>
#include <sys.h>
#include <systick.h>

#define TRIG PCout(7)
#define ECHO PCin(9)

// ≥ı ºªØ
void sr04_init(void);

uint32_t sr04_get_distance(void);

#endif
