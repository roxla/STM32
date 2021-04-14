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
	// 1.将当前PG9设置为输出模式，并置为高电平
	dht11_set_io(GPIO_Mode_OUT);
	DQ_OUT = 1;
	// 2.将PG9输出低电平，并延时至少18ms
	DQ_OUT = 0;
	delay_ms(20);
	// 3.PG9输出高电平，并延时20-40us（如30us）
	DQ_OUT = 1;
	delay_us(30);
	// 4.再将PG9设置为输入模式
	dht11_set_io(GPIO_Mode_IN);
	// 5.等待PG9变为低电平（最好添加超时处理）
	while(DQ_IN)
	{
		t++;
		delay_us(1);
		if(t>4000)
			return -1;
	}
	t = 0;
	// 6.等待PG9变为高电平（最好添加超时处理）
	while(DQ_IN == 0)
	{
		t++;
		delay_us(1);
		if(t>100)
			return -2;
	}
	t = 0;
	// 7.通过超时检测判断高电平合法性（等待变为低电平）
	while(DQ_IN)
	{
		t++;
		delay_us(1);
		if(t>100)
			return -3;
	}
	// 8.接收40bit（5字节）数据（0或1）
	for(n=0; n<5; n++)
	{
		data = 0;
		for(i=7; i>=0; i--)
		{
			// 8.1等待50us结束
			t = 0;
			while(DQ_IN == 0)
			{
				t++;
				delay_us(1);
				if(t>100)
					return -4;
			}
			// 8.2 开始判断数据0或1
			delay_us(40); // 40~60us左右
			if(DQ_IN)
			{
				data |= 1 << i;
				// 等待剩余高电平持续完毕
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
	// 9.校验数据（通过校验和）
	check_sum = (pbuf[0]+pbuf[1]+pbuf[2]+pbuf[3])&0xFF;
	if(check_sum != pbuf[4])
	{
		return -6;
	}
	
	return 1;
}
