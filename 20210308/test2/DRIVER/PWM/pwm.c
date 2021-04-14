#include <pwm.h>

void tim1_pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* 使能定时器3时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* 配置定时器 */
	TIM_TimeBaseStructure.TIM_Prescaler = 1680-1;	// 100000Hz=100kHz
	TIM_TimeBaseStructure.TIM_Period = 1000-1;		//周期1000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//（STM32F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* 定时器1的PWM模式配置：通道3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;		// 初始比较值（与周期共同决定了占空比）
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// 初始极性：高电平
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);	// 通道3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	// 使能通道3重装载
	
	/* 定时器1的PWM模式配置：通道4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStructure.TIM_Pulse = 50;		// 初始比较值（与周期共同决定了占空比）
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// 初始极性：高电平
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);	// 通道4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);	// 使能通道4重装载

	// 使能定时器1ARR重装载
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	// 打开PWM输出
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	/* 使能定时器1 */
	TIM_Cmd(TIM1, ENABLE);
}

void tim13_pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* 使能定时器13时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	
	/* 配置定时器 */
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;	// 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//（STM32F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
	/* 定时器14的PWM模式配置：通道1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		// 初始比较值（与周期共同决定了占空比）
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// 初始极性：高电平
	TIM_OC1Init(TIM13, &TIM_OCInitStructure);	// 通道1
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);	// 使能通道1重装载

	// 使能定时器14ARR重装载
	TIM_ARRPreloadConfig(TIM13, ENABLE);

	/* 使能定时器14 */
	TIM_Cmd(TIM13, ENABLE);
}

void tim14_pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* 使能定时器14时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	/* 配置定时器 */
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;	// 100000Hz=100kHz
	TIM_TimeBaseStructure.TIM_Period = 1000-1;		//周期1000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//（STM32F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	/* 定时器14的PWM模式配置：通道1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;		// 初始比较值（与周期共同决定了占空比）
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// 初始极性：高电平
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);	// 通道1
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);	// 使能通道1重装载

	// 使能定时器14ARR重装载
	TIM_ARRPreloadConfig(TIM14, ENABLE);

	/* 使能定时器14 */
	TIM_Cmd(TIM14, ENABLE);
}
