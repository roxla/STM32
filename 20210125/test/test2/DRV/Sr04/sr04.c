#include <myhead.h>

void sr04_init(void)
{
	// PC7��ʼ��Ϊ�����PC9��ʼ��Ϊ����    
	GPIO_InitTypeDef GPIO_InitStruct;
	// ����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// ��ʼ��GPIOC7 ---- TrigΪ�������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	// Ĭ������͵�ƽ
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	
	// ��ʼ��GPIOC9 ---- EchoΪ���빦��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}
// ���
int get_distance(void)
{
	u32 retry = 0;
	
	// Trig����>10us������	
	TRIG = 1; // ����	
	delay_us(15); // ��ʱ15us	
	TRIG = 0; // ����
	
	// �ȴ�ECHO������,����60ms��û�б�ߣ�ֱ�ӷ���-1
	while(ECHO == 0)
	{
		retry++;
		delay_us(1);
		if(retry > 60000)
			return -1;
	}
	
	retry = 0;
	// ����ߵ�ƽʱ�� ----- �ȴ����ͬʱ��ʱ
	while(ECHO)
	{
		retry++;
		delay_us(10);
		if(retry > 6000)
			return -1;
	}
	
	//������벢����	
	return retry *10 / 58;
}
