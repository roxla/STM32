#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f4xx.h" 
#include "sys.h"

#define BEEP PFout(8)

// ≥ı ºªØ
extern void beep_af_init(void);

#endif
