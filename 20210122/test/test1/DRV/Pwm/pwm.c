#include <myhead.h>

void pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 1.����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	// 2.��ʼ��GPIOF9Ϊ���ù��� 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // ���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // ����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // PF9
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	// 3.��TIM14�ĸ��ù���ӳ�䵽PF9
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	// 4.��ʼ��TIM14	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1; // 1MHz
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // ���¼���
	// 1MHz X 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	
	// 5.��ʼ��PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM1ģʽ
	TIM_OCInitStruct.TIM_Pulse = 50; // �Ƚϼ���ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //�͵�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	
	// 6.��ʼԤװ�غ��Զ���װ��
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	
	// 7.������ʱ��
	TIM_Cmd(TIM14, ENABLE);
}
