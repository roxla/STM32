#include <myhead.h>

void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//1.ʹ��PWR��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	//2.ʹ��RTC�Ĵ�������
	PWR_BackupAccessCmd(ENABLE);
	
	//3.ʹ��LSE����ѡ��LSE��ΪRTC��ʱ��Դ
	RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ�LSE����
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	//ѡ��LSE��ΪRTCʱ��Դ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	//4.ʹ��RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	
	//5.ȷ��RTC�Ĵ�������
	RTC_WaitForSynchro();
	
	//6.��ʼ��RTC�ķ�Ƶϵ��(�첽��Ƶֵ������)��ʱ���ʽ
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;//24Сʱ��ʽ
	RTC_InitStruct.RTC_AsynchPrediv = 0x7f;//128�첽��Ƶ
	RTC_InitStruct.RTC_SynchPrediv = 0xff;//256ͬ����Ƶ
	RTC_Init(&RTC_InitStruct);
	
	//7.�������ں�ʱ�� 2021-1-27 14:30:30
	RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
	RTC_TimeStruct.RTC_Hours = 0x14;//BCD��
	RTC_TimeStruct.RTC_Minutes = 0x30;
	RTC_TimeStruct.RTC_Seconds = 0x30;
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
	
	RTC_DateStruct.RTC_Year = 0x21;
	RTC_DateStruct.RTC_Month = RTC_Month_January;
	RTC_DateStruct.RTC_Date = 0x27;
	RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Wednesday;
	RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
	
	//8.д���ݼĴ���0
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
}

//����ʱ��
int set_time(char *s)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	int i = 0;
	char *p = NULL;
	char *time_str[3] = {0};
	char time_buf[3] = {0};
	
	//��ʱ����Ϣ���ַ�����ȡ�� settime:12:12:12
	p = strtok(s,":");
	if(p&&(strcmp(p,"settime")==0)){
		//����ȡ��ʱ����
		for(i=0;i<3;i++){
			time_str[i] = strtok(NULL,":");
			if(!time_str[i])
				return -1;
		}
		
		//ȡ��ʱ�����Ӧ������
		p = strtok(NULL,":");
		if(p)
			return -1;
	}
	else{
		return -1;
	}
	
	//��ʱ�����ַ���ת��BCD��
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
	
	//1.ʹ��PWR��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//2.ʹ��RTC�Ĵ�������
	PWR_BackupAccessCmd(ENABLE);
	//4.ʹ��RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	//5.ȷ��RTC�Ĵ�������
	RTC_WaitForSynchro();
 
 	//7.�������ں�ʱ��
	RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
	RTC_TimeStruct.RTC_Hours = time_buf[0];//BCD��
	RTC_TimeStruct.RTC_Minutes = time_buf[1];
	RTC_TimeStruct.RTC_Seconds = time_buf[2];
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
	
	return 0;
}

//���ӳ�ʼ��
void rtc_alarm_init(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.ʹ��PWR��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	//2.ʹ��RTC�Ĵ�������
	PWR_BackupAccessCmd(ENABLE);
	
	//3.ʹ��RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	
	//4.ȷ��RTC�Ĵ�������
	RTC_WaitForSynchro();
	
	//5.�ر�����
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//6.��������A����
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x15;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x30;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0x50;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;//ѡ������
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = 0x24;//27��
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay|RTC_AlarmMask_Hours;//RTC_AlarmMask_None;//������
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
	
	//7.����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	
	//8.ʹ������A�ж�
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//9.��ʼ��EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;//�ⲿ�ж���17 ----- ����
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	//10.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;//RTC�����ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//11.ʹ��RTC����
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

//�жϴ�����
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET){//����A�ж�
		BEEP = 1;
		//����жϱ�־
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	
	//����ⲿ�ж���17�ж�
	EXTI_ClearITPendingBit(EXTI_Line17);
}
