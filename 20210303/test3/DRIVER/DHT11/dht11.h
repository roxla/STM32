#ifndef _DHT11_H_
#define _DHT11_H_

#include <stm32f4xx.h>
#include <sys.h>
#include <systick.h>

#define DQ_OUT PGout(9)
#define DQ_IN PGin(9)

void dht11_set_io(GPIOMode_TypeDef IO);
int8_t dht11_read_data(uint8_t *pbuf);

#endif
