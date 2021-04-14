#include <beep.h>

//��ʱ
void delay(unsigned int ms)
{
	int i,j;
	
	for(i=0;i<ms;i++){
		for(j=0;j<5000;j++);
	}
}
// ��ʼ��
void beep_init(void)
{
	// BEEP --- PF8
	
	// RCC
	RCC->AHB1ENR |= (0x1 << 5);
	// GPIO_INIT
	// ���ģʽ
	GPIOF->MODER &= ~(0x3 << 16);
	GPIOF->MODER |= (0x1 << 16);
	// �������
	GPIOF->OTYPER &= ~(0x1 << 8);
	// ����ٶ� 50M
	GPIOF->OSPEEDR &= ~(0x3 << 16);
	GPIOF->OSPEEDR |= (0x2 << 16);
	// ��������
	GPIOF->PUPDR &= ~(0x3 << 16);
}
// ����
void beep_switch(int state)
{
	// PF8�ߵ�ƽ��ͨ���͵�ƽ�Ͽ�
	if(state)
	{
		GPIOF->BSRRL |= (0x1 << 8);
	}
	else
	{
		GPIOF->BSRRH |= (0x1 << 8);
	}
}
