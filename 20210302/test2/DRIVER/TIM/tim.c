#include <tim.h>

void tim2_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能定时器2时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* 配置定时器 */
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		//计数周期
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割（F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式：向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// 使能中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* 配置定时器中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 打开定时器2 */
	TIM_Cmd(TIM2, ENABLE);
}

void tim3_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能定时器3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* 配置定时器 */
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		//计数周期
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割（F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式：向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// 使能中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* 配置定时器中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 打开定时器3 */
	TIM_Cmd(TIM3, ENABLE);
}
