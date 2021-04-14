#include <stm32f4xx.h>
#include <btn.h>

void btn_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4;	//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
