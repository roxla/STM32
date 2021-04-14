#include "pwm.h"

void tim13_pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* ʹ�ܶ�ʱ��13ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	
	/* ���ö�ʱ�� */
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;	// 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//��STM32F407��Ч��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
	/* ��ʱ��14��PWMģʽ���ã�ͨ��1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		// ��ʼ�Ƚ�ֵ�������ڹ�ͬ������ռ�ձȣ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// ��ʼ���ԣ��ߵ�ƽ
	TIM_OC1Init(TIM13, &TIM_OCInitStructure);	// ͨ��1
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);	// ʹ��ͨ��1��װ��

	// ʹ�ܶ�ʱ��14ARR��װ��
	TIM_ARRPreloadConfig(TIM13, ENABLE);

	/* ʹ�ܶ�ʱ��14 */
	TIM_Cmd(TIM13, ENABLE);
}

void tim14_pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* ʹ�ܶ�ʱ��14ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	/* ���ö�ʱ�� */
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;	// 100000Hz=100kHz
	TIM_TimeBaseStructure.TIM_Period = 1000-1;		//����1000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//��STM32F407��Ч��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* ��ʱ��14��PWMģʽ���ã�ͨ��1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000;		// ��ʼ�Ƚ�ֵ�������ڹ�ͬ������ռ�ձȣ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// ��ʼ���ԣ��ߵ�ƽ
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);	// ͨ��1
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);	// ʹ��ͨ��1��װ��

	// ʹ�ܶ�ʱ��14ARR��װ��
	TIM_ARRPreloadConfig(TIM14, ENABLE);

	/* ʹ�ܶ�ʱ��14 */
	TIM_Cmd(TIM14, ENABLE);
}
