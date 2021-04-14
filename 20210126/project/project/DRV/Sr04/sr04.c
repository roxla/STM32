#include <myhead.h>

void sr04_init(void)
{
	// PC7初始化为输出，PC9初始化为输入    
	GPIO_InitTypeDef GPIO_InitStruct;
	// 开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// 初始化GPIOC7 ---- Trig为输出功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	// 默认输出低电平
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	
	// 初始化GPIOC9 ---- Echo为输入功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}
// 测距
int get_distance(void)
{
	u32 retry = 0, count = 0;
	
	// Trig发送>10us高脉冲	
	TRIG = 1; // 拉高	
	delay_us(15); // 延时15us
	TRIG = 0; // 拉低
	
	// 等待ECHO脚拉高,超过60ms还没有变高，直接返回-1
	while(ECHO == 0)
	{
		retry++;
		delay_us(1);
		if(retry > 60000)
			return -1;
	}
	
	retry = 0;
	// 重置启动定时器
	TIM_SetCounter(TIM2,100000);
	TIM_Cmd(TIM2,ENABLE);
	
	// 计算高电平时间 ----- 等待变低同时计时
	while(ECHO)
	{
		retry++;
		delay_us(1);
		if(retry > 60000)
		{
			TIM_Cmd(TIM2,DISABLE);
			return -1;
		}
	}
	
	// 读取定时器的计数值，关闭定时器
	count = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2, DISABLE);
	// 计算距离并返回	
	return (100000 - count) / 58;
}
