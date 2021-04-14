#include "tim.h"

extern OS_FLAG_GRP g_flag_grp;

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能定时器3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* 配置定时器3 */
	TIM_TimeBaseStructure.TIM_Period = 60000-1;		//计数周期（6秒）
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//预分频系数（1000Hz=1kHz）
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割（F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式：向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// 使能中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* 配置定时器中断优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// TIM3中断服务函数
void TIM3_IRQHandler(void)
{
	OS_ERR err;

	//说明当前进入中断服务程序
	OSIntEnter(); 
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		// 设置标志组
		OSFlagPost(&g_flag_grp, 0x02, OS_OPT_POST_FLAG_SET, &err);
	}
	
	//说明当前中断服务程序即将退出
	OSIntExit();
}
