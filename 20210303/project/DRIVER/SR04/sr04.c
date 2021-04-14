#include <sr04.h>

void sr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

uint32_t sr04_get_distance(void)
{
	uint32_t t = 0;
	// 拉高PC7电平10us，触发引脚Trig
	TRIG = 1;
	delay_us(10);
	TRIG = 0;
	// 等待PC9电平变高
	while(ECHO == 0)
	{
		t++;
		delay_us(9);
		if(retry > 10000)
			return -1;
	}
	retry = 0;
	// 记录高电平的时间
	while(ECHO)
	{
		delay_us(9);
		t++;
		if(retry > 10000)
			return -1;
	}
	// 计算距离
	t = t / 2 * 3;
	
	return t;
}
