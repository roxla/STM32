#include <tim.h>

void tim2_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�ܶ�ʱ��2ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* ���ö�ʱ�� */
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		//��������
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָF407��Ч��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// ʹ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* ���ö�ʱ���ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* �򿪶�ʱ��2 */
	TIM_Cmd(TIM2, ENABLE);
}

void tim3_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�ܶ�ʱ��3ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* ���ö�ʱ�� */
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		//��������
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷָF407��Ч��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// ʹ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* ���ö�ʱ���ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* �򿪶�ʱ��3 */
	TIM_Cmd(TIM3, ENABLE);
}
