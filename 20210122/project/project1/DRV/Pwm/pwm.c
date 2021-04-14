#include <myhead.h>

void pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 1.开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	// 2.初始化GPIOF9为复用功能 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // 输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // 推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // PF9
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	// 3.将TIM14的复用功能映射到PF9
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	// 4.初始化TIM14	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1; // 1MHz
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
	// 1MHz X 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	
	// 5.初始化PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM1模式
	TIM_OCInitStruct.TIM_Pulse = 50; // 比较计数值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //低电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	
	// 6.开始预装载和自动重装载
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	
	// 7.启动定时器
	TIM_Cmd(TIM14, ENABLE);
}
