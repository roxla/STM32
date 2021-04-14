#include "rtc.h"

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

void rtc_init_set(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	
	/* ʹ�ܵ�Դ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC��ؼĴ���	*/
	PWR_BackupAccessCmd(ENABLE);

	/* ʹ���ⲿ�����ṩʱ��Դ��32.768kHz */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* �ȴ��������ã����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* ѡ��RTCӲ��ʱ�� */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ��ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ�RTC��ؼĴ������� */
	RTC_WaitForSynchro();
	
	/* ����RTC����ʼ�� */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;		//�첽��Ƶ
	RTC_InitStructure.RTC_SynchPrediv = 256-1;		//ͬ����Ƶ
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	
	/* �������ڣ�2021-3-17 ���� */
	RTC_DateStructure.RTC_Year = 0x21;
	RTC_DateStructure.RTC_Month = RTC_Month_March;
	RTC_DateStructure.RTC_Date = 0x17;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Wednesday ;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	/* ����ʱ�䣺���磺15:28:30 */
	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours   = 0x09;
	RTC_TimeStructure.RTC_Minutes = 0x50;
	RTC_TimeStructure.RTC_Seconds = 0x30; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	//�رջ��ѹ���
	RTC_WakeUpCmd(DISABLE);
	
	//Ϊ���ѹ���ѡ��RTC���úõ�ʱ��Դ
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//���û��Ѽ���ֵΪ�Զ����أ�д��ֵĬ����0
		//(���ý���1�μ���������1��ʱ��)
	RTC_SetWakeUpCounter(0);
	
	//���RTC�����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//ʹ��RTC�����ж�
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//ʹ�ܻ��ѹ���
	RTC_WakeUpCmd(ENABLE);

	/* �����ⲿ�жϿ�����22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//��ǰʹ���ⲿ�жϿ�����22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش����ж� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//ʹ���ⲿ�жϿ�����22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//����RTC�����жϴ���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//��Ӧ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	/* д��0x9527�����ݼĴ���0 */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x9527);

}

void rtc_init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	/* ʹ�ܵ�Դ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC��ؼĴ���	*/
	PWR_BackupAccessCmd(ENABLE);

	/* ʹ���ⲿ�����ṩʱ��Դ��32.768kHz */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* �ȴ��������ã����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* ѡ��RTCӲ��ʱ�� */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ��ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ�RTC��ؼĴ������� */
	RTC_WaitForSynchro();
	
	/* ����RTC����ʼ�� */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;		//�첽��Ƶ
	RTC_InitStructure.RTC_SynchPrediv = 256-1;		//ͬ����Ƶ
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	
	//�رջ��ѹ���
	RTC_WakeUpCmd(DISABLE);
	
	//Ϊ���ѹ���ѡ��RTC���úõ�ʱ��Դ
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//���û��Ѽ���ֵΪ�Զ����أ�д��ֵĬ����0
		//(���ý���1�μ���������1��ʱ��)
	RTC_SetWakeUpCounter(0);
	
	//���RTC�����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//ʹ��RTC�����ж�
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	//ʹ�ܻ��ѹ���
	RTC_WakeUpCmd(ENABLE);

	/* �����ⲿ�жϿ�����22 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//��ǰʹ���ⲿ�жϿ�����22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش����ж� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//ʹ���ⲿ�жϿ�����22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;		//����RTC�����жϴ���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//��Ӧ���ȼ�Ϊ0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

// RTC�����жϣ�1������
void RTC_WKUP_IRQHandler(void)
{	
	OS_ERR err;
	//˵����ǰ�����жϷ������
	OSIntEnter(); 
	
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		// ���ñ�־��
		OSFlagPost(&g_flag_grp, FLAG_RTC_WKUP, OS_OPT_POST_FLAG_SET, &err);
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
	
	//˵����ǰ�жϷ�����򼴽��˳�
	OSIntExit();
}
