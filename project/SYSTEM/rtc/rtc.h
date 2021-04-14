#ifndef _RTC_H
#define _RTC_H

#include "stm32f4xx.h" 
#include "includes.h"

// 初始化RTC设置（带时间设置）
extern void rtc_init_set(void);
// 初始化RTC设置（不带时间设置）
extern void rtc_init(void);

#endif
