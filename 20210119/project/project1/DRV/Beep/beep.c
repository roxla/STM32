#include <beep.h>

//延时
void delay(unsigned int ms)
{
	int i,j;
	
	for(i=0;i<ms;i++){
		for(j=0;j<5000;j++);
	}
}
// 初始化
void beep_init(void)
{
	// BEEP --- PF8
	
	// RCC
	RCC->AHB1ENR |= (0x1 << 5);
	// GPIO_INIT
	// 输出模式
	GPIOF->MODER &= ~(0x3 << 16);
	GPIOF->MODER |= (0x1 << 16);
	// 推挽输出
	GPIOF->OTYPER &= ~(0x1 << 8);
	// 输出速度 50M
	GPIOF->OSPEEDR &= ~(0x3 << 16);
	GPIOF->OSPEEDR |= (0x2 << 16);
	// 无上下拉
	GPIOF->PUPDR &= ~(0x3 << 16);
}
// 控制
void beep_switch(int state)
{
	// PF8高电平接通，低电平断开
	if(state)
	{
		GPIOF->BSRRL |= (0x1 << 8);
	}
	else
	{
		GPIOF->BSRRH |= (0x1 << 8);
	}
}
