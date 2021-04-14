#include <sr04.h>

void sr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

uint32_t sr04_get_distance(void)
{
	uint32_t t = 0;
	// ����PC7��ƽ10us����������Trig
	TRIG = 1;
	delay_us(10);
	TRIG = 0;
	// �ȴ�PC9��ƽ���
	while(ECHO == 0)
	{
		t++;
		delay_us(9);
		if(retry > 10000)
			return -1;
	}
	retry = 0;
	// ��¼�ߵ�ƽ��ʱ��
	while(ECHO)
	{
		delay_us(9);
		t++;
		if(retry > 10000)
			return -1;
	}
	// �������
	t = t / 2 * 3;
	
	return t;
}
