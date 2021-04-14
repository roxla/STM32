#include <myhead.h>

void timer10_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 1.ʹ�ܶ�ʱ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	//2.��ʼ����ʱ��10
	//168MHz / 16800 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // ���¼���
	// 10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 5000; // 500ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStruct);
	
	//3.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn; // TIM10
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // ��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // ��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//4.ʹ�ܶ�ʱ��10�ж�
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	//6.ʹ�ܶ�ʱ���ж�
	TIM_Cmd(TIM10, ENABLE);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	//��������ж�
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET){
		PFout(14) ^= 1;
		
		//����жϱ�־
		TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
	}
}
