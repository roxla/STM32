#include <myhead.h>

// TIM1
void timer1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 1.使能定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// 2.初始化定时器1
	// 168MHz / 16800 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
	// 10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 1000; // 100ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//3.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;//TIM1
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//4.使能定时器2中断
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//6.使能定时器中断
	TIM_Cmd(TIM1,ENABLE);
}
// TIM1
void TIM1_UP_TIM10_IRQHandler(void)
{
	//处理更新中断
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
		PFout(9) ^= 1;
		
		//清除中断标志
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
// TIM2
void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//2.初始化定时器2
	//84MHz / 8400 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//向下计数
	//10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 2000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	//3.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//TIM2
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//4.使能定时器2中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//6.使能定时器中断
	TIM_Cmd(TIM2,ENABLE);
}
// TIM2
void TIM2_IRQHandler(void)
{
	//处理更新中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		PFout(10) ^= 1;
		
		//清除中断标志
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
// TIM3
void timer3_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//2.初始化定时器2
	//84MHz / 8400 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//向下计数
	//10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 5000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

	//3.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//TIM2
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//4.使能定时器2中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//6.使能定时器中断
	TIM_Cmd(TIM3,ENABLE);
}
// TIM3
void TIM3_IRQHandler(void)
{
	//处理更新中断
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET){
		PEout(13) ^= 1;
		
		//清除中断标志
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
// TIM8
void timer8_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 1.使能定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	// 2.初始化定时器8
	// 168MHz / 16800 = 10KHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
	// 10KHz X 10000 = 1s
	TIM_TimeBaseInitStruct.TIM_Period = 20000; // 2000ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	
	// 3.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn; // TIM8
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// 4.使能定时器8中断
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	// 6.使能定时器中断
	TIM_Cmd(TIM8, ENABLE);
}
// TIM8
void TIM8_UP_TIM13_IRQHandler(void)
{
	//处理更新中断
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET){
		PEout(14) ^= 1;
		
		//清除中断标志
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
	}
}
