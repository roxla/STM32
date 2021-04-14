#include <myhead.h>

void pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// 1.开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // LED4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // TIM1_CH4
	
	// 2.初始化GPIOF9为复用功能 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // 输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // 推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14; // PE14
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// 3.将TIM1的复用功能映射到PE14
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	// 4.初始化TIM1
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; // 向下计数
	// 1MHz X 1000 = 1ms
	TIM_TimeBaseInitStruct.TIM_Period = 1000; // 100ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	// 5.初始化PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // PWM1模式
	TIM_OCInitStruct.TIM_Pulse = 50; // 比较计数值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //低电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	// 6.开始预装载和自动重装载
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	// 7.启动定时器
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}
