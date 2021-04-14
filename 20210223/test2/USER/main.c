#include <stm32f4xx.h>	// STM32F4ϵ��оƬͷ�ļ�

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
	
	/* ʹ�ܶ�ӦGPIO��RCCʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* ����GPIO����ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	while(1)
	{
//		GPIO_ResetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
		delay(500);
		GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
		delay(500);
	}
}
