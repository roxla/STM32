#include <myhead.h>

//初始化
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//PF9 PF10
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	led_onoff(1, 0);
	led_onoff(2, 0);
}

//控制亮灭
void led_onoff(int led_num,int state)
{
	switch(led_num){
		case 1:
			if(state)
				GPIO_ResetBits(GPIOF,GPIO_Pin_9); // 亮
			else
				GPIO_SetBits(GPIOF,GPIO_Pin_9); // 灭
			break;
		case 2:
			if(state)
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
			else
				GPIO_SetBits(GPIOF,GPIO_Pin_10);
			break;
		case 3:
			break;
		case 4:
			break;
	}
}
