#ifndef _BEEP_H_
#define _BEEP_H_

#include <stm32f4xx.h>
#include <sys.h>

#define BEEP PFout(8)

// ≥ı ºªØ
void beep_init(void);
void beep_af_init(void);

#endif
