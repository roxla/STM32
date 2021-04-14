#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f4xx.h" 
#include "sys.h"
#include "includes.h"
#include "oled.h"

#define FLASH_ADDR_SECTOR_1 0x08004000
#define FLASH_ADDR_SECTOR_2 0x08008000
#define FLASH_ADDR_SECTOR_3 0x0800C000
#define FLASH_ADDR_SECTOR_4 0x08010000
#define FLASH_ADDR_SECTOR_5 0x08020000
#define FLASH_ADDR_SECTOR_6 0x08040000
#define FLASH_ADDR_SECTOR_7 0x08060000

#define O_PWD_LEN 6

extern void flash_set_pwd(u32 pwd);
extern void flash_get_pwd(u32 *pwd);
extern uint8_t virtual_pwd(char *pwd, char *o_pwd);

#endif
