#include <myhead.h>

void timer10_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 1.使能定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	//2.初始化定时器10
	//168MHz / 16800 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
	// 10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 5000; // 500ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStruct);
	
	//3.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn; // TIM10
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//4.使能定时器10中断
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	//6.使能定时器中断
	TIM_Cmd(TIM10, ENABLE);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	//处理更新中断
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET){
		PFout(14) ^= 1;
		
		//清除中断标志
		TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
	}
}
