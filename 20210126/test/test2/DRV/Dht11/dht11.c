#include <myhead.h>

//配置单总线是输入还是输出模式
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
// 测试DHT11是否有响应，返回0正常，非0表示无响应
u8 DHT11_Check(void)
{
	u32 retry = 0;
	
	// 输出模式
	dht11_set_io(GPIO_Mode_OUT);
	
	// 发送18ms低电平起始信号
	DQ_OUT = 0;
	delay_ms(20);//保持20ms
	
	// 拉高等待20~40us
	DQ_OUT = 1;
	delay_us(30);
	
	// 将总线设置为输入模式，等待DHT11的响应
	dht11_set_io(GPIO_Mode_IN);
	while(DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// 如果模块没有响应，直接退出
	if(retry >= 100)
		return 1;
		
	retry = 0;
	// 80us会再次拉高
	while(!DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// 如果模块没有拉高，直接退出
	if(retry >= 100)
		return 1;
	
	return 0;
}
// DHT11读一位数据
u8 dht11_read_bit(void)
{
	u32 retry = 0;
	
	// 等待进入低电平，80us
	while(DQ_IN && retry < 100)
	{	
		retry++;
		delay_us(1);
	}
	// 如果模块没有响应，直接退出
	if(retry >= 100)
		return 100;
		
	retry = 0;
	// 等待变高，50us
	while(!DQ_IN && retry < 100)
	{
		retry++;
		delay_us(1);
	}
	if(retry>=100) // 没有拉低，退出
		return 100;
	
	// 延时40us,此时低电平 ----- 0  高电平 ------ 1
	delay_us(40);
	return DQ_IN;
}
//读一个字节的数据，先发送高位
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
//读取温湿度数据
//返回0表示成功，非0表示失败
//temp和humi是输出参数
u8 dht11_read_data(u8 *temp,u8 *humi)
{
	u8 i, buf[5];
	//起始信号
	if(DHT11_Check() == 0)
	{
		// 读取40位数据
		for(i = 0; i < 5; i++)
		{
			buf[i] = dht11_read_byte();
		}
		//判断校验和
		if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
		{
			*temp = buf[2];
			*humi = buf[0];
			
			return 0;
		}
	}
	
	return 1;
}
