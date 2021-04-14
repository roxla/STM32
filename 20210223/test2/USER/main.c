#include <stm32f4xx.h>	// STM32F4系列芯片头文件

void delay(unsigned int num)
{
	uint32_t i, j;
	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 5000; j++);
	}
}

int main(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 使能对应GPIO的RCC时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* 配置GPIO并初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	while(1)
	{
//		GPIO_ResetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
		delay(500);
		GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
		delay(500);
	}
}
