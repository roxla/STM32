#include <led.h>

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;	//引脚号
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// 初始设置全灭
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}

void led1_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* 将定时器连接到复用引脚 */ 
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
}

void led3_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* 将定时器连接到复用引脚 */ 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
}

void led4_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* 将定时器连接到复用引脚 */ 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
}
