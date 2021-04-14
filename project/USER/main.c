#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "beep.h"
#include "pwm.h"
#include "rtc.h"
#include "includes.h"
#include "flash.h"
#include "oled.h"
#include "keyboard.h"
#include "w25q128.h"
#include "tim.h"
#include "ble.h"
#include "rfid.h"

//初始化控制块
OS_TCB app_task_tcb_init;
void app_task_init(void *parg);
CPU_STK app_task_stk_init[128];			//初始化的任务堆栈，大小为128字，也就是512字节

//温湿度控制块
OS_TCB app_task_tcb_dht11;
void app_task_dht11(void *parg);
CPU_STK app_task_stk_dht11[128];		//温湿度的任务堆栈，大小为128字，也就是512字节

//led控制块
OS_TCB app_task_tcb_led;
void app_task_led(void *parg);
CPU_STK app_task_stk_led[128];			//温湿度的任务堆栈，大小为128字，也就是512字节

//蜂鸣器控制块
OS_TCB app_task_tcb_beep;
void app_task_beep(void *parg);
CPU_STK app_task_stk_beep[128];			//温湿度的任务堆栈，大小为128字，也就是512字节

//任务rtc控制块
OS_TCB app_task_tcb_rtc;
void app_task_rtc(void *parg);
CPU_STK app_task_stk_rtc[128];			//rtc的任务堆栈，大小为128字，也就是512字节

//任务flash控制块
OS_TCB app_task_tcb_flash;
void app_task_flash(void *parg);
CPU_STK app_task_stk_flash[128];		//flash的任务堆栈，大小为128字，也就是512字节

//任务外部flash控制块
OS_TCB app_task_tcb_w25q128;
void app_task_w25q128(void *parg);
CPU_STK app_task_stk_w25q128[128];		//外部flash的任务堆栈，大小为128字，也就是512字节

//任务矩阵键盘控制块
OS_TCB app_task_tcb_keyboard;
void app_task_keyboard(void *parg);
CPU_STK app_task_stk_keyboard[128];		//矩阵键盘的任务堆栈，大小为128字，也就是512字节

//任务蓝牙控制块
OS_TCB app_task_tcb_ble;
void app_task_ble(void *parg);
CPU_STK app_task_stk_ble[128];			//蓝牙的任务堆栈，大小为128字，也就是512字节

//任务OLED控制块
OS_TCB app_task_tcb_oled;
void app_task_oled(void *parg);
CPU_STK app_task_stk_oled[128];			//OLED的任务堆栈，大小为128字，也就是512字节

//任务解锁控制块
OS_TCB app_task_tcb_lockon;
void app_task_lockon(void *parg);
CPU_STK app_task_stk_lockon[128];		//解锁的任务堆栈，大小为128字，也就是512字节

//任务解锁控制块
OS_TCB app_task_tcb_rfid;
void app_task_rfid(void *parg);
CPU_STK app_task_stk_rfid[128];			//解锁的任务堆栈，大小为128字，也就是512字节

OS_FLAG_GRP				g_flag_grp;			//事件标志组的对象

OS_MUTEX				g_mutex_oled;		//互斥锁的对象

OS_SEM					g_sem_led;			//信号量的对象
OS_SEM					g_sem_beep;			//信号量的对象

OS_Q					g_queue_key;		//消息队列的对象
OS_Q					g_queue_flash;		//消息队列的对象

char					g_pwd[7];			//密码(全局)
char 					g_cardData[7]; 		//卡密码(全局)

//主函数
int main(void)
{
	OS_ERR err;

	systick_init();  													//时钟初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//中断分组配置
	
	usart_init(9600);  				 									//串口初始化
	
	LED_Init();         												//LED初始化	

	//OS初始化，它是第一个运行的函数,初始化各种的全局变量，例如中断嵌套计数器、优先级、存储器
	OSInit(&err);


	//创建任务1
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_init,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_init",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_init,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)5,											 	//任务的优先级5		
					(CPU_STK *)app_task_stk_init,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
					
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_init create fail\r\n");
		
		while(1);
	
	}

	//启动OS，进行任务调度
	OSStart(&err);
					
					
	printf(".......\r\n");
					
	while(1);
	
}


void app_task_init(void *parg)
{
	OS_ERR err;
	
	// 温湿度初始化
	dht11_set_io(GPIO_Mode_OUT);
	
	// 配置LED1为复用模式
	led1_af_init();
	// 开启PWM
	tim14_pwm_config();
	
	// 配置beep为复用模式
	beep_af_init();
	// 开启PWM
	tim13_pwm_config();
	
	// RTC时钟初始化
	// 判断备份寄存器0是否为0x9527
		// 不是：代表RTC的时间需要设置
		// 是：代表RTC时间已设置，无需设置
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x9527)
		rtc_init_set();
	else
		rtc_init();
	
	// 初始化OLED
	OLED_Init(); 
	
	// 初始化外部flash
	w25qxx_init();
	
	// 键盘初始化
	keyboard_init();
	// 初始化密码设置
	flash_set_pwd(123456);
	// 初始化定时器3
//	TIM3_Init();
	
	// 蓝牙初始化
	uart3_init();
	ble_config();
	
	//创建事件标志组，所有标志位初值为0
	OSFlagCreate(&g_flag_grp,"g_flag_grp",0,&err);
	
	//创建消息队列，用于处理键盘数据
	OSQCreate(&g_queue_key,"g_queue_key",16,&err);
	//创建消息队列，用于处理flash数据
	OSQCreate(&g_queue_flash,"g_queue_flash",16,&err);
	
	//创建互斥锁
	OSMutexCreate(&g_mutex_oled, "g_mutex_oled",&err);
	
	// 温湿度任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_dht11,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_dht11",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_dht11,							//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_dht11,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_dht11 create fail\r\n");
		while(1);
	}
	
	// led任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_led,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_led",								//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_led,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_led,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_led create fail\r\n");
		while(1);
	}
	
	// beep任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_beep,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_beep",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_beep,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_beep,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_beep create fail\r\n");
		while(1);
	}
	
	// rtc任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_rtc,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_rtc",								//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_rtc,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_rtc,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_rtc create fail\r\n");
		while(1);
	}
	
	// flash任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_flash,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_flash",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_flash,							//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_flash,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_flash create fail\r\n");	
		while(1);
	}
	
	// 外部flash任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_w25q128,						//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_w25q128",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_w25q128,							//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_w25q128,						//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_w25q128 create fail\r\n");
		while(1);
	}
	
	// 矩阵键盘任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_keyboard,						//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_keyboard",						//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_keyboard,							//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_keyboard,						//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// 蓝牙任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_ble,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_ble",								//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_ble,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_ble,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// OLED任务创建
	// 设置标志组
	OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_oled,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_oled",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_oled,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_oled,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// 解锁任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_lockon,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_lockon",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_lockon,							//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_lockon,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// RFID任务创建
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_rfid,							//任务控制块，等同于线程id
					(CPU_CHAR *)"app_task_rfid",							//任务的名字，名字可以自定义的
					(OS_TASK_PTR)app_task_rfid,								//任务函数，等同于线程函数
					(void *)0,												//传递参数，等同于线程的传递参数
					(OS_PRIO)7,											 	//任务的优先级7		
					(CPU_STK *)app_task_stk_rfid,							//任务堆栈基地址
					(CPU_STK_SIZE)128/10,									//任务堆栈深度限位，用到这个位置，任务不能再继续使用
					(CPU_STK_SIZE)128,										//任务堆栈大小			
					(OS_MSG_QTY)0,											//禁止任务消息队列
					(OS_TICK)0,												//默认时间片长度																
					(void  *)0,												//不需要补充用户存储区
					(OS_OPT)OS_OPT_TASK_NONE,								//没有任何选项
					&err													//返回的错误码
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}

	//删除自身任务，进入休眠态
	OSTaskDel(NULL,&err);
}

void app_task_dht11(void *parg)
{
	uint8_t buf[5] = {0};
	char temp[30];
	OS_ERR err;
	
	while(1)
	{
		dht11_read_data(buf);
		snprintf(temp, sizeof(temp), "T: %d.%d  H: %d.%d", buf[2], buf[3], buf[0], buf[1]);
		//阻塞等待互斥锁
		OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
		
		OLED_ShowString(18,28,(u8 *)temp,12);
		OLED_Refresh();
		//立即释放互斥锁
		OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		
		delay_ms(6000);
	}
}

void app_task_led(void *parg)
{
	
}

void app_task_beep(void *parg)
{
	
}

void app_task_rtc(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	char tim[30];

	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_RTC_WKUP,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_RTC_WKUP)
		{
			//阻塞等待互斥锁
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			
			RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
			RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
			
			snprintf(tim, sizeof(tim), 
					"20%02X-%02X-%02X %02X:%02X:%02X", 
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					
			OLED_ShowString(8,53,(u8 *)tim,12);//6*12
			OLED_Refresh();
			
			//立即释放互斥锁
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		}
	}
}

void app_task_oled(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	
	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_OLED_MAIN_SHOW,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_OLED_MAIN_SHOW)
		{
			//阻塞等待互斥锁
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			
			OLED_Clear();
			OLED_ShowChinese(18,0,26,16);//智
			OLED_ShowChinese(36,0,27,16);//能
			OLED_ShowChinese(54,0,18,16);//密
			OLED_ShowChinese(72,0,19,16);//码
			OLED_ShowChinese(90,0,21,16);//锁
			OLED_Refresh();
			
			//立即释放互斥锁
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		}
	}
}

void app_task_flash(void *parg)
{
	uint32_t o_pwd = 0, i;
	char *p=NULL;
	OS_ERR  err;
	OS_MSG_SIZE msg_size;
	char str[20];
	// 获取密码
	flash_get_pwd(&o_pwd);
	sprintf(g_pwd, "%d", o_pwd);
	
	while(1)
	{
		memset(str, 0 ,sizeof(str));
		p = OSQPend(&g_queue_key,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
			printf("%s\r\n",p);
			if(strcmp(p,"A")==0)  //进入输入密码
			{
				// 挂起任务
				OSTaskSuspend(&app_task_tcb_rtc,&err);
				OSTaskSuspend(&app_task_tcb_dht11,&err);
				
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowChinese(18,0,15,16);//请
				OLED_ShowChinese(36,0,16,16);//输
				OLED_ShowChinese(54,0,17,16);//入
				OLED_ShowChinese(72,0,18,16);//密
				OLED_ShowChinese(90,0,19,16);//码
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				i = getpassword_key(str);
				printf("pwd = %s\r\n",str);
				printf("g_pwd = %s\r\n",g_pwd);
				if(i == 1)
				{
					i = virtual_pwd(str, g_pwd);
					if(i == 1)
					{	
						// 写入记录到flash中
						OSQPost(&g_queue_flash,"key",3,OS_OPT_POST_FIFO,&err);
					
						//阻塞等待互斥锁
						OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
						OLED_Clear();
						OLED_ShowChinese(27,0,20,16);//开
						OLED_ShowChinese(45,0,21,16);//锁
						OLED_ShowChinese(63,0,22,16);//成
						OLED_ShowChinese(81,0,23,16);//功
						OLED_Refresh();
						//立即释放互斥锁
						OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);

						LED2 = ~LED2;
						delay_ms(500);
						LED2 = ~LED2;
					}
					else
					{
						//阻塞等待互斥锁
						OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
						OLED_Clear();
						OLED_ShowChinese(27,0,20,16);//开
						OLED_ShowChinese(45,0,21,16);//锁
						OLED_ShowChinese(63,0,24,16);//失
						OLED_ShowChinese(81,0,25,16);//败
						OLED_Refresh();
						//立即释放互斥锁
						OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
						
						LED2 = ~LED2;
						delay_ms(1000);
						LED2 = ~LED2;
					}
				}
				// 设置标志组
				OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
				//恢复任务
				OSTaskResume(&app_task_tcb_rtc,&err);
				OSTaskResume(&app_task_tcb_dht11,&err);
			}
		}
	}
}

void app_task_w25q128(void *parg)
{
	OS_ERR err;
	uint16_t temp;
	char buf[64], unlock[20];
	uint32_t mem_rec_cnt = 0, i;
	char *p=NULL;
	OS_MSG_SIZE msg_size;
	
	//申请保护CPU的状态
	CPU_SR_ALLOC();
	
	temp = w25qxx_read_id();
	printf("0x%X\r\n",temp);
	
//	w25qxx_erase_sector(0);
	
	//尝试获取100条记录
	for(i=0;i<100;i++)
	{
		//获取存储的记录
		w25qxx_read_data(i*64,(u8 *)buf,64);
		
		//检查记录是否存在换行符号，不存在则不打印输出
		if(strstr((const char *)buf,"\n") == 0)
			break;	
	}
	mem_rec_cnt = i;	// flash存储的操作记录的数量
	
	while(1)
	{
		p = OSQPend(&g_queue_flash,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
			if(mem_rec_cnt < 100)
			{
				RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
				RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
				// 密码解锁
				if(strcmp(p,"key") == 0)
					snprintf(unlock, 20, "%s", "密码解锁");
				// 蓝牙解锁
				if(strcmp(p,"ble") == 0)
					snprintf(unlock, 20, "%s", "蓝牙解锁");
				// RFID解锁
				if(strcmp(p,"rfid") == 0)
					snprintf(unlock, 20, "%s", "RFID解锁");
				// 指纹解锁
				if(strcmp(p,"as608") == 0)
					snprintf(unlock, 20, "%s", "指纹解锁");
				
				snprintf(buf, 64, "20%02X-%02X-%02X %02X:%02X:%02X  %s\r\n", 
						RTC_DateStructure.RTC_Year,
						RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date,
						RTC_TimeStructure.RTC_Hours,
						RTC_TimeStructure.RTC_Minutes,
						RTC_TimeStructure.RTC_Seconds,
						unlock);
				
				OS_CRITICAL_ENTER();	// flash写入数据，进入临界区
				w25qxx_write_page(mem_rec_cnt*64,(u8 *)buf,64);
				OS_CRITICAL_EXIT() ;  //flash写入数据完成，退出临界区
				
				mem_rec_cnt++;	// 写入计数+1
			}
			else
			{
				// 挂起实时时间显示
				OSTaskSuspend(&app_task_tcb_rtc,&err);
				OSTaskSuspend(&app_task_tcb_dht11,&err);
			
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowString(28,50,"data full!",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				OS_CRITICAL_ENTER();	// flash擦除数据，进入临界区
				w25qxx_erase_sector(0);
				OS_CRITICAL_EXIT() ;  //flash擦除数据完成，退出临界区
				
				mem_rec_cnt = 0;
				delay_ms(100);
				// 设置标志组
				OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
				OSQPost(&g_queue_flash,p,strlen(p),OS_OPT_POST_FIFO,&err);
				//恢复实时时钟
				OSTaskResume(&app_task_tcb_rtc,&err);
				OSTaskResume(&app_task_tcb_dht11,&err);
			}
		}
	}
	

}

void app_task_keyboard(void *parg)
{
	while(1)
	{
		get_key();
		delay_ms(50);
	}
}

void app_task_ble(void *parg)
{	
	while(1)
	{
		parse_cmd();
		delay_ms(100);
	}
}

void app_task_lockon(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_LOCKON_ALL,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_LOCKON_FLAG)
		{
			// 挂起实时时间显示
			OSTaskSuspend(&app_task_tcb_rtc,&err);
			OSTaskSuspend(&app_task_tcb_dht11,&err);

			// BLE
			if(flags & FLAG_OLED_BLE_LOCKON)
			{
				// 写入记录到flash中
				OSQPost(&g_queue_flash,"ble",3,OS_OPT_POST_FIFO,&err);
			
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowChinese(27,0,20,16);//开
				OLED_ShowChinese(45,0,21,16);//锁
				OLED_ShowChinese(63,0,22,16);//成
				OLED_ShowChinese(81,0,23,16);//功
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				LED2 = ~LED2;
				delay_ms(500);
				LED2 = ~LED2;
			}
			// RFID
			if(flags & FLAG_OLED_RFID_LOCKON)
			{
				if(strcmp(g_cardData,"123456") == 0)
				{
					// 写入记录到flash中
					OSQPost(&g_queue_flash,"rfid",4,OS_OPT_POST_FIFO,&err);
					
					//阻塞等待互斥锁
					OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
					OLED_Clear();
					OLED_ShowChinese(27,0,20,16);//开
					OLED_ShowChinese(45,0,21,16);//锁
					OLED_ShowChinese(63,0,22,16);//成
					OLED_ShowChinese(81,0,23,16);//功
					OLED_Refresh();
					//立即释放互斥锁
					OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
					
					LED2 = ~LED2;
					delay_ms(500);
					LED2 = ~LED2;
				}
				else
				{
					//阻塞等待互斥锁
					OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
					OLED_Clear();
					OLED_ShowChinese(27,0,20,16);//开
					OLED_ShowChinese(45,0,21,16);//锁
					OLED_ShowChinese(63,0,24,16);//失
					OLED_ShowChinese(81,0,25,16);//败
					OLED_Refresh();
					//立即释放互斥锁
					OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
					
					LED2 = ~LED2;
					delay_ms(500);
					LED2 = ~LED2;
					
				}
				memset(g_cardData, 0, 7);
			}
			
			// 设置标志组
			OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
			//恢复实时时钟
			OSTaskResume(&app_task_tcb_rtc,&err);
			OSTaskResume(&app_task_tcb_dht11,&err);
		}
	}
}

void app_task_rfid(void *parg)
{
	while(1)
	{
		MFRC522_Initializtion();			// 初始化MFRC522
		MFRC522Get(g_cardData);
		delay_ms(1000);
	}
}
