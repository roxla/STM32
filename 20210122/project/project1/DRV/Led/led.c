#include <myhead.h>

//初始化
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// 开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// 初始化GPIOF
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//PF9 PF10
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	// 初始化GPIOE
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;//PF9 PF10
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	led_onoff(1, 0);
	led_onoff(2, 0);
	led_onoff(3, 0);
	led_onoff(4, 0);
}
//控制亮灭
void led_onoff(int led_num,int state)
{
	switch(led_num){
		case 1:
			if(state)
				GPIO_ResetBits(GPIOF, GPIO_Pin_9); // 亮
			else
				GPIO_SetBits(GPIOF, GPIO_Pin_9); // 灭
			break;
		case 2:
			if(state)
				GPIO_ResetBits(GPIOF, GPIO_Pin_10);
			else
				GPIO_SetBits(GPIOF, GPIO_Pin_10);
			break;
		case 3:
			if(state)
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
			else
				GPIO_SetBits(GPIOE, GPIO_Pin_13);
			break;
		case 4:
			if(state)
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			else
				GPIO_SetBits(GPIOE, GPIO_Pin_14);
			break;
	}
}
