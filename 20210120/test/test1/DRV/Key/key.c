#include <key.h>

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // ���ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; // PA0
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
