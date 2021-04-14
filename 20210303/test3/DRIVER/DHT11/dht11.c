#include <dht11.h>

void dht11_set_io(GPIOMode_TypeDef IO)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = IO;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

int8_t dht11_read_data(uint8_t *pbuf)
{
	int i, n;
	uint32_t t = 0;
	uint8_t data, check_sum;
	// 1.����ǰPG9����Ϊ���ģʽ������Ϊ�ߵ�ƽ
	dht11_set_io(GPIO_Mode_OUT);
	DQ_OUT = 1;
	// 2.��PG9����͵�ƽ������ʱ����18ms
	DQ_OUT = 0;
	delay_ms(20);
	// 3.PG9����ߵ�ƽ������ʱ20-40us����30us��
	DQ_OUT = 1;
	delay_us(30);
	// 4.�ٽ�PG9����Ϊ����ģʽ
	dht11_set_io(GPIO_Mode_IN);
	// 5.�ȴ�PG9��Ϊ�͵�ƽ�������ӳ�ʱ����
	while(DQ_IN)
	{
		t++;
		delay_us(1);
		if(t>4000)
			return -1;
	}
	t = 0;
	// 6.�ȴ�PG9��Ϊ�ߵ�ƽ�������ӳ�ʱ����
	while(DQ_IN == 0)
	{
		t++;
		delay_us(1);
		if(t>100)
			return -2;
	}
	t = 0;
	// 7.ͨ����ʱ����жϸߵ�ƽ�Ϸ��ԣ��ȴ���Ϊ�͵�ƽ��
	while(DQ_IN)
	{
		t++;
		delay_us(1);
		if(t>100)
			return -3;
	}
	// 8.����40bit��5�ֽڣ����ݣ�0��1��
	for(n=0; n<5; n++)
	{
		data = 0;
		for(i=7; i>=0; i--)
		{
			// 8.1�ȴ�50us����
			t = 0;
			while(DQ_IN == 0)
			{
				t++;
				delay_us(1);
				if(t>100)
					return -4;
			}
			// 8.2 ��ʼ�ж�����0��1
			delay_us(40); // 40~60us����
			if(DQ_IN)
			{
				data |= 1 << i;
				// �ȴ�ʣ��ߵ�ƽ�������
				while(DQ_IN)
				{
					t++;
					delay_us(1);
					if(t>100)
						return -5;
				}
			}
		}
		pbuf[n] = data;
	}
	// 9.У�����ݣ�ͨ��У��ͣ�
	check_sum = (pbuf[0]+pbuf[1]+pbuf[2]+pbuf[3])&0xFF;
	if(check_sum != pbuf[4])
	{
		return -6;
	}
	
	return 1;
}
