#include <myhead.h>

void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//1.使能PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	//2.使能RTC寄存器访问
	PWR_BackupAccessCmd(ENABLE);
	
	//3.使能LSE并且选择LSE作为RTC的时钟源
	RCC_LSEConfig(RCC_LSE_ON);
	//等待LSE就绪
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	//选择LSE作为RTC时钟源
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	//4.使能RTC时钟
	RCC_RTCCLKCmd(ENABLE);
	
	//5.确认RTC寄存器就绪
	RTC_WaitForSynchro();
	
	//6.初始化RTC的分频系数(异步分频值尽量大)和时间格式
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;//24小时格式
	RTC_InitStruct.RTC_AsynchPrediv = 0x7f;//128异步分频
	RTC_InitStruct.RTC_SynchPrediv = 0xff;//256同步分频
	RTC_Init(&RTC_InitStruct);
	
	//7.设置日期和时间 2021-1-27 14:30:30
	RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
	RTC_TimeStruct.RTC_Hours = 0x14;//BCD码
	RTC_TimeStruct.RTC_Minutes = 0x30;
	RTC_TimeStruct.RTC_Seconds = 0x30;
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
	
	RTC_DateStruct.RTC_Year = 0x21;
	RTC_DateStruct.RTC_Month = RTC_Month_January;
	RTC_DateStruct.RTC_Date = 0x27;
	RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Wednesday;
	RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
	
	//8.写备份寄存器0
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
}

//设置时间
int set_time(char *s)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	int i = 0;
	char *p = NULL;
	char *time_str[3] = {0};
	char time_buf[3] = {0};
	
	//把时间信息从字符串中取出 settime:12:12:12
	p = strtok(s,":");
	if(p&&(strcmp(p,"settime")==0)){
		//依次取出时分秒
		for(i=0;i<3;i++){
			time_str[i] = strtok(NULL,":");
			if(!time_str[i])
				return -1;
		}
		
		//取完时分秒后应无数据
		p = strtok(NULL,":");
		if(p)
			return -1;
	}
	else{
		return -1;
	}
	
	//将时分秒字符串转成BCD码
	time_buf[0] = strtol(time_str[0], NULL, 16);
	if(time_buf[0]<0||time_buf[0]>0x23){
		return -1;
	}
	time_buf[1] = strtol(time_str[1], NULL, 16);
	if(time_buf[1]<0||time_buf[1]>0x59){
		return -1;
	}
	time_buf[2] = strtol(time_str[2], NULL, 16);
	if(time_buf[2]<0||time_buf[2]>0x59){
		return -1;
	}
	
	//1.使能PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//2.使能RTC寄存器访问
	PWR_BackupAccessCmd(ENABLE);
	//4.使能RTC时钟
	RCC_RTCCLKCmd(ENABLE);
	//5.确认RTC寄存器就绪
	RTC_WaitForSynchro();
 
 	//7.设置日期和时间
	RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
	RTC_TimeStruct.RTC_Hours = time_buf[0];//BCD码
	RTC_TimeStruct.RTC_Minutes = time_buf[1];
	RTC_TimeStruct.RTC_Seconds = time_buf[2];
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
	
	return 0;
}

//闹钟初始化
void rtc_alarm_init(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.使能PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	//2.使能RTC寄存器访问
	PWR_BackupAccessCmd(ENABLE);
	
	//3.使能RTC时钟
	RCC_RTCCLKCmd(ENABLE);
	
	//4.确认RTC寄存器就绪
	RTC_WaitForSynchro();
	
	//5.关闭闹钟
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//6.设置闹钟A参数
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x15;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x30;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0x50;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;//选择日期
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = 0x24;//27号
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay|RTC_AlarmMask_Hours;//RTC_AlarmMask_None;//不屏蔽
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
	
	//7.清除中断标志
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	
	//8.使能闹钟A中断
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//9.初始化EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;//外部中断线17 ----- 闹钟
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	//10.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;//RTC闹钟中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//11.使能RTC闹钟
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

//中断处理函数
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET){//闹钟A中断
		BEEP = 1;
		//清除中断标志
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	
	//清除外部中断线17中断
	EXTI_ClearITPendingBit(EXTI_Line17);
}
