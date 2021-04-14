#include <key.h>

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // 输出模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; // PA0
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
