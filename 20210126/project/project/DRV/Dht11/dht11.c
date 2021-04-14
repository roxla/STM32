#include <myhead.h>

//���õ����������뻹�����ģʽ
void dht11_set_io(GPIOMode_TypeDef IO)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = IO;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // PG9
	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}
// ����DHT11�Ƿ�����Ӧ������0��������0��ʾ����Ӧ
u8 DHT11_Check(void)
{
	u32 retry = 0;
	
	// ���ģʽ
	dht11_set_io(GPIO_Mode_OUT);
	
	// ����18ms�͵�ƽ��ʼ�ź�
	DQ_OUT = 0;
	delay_ms(20);//����20ms
	
	// ���ߵȴ�20~40us
	DQ_OUT = 1;
	delay_us(30);
	
	// ����������Ϊ����ģʽ���ȴ�DHT11����Ӧ
	dht11_set_io(GPIO_Mode_IN);
	while(DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// ���ģ��û����Ӧ��ֱ���˳�
	if(retry >= 100)
		return 1;
		
	retry = 0;
	// 80us���ٴ�����
	while(!DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// ���ģ��û�����ߣ�ֱ���˳�
	if(retry >= 100)
		return 1;
	
	return 0;
}
// DHT11��һλ����
u8 dht11_read_bit(void)
{
	u32 retry = 0;
	
	// �ȴ�����͵�ƽ��80us
	while(DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// ���ģ��û����Ӧ��ֱ���˳�
	if(retry >= 100)
		return 100;
		
	retry = 0;
	// �ȴ���ߣ�50us
	while(!DQ_IN && retry < 100)
	{
		retry++;
		delay_us(1);
	}
	if(retry>=100) // û�����ͣ��˳�
		return 100;
	
	// ��ʱ40us,��ʱ�͵�ƽ ----- 0  �ߵ�ƽ ------ 1
	delay_us(40);
	return DQ_IN;
}
//��һ���ֽڵ����ݣ��ȷ��͸�λ
u8 dht11_read_byte(void)
{
	u8 i, dat = 0;
	for(i = 0; i < 8; i++)
	{
//		if(dht11_read_bit() >= 100)
//			return 
		dat |= dht11_read_bit() << (7-i);
	}
	
	return dat;
}
//��ȡ��ʪ������
//����0��ʾ�ɹ�����0��ʾʧ��
//temp��humi���������
u8 dht11_read_data(u8 *temp,u8 *humi)
{
	u8 i, buf[5];
	//��ʼ�ź�
	if(DHT11_Check() == 0)
	{
		// ��ȡ40λ����
		for(i = 0; i < 5; i++)
		{
			buf[i] = dht11_read_byte();
		}
		//�ж�У���
		if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
		{
			*temp = buf[2];
			*humi = buf[0];
			
			return 0;
		}
	}
	
	return 1;
}
