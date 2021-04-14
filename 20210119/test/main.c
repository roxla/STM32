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
	// GPIO��ʼ��
	// ���ģʽ
	GPIOF->MODER &= ~(0x3 << 18);
	GPIOF->MODER |= (0x1 << 18);
	// �������
	GPIOF->OTYPER &= ~(0x1 << 9);
	// ����ٶ� 50M
	GPIOF->OSPEEDR &= ~(0x3 << 18);
	GPIOF->OSPEEDR |= (0x2 << 18);
	// ��������
	GPIOF->PUPDR &= ~(0x3 << 18);
}

void led2()
{
	// ���ģʽ	
	GPIOF->MODER &= ~(0x3 << 20);
	GPIOF->MODER |= (0x1 << 20);
	// �������
	GPIOF->OTYPER &= ~(0x1 << 10);
	// ����ٶ� 50M
	GPIOF->OSPEEDR &= ~(0x3 << 20);
	GPIOF->OSPEEDR |= (0x2 << 20);
	// ��������
	GPIOF->PUPDR &= ~(0x3 << 20);
}

void led3()
{
	// ���ģʽ
	GPIOE->MODER &= ~(0x3 << 26);
	GPIOE->MODER |= (0x1 << 26);
	// �������
	GPIOE->OTYPER &= ~(0x1 << 13);
	// ����ٶ� 50M
	GPIOE->OSPEEDR &= ~(0x3 << 26);
	GPIOE->OSPEEDR |= (0x2 << 26);
	// ��������
	GPIOE->PUPDR &= ~(0x3 << 26);
}

void led4()
{
	// ���ģʽ
	GPIOE->MODER &= ~(0x3 << 28);
	GPIOE->MODER |= (0x1 << 28);
	// �������
	GPIOE->OTYPER &= ~(0x1 << 14);
	// ����ٶ� 50M
	GPIOE->OSPEEDR &= ~(0x3 << 28);
	GPIOE->OSPEEDR |= (0x2 << 28);
	// ��������
	GPIOE->PUPDR &= ~(0x3 << 28);
}

int main()
{
	// ����GPIOFʱ�� RCC_AHB1ENR�ĵ�5λ��1
	RCC->AHB1ENR |= (0x1 << 5);
	// ����GPIOEʱ�� RCC_AHB1ENR�ĵ�4λ��1
	RCC->AHB1ENR |= (0x1 << 4);
	
	// GPIO��ʼ��
	led1();
	led2();
	led3();
	led4();
	
	// ��˸
//	while(1)
//	{
//		// ��
//		GPIOF->BSRRH |= (0x1 << 9);
//		GPIOF->BSRRH |= (0x1 << 10);
//		GPIOE->BSRRH |= (0x1 << 13);
//		GPIOE->BSRRH |= (0x1 << 14);
//		// ��ʱ
//		delay(500);
//		
//		// ��
//		GPIOF->BSRRL |= (0X1 << 9);
//		GPIOF->BSRRL |= (0x1 << 10);
//		GPIOE->BSRRL |= (0x1 << 13);
//		GPIOE->BSRRL |= (0x1 << 14);
//		// ��ʱ
//		delay(500);
//	}
	// ��
	GPIOF->BSRRL |= (0X1 << 9);
	GPIOF->BSRRL |= (0x1 << 10);
	GPIOE->BSRRL |= (0x1 << 13);
	GPIOE->BSRRL |= (0x1 << 14);
	
	
	return 0;
}