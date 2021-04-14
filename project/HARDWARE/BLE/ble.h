#ifndef _BLE_H_
#define _BLE_H_

#include "stm32f4xx.h" 
#include "sys.h"
#include "delay.h"
#include "includes.h"
#include "w25q128.h"
#include "rfid.h"

static volatile u32 uart3_cnt = 0;//记录串口3收到的数据长度
static volatile u8 uart3_buf[64] = {0};//记录uart3收到的数据
static volatile u32 uart3_flag = 0;//记录uart3收到完整数据 1 ----- 完整

extern OS_MUTEX				g_mutex_oled;		//互斥锁的对象

extern void ble_config(void);
extern void uart3_init(void);
extern void uart3_putc(char ch);
extern void uart3_puts(const char *s);
extern void parse_cmd(void);

#endif
