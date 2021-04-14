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

void led1_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* ����ʱ�����ӵ��������� */ 
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
}

void led3_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* ����ʱ�����ӵ��������� */ 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
}

void led4_af_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* ����ʱ�����ӵ��������� */ 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
}

void led_all_switch(uint32_t n)
{
	switch(n)
	{
		case 0:
			LED1 = 1;
			LED2 = 1;
			LED3 = 1;
			LED4 = 1;
			break;
		case 1:
			LED1 = 0;
			LED2 = 1;
			LED3 = 1;
			LED4 = 1;
			break;
		case 2:
			LED1 = 0;
			LED2 = 0;
			LED3 = 1;
			LED4 = 1;
			break;
		case 3:
			LED1 = 0;
			LED2 = 0;
			LED3 = 0;
			LED4 = 1;
			break;
		case 4:
			LED1 = 0;
			LED2 = 0;
			LED3 = 0;
			LED4 = 0;
			break;
	}
}
