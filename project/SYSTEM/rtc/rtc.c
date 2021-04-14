#include "rtc.h"

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

void rtc_init_set(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	
	/* 使能电源控制时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC相关寄存器	*/
	PWR_BackupAccessCmd(ENABLE);

	/* 使能外部晶振提供时钟源：32.768kHz */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 等待晶振启用（起振） */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* 选择RTC硬件时钟 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 打开时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待RTC相关寄存器就绪 */
	RTC_WaitForSynchro();
	
	/* 配置RTC并初始化 */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;		//异步分频
	RTC_InitStructure.RTC_SynchPrediv = 256-1;		//同步分频
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);
	
	/* 设置日期：2021-3-17 周三 */
	RTC_DateStructure.RTC_Year = 0x21;
	RTC_DateStructure.RTC_Month = RTC_Month_March;
	RTC_DateStructure.RTC_Date = 0x17;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Wednesday ;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	/* 设置时间：下午：15:28:30 */
	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours   = 0x09;
	RTC_TimeStructure.RTC_Minutes = 0x50;
	RTC_TimeStructure.RTC_Seconds = 0x30; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	//关闭唤醒功能
	RTC_WakeUpCmd(DISABLE);
	
	//为唤醒功能选择RTC配置好的时钟源
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//设置唤醒计数值为自动重载，写入值默认是0
		//(设置进行1次计数，就是1秒时间)
	RTC_SetWakeUpCounter(0);
	
	//清除RTC唤醒中断标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//使能RTC唤醒中断
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//使能唤醒功能
	RTC_WakeUpCmd(ENABLE);

	/* 配置外部中断控制线22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//当前使用外部中断控制线22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发中断 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//使能外部中断控制线22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//允许RTC唤醒中断触发
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//响应优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//使能
	NVIC_Init(&NVIC_InitStructure);
	
	/* 写入0x9527到备份寄存器0 */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x9527);

}

void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	/* 使能电源控制时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC相关寄存器	*/
	PWR_BackupAccessCmd(ENABLE);

	/* 使能外部晶振提供时钟源：32.768kHz */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 等待晶振启用（起振） */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* 选择RTC硬件时钟 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 打开时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待RTC相关寄存器就绪 */
	RTC_WaitForSynchro();
	
	/* 配置RTC并初始化 */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;		//异步分频
	RTC_InitStructure.RTC_SynchPrediv = 256-1;		//同步分频
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);
	
	//关闭唤醒功能
	RTC_WakeUpCmd(DISABLE);
	
	//为唤醒功能选择RTC配置好的时钟源
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//设置唤醒计数值为自动重载，写入值默认是0
		//(设置进行1次计数，就是1秒时间)
	RTC_SetWakeUpCounter(0);
	
	//清除RTC唤醒中断标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//使能RTC唤醒中断
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//使能唤醒功能
	RTC_WakeUpCmd(ENABLE);

	/* 配置外部中断控制线22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//当前使用外部中断控制线22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发中断 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//使能外部中断控制线22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//允许RTC唤醒中断触发
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//响应优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//使能
	NVIC_Init(&NVIC_InitStructure);
}

// RTC唤醒中断（1秒间隔）
void RTC_WKUP_IRQHandler(void)
{	
	OS_ERR err;
	//说明当前进入中断服务程序
	OSIntEnter(); 
	
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		// 设置标志组
		OSFlagPost(&g_flag_grp, FLAG_RTC_WKUP, OS_OPT_POST_FLAG_SET, &err);
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
	
	//说明当前中断服务程序即将退出
	OSIntExit();
}
