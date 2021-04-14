#include <stm32f4xx.h>

#define UINT unsigned int

void delay(UINT ms)
{
	int i,j;
	
	for(i = 0; i < ms; i++)
		for(j = 0; j < 5000; j++);
}

void led1()
{
	// GPIO初始化
	// 输出模式
	GPIOF->MODER &= ~(0x3 << 18);
	GPIOF->MODER |= (0x1 << 18);
	// 推挽输出
	GPIOF->OTYPER &= ~(0x1 << 9);
	// 输出速度 50M
	GPIOF->OSPEEDR &= ~(0x3 << 18);
	GPIOF->OSPEEDR |= (0x2 << 18);
	// 无上下拉
	GPIOF->PUPDR &= ~(0x3 << 18);
}

void led2()
{
	// 输出模式	
	GPIOF->MODER &= ~(0x3 << 20);
	GPIOF->MODER |= (0x1 << 20);
	// 推挽输出
	GPIOF->OTYPER &= ~(0x1 << 10);
	// 输出速度 50M
	GPIOF->OSPEEDR &= ~(0x3 << 20);
	GPIOF->OSPEEDR |= (0x2 << 20);
	// 无上下拉
	GPIOF->PUPDR &= ~(0x3 << 20);
}

void led3()
{
	// 输出模式
	GPIOE->MODER &= ~(0x3 << 26);
	GPIOE->MODER |= (0x1 << 26);
	// 推挽输出
	GPIOE->OTYPER &= ~(0x1 << 13);
	// 输出速度 50M
	GPIOE->OSPEEDR &= ~(0x3 << 26);
	GPIOE->OSPEEDR |= (0x2 << 26);
	// 无上下拉
	GPIOE->PUPDR &= ~(0x3 << 26);
}

void led4()
{
	// 输出模式
	GPIOE->MODER &= ~(0x3 << 28);
	GPIOE->MODER |= (0x1 << 28);
	// 推挽输出
	GPIOE->OTYPER &= ~(0x1 << 14);
	// 输出速度 50M
	GPIOE->OSPEEDR &= ~(0x3 << 28);
	GPIOE->OSPEEDR |= (0x2 << 28);
	// 无上下拉
	GPIOE->PUPDR &= ~(0x3 << 28);
}

int main()
{
	// 开启GPIOF时钟 RCC_AHB1ENR的第5位置1
	RCC->AHB1ENR |= (0x1 << 5);
	// 开启GPIOE时钟 RCC_AHB1ENR的第4位置1
	RCC->AHB1ENR |= (0x1 << 4);
	
	// GPIO初始化
	led1();
	led2();
	led3();
	led4();
	
	// 闪烁
//	while(1)
//	{
//		// 亮
//		GPIOF->BSRRH |= (0x1 << 9);
//		GPIOF->BSRRH |= (0x1 << 10);
//		GPIOE->BSRRH |= (0x1 << 13);
//		GPIOE->BSRRH |= (0x1 << 14);
//		// 延时
//		delay(500);
//		
//		// 灭
//		GPIOF->BSRRL |= (0X1 << 9);
//		GPIOF->BSRRL |= (0x1 << 10);
//		GPIOE->BSRRL |= (0x1 << 13);
//		GPIOE->BSRRL |= (0x1 << 14);
//		// 延时
//		delay(500);
//	}
	// 灭
	GPIOF->BSRRL |= (0X1 << 9);
	GPIOF->BSRRL |= (0x1 << 10);
	GPIOE->BSRRL |= (0x1 << 13);
	GPIOE->BSRRL |= (0x1 << 14);
	
	
	return 0;
}