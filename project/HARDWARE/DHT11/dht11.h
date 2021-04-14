#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f4xx.h" 
#include "delay.h"
#include "sys.h"

#define DQ_OUT PGout(9)
#define DQ_IN PGin(9)

//初始化温湿度模块（PG9）
extern void dht11_set_io(GPIOMode_TypeDef IO);
// 获取温湿度模块数据（单总线：PG9）
extern int8_t dht11_read_data(uint8_t *pbuf);

#endif
