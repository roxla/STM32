#ifndef _BLE_H_
#define _BLE_H_

#include "stm32f4xx.h" 
#include "sys.h"
#include "delay.h"
#include "includes.h"
#include "w25q128.h"
#include "rfid.h"

static volatile u32 uart3_cnt = 0;//��¼����3�յ������ݳ���
static volatile u8 uart3_buf[64] = {0};//��¼uart3�յ�������
static volatile u32 uart3_flag = 0;//��¼uart3�յ��������� 1 ----- ����

extern OS_MUTEX				g_mutex_oled;		//�������Ķ���

extern void ble_config(void);
extern void uart3_init(void);
extern void uart3_putc(char ch);
extern void uart3_puts(const char *s);
extern void parse_cmd(void);

#endif
