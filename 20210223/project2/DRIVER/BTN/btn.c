#include <stm32f4xx.h>
#include <btn.h>

void btn_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4;	//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
