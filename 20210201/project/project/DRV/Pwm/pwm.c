#include <myhead.h>

void pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 1.����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // LED4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // TIM1_CH4
	
	// 2.��ʼ��GPIOF9Ϊ���ù��� 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // ���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // ����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14; // PE14
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// 3.��TIM1�ĸ��ù���ӳ�䵽PE14
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	// 4.��ʼ��TIM1
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // ���¼���
	// 1MHz X 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000; // 100ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	// 5.��ʼ��PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM1ģʽ
	TIM_OCInitStruct.TIM_Pulse = 50; // �Ƚϼ���ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //�͵�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	// 6.��ʼԤװ�غ��Զ���װ��
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	// 7.������ʱ��
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}
