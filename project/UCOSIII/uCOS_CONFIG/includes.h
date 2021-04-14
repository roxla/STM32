/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>

/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <bsp.h>
//#include  <bsp_int.h>

extern OS_FLAG_GRP			g_flag_grp;			//事件标志组的对象

extern OS_MUTEX				g_mutex_oled;		//互斥锁的对象

extern OS_SEM				g_sem_led;			//信号量的对象
extern OS_SEM				g_sem_beep;			//信号量的对象

extern OS_Q					g_queue_key;		//消息队列的对象
extern OS_Q					g_queue_flash;		//消息队列的对象

extern RTC_TimeTypeDef RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure;
extern char g_pwd[7];
extern char g_cardData[7];

#define FLAG_CHANGE_PWD		 		0x01			// 密码修改
#define FLAG_OLED_KEY_LOCKON 		0x04			// 密码开锁
#define FLAG_OLED_BLE_LOCKON 		0x08			// 蓝牙开锁
#define FLAG_OLED_MAIN_SHOW  		0x10			// OLED主界面显示
#define FLAG_RTC_WKUP		 		0x20			// RTC唤醒
#define FLAG_OLED_RFID_LOCKON		0x40			// RFID解锁
#define FLAG_OLED_as608_LOCKON		0x80			// 指纹解锁
#define FLAG_LOCKON_FLAG 			0x02			// 解锁判断
#define FLAG_LOCKON_ALL		 		0xce			// 解锁标志位

//任务控制块
extern OS_TCB app_task_tcb_led;
extern OS_TCB app_task_tcb_rtc;
extern OS_TCB app_task_tcb_dht11;
extern OS_TCB app_task_tcb_keyboard;
extern OS_TCB app_task_tcb_w25q128;
extern OS_TCB app_task_tcb_flash;
extern OS_TCB app_task_tcb_ble;
extern OS_TCB app_task_tcb_oled;
extern OS_TCB app_task_tcb_rfid;

#endif
