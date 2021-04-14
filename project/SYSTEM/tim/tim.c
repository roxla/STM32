#include "tim.h"

extern OS_FLAG_GRP g_flag_grp;

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�ܶ�ʱ��3ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* ���ö�ʱ��3 */
	TIM_TimeBaseStructure.TIM_Period = 60000-1;		//�������ڣ�6�룩
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//Ԥ��Ƶϵ����1000Hz=1kHz��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָF407��Ч��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// ʹ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* ���ö�ʱ���ж����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// TIM3�жϷ�����
void TIM3_IRQHandler(void)
{
	OS_ERR err;

	//˵����ǰ�����жϷ������
	OSIntEnter(); 
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		// ���ñ�־��
		OSFlagPost(&g_flag_grp, 0x02, OS_OPT_POST_FLAG_SET, &err);
	}
	
	//˵����ǰ�жϷ�����򼴽��˳�
	OSIntExit();
}
