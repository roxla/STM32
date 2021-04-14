#include <stm32f4xx.h>
#include <led.h>

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;	//���ź�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// ��ʼ����ȫ��
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}

void led_control(int num, int state)
{
	if(num == 1)
	{
		if(state != 0)
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		else
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
	}
	else if(num == 2)
	{
		if(state != 0)
			GPIO_ResetBits(GPIOF, GPIO_Pin_10);
		else
			GPIO_SetBits(GPIOF, GPIO_Pin_10);
	}
	else if(num == 3)
	{
		if(state != 0)
			GPIO_ResetBits(GPIOE, GPIO_Pin_13);
		else
			GPIO_SetBits(GPIOE, GPIO_Pin_13);
	}
	else if(num == 4)
	{
		if(state != 0)
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
		else
			GPIO_SetBits(GPIOE, GPIO_Pin_14);
	}
}
