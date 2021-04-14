#include <myhead.h>

void systick_init(void)
{
	// 选择原始时钟为HCLK/8 = 21MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
// ms延时 1/21000000s  1/1000s  1ms = 21000次  (2^24-1)/21000 = 798
void my_delay_ms(u32 nms)
{
	// 最大计算798ms
	if(nms > 798)
		nms = 798;
	// 设置初始值
	SysTick->LOAD = 21000 * nms - 1;
	SysTick->VAL = 0; // 当前计数值为0
	// 启动systick开始计时
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	// 等待时间到
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	// 关闭启动systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
// us延时
void my_delay_us(u32 nus)
{
	// 最大计算798915us
	if(nus > 798915)
		nus = 798915;
	// 设置初始值
	SysTick->LOAD = 21 * nus - 1;
	SysTick->VAL = 0; // 当前计数值为0
	// 启动systick开始计时
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	// 等待时间到
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	// 关闭启动systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
// s延时
void my_delay_s(u32 s)
{
	if(s <= 0)
		s = 1;
	while(s--)
	{
		my_delay_ms(400);
		my_delay_ms(400);
	}
}
