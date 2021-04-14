#include <myhead.h>

void systick_init(void)
{
	// ѡ��ԭʼʱ��ΪHCLK/8 = 21MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
// ms��ʱ 1/21000000s  1/1000s  1ms = 21000��  (2^24-1)/21000 = 798
void my_delay_ms(u32 nms)
{
	// ������798ms
	if(nms > 798)
		nms = 798;
	// ���ó�ʼֵ
	SysTick->LOAD = 21000 * nms - 1;
	SysTick->VAL = 0; // ��ǰ����ֵΪ0
	// ����systick��ʼ��ʱ
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	// �ȴ�ʱ�䵽
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	// �ر�����systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
// us��ʱ
void my_delay_us(u32 nus)
{
	// ������798915us
	if(nus > 798915)
		nus = 798915;
	// ���ó�ʼֵ
	SysTick->LOAD = 21 * nus - 1;
	SysTick->VAL = 0; // ��ǰ����ֵΪ0
	// ����systick��ʼ��ʱ
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	// �ȴ�ʱ�䵽
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	// �ر�����systick
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
// s��ʱ
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
