#ifndef _W25Q128_H_
#define _W25Q128_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "includes.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

#define W25QXX_SS   PBout(14)

extern void w25qxx_init(void);
extern uint8_t SPI1_SendByte(uint8_t byte);
extern uint16_t w25qxx_read_id(void);
extern void w25qxx_read_data(uint32_t addr,uint8_t *pbuf,uint32_t len);
extern uint8_t w25qxx_read_status1(void);
extern void w25qxx_write_enable(void);
extern void w25qxx_write_disable(void);
extern void w25qxx_erase_sector(uint32_t addr);
extern void w25qxx_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len);

#endif
