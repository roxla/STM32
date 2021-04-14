#include <myhead.h>

//��ʼ��
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//PF9 PF10
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	led_onoff(1, 0);
	led_onoff(2, 0);
}

//��������
void led_onoff(int led_num,int state)
{
	switch(led_num){
		case 1:
			if(state)
				GPIO_ResetBits(GPIOF,GPIO_Pin_9); // ��
			else
				GPIO_SetBits(GPIOF,GPIO_Pin_9); // ��
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
