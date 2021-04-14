#include <myhead.h>

int main(void)
{
	u8 temp = 0,humi = 0;
	led_init();
	usart_init();
	systick_init();
	delay_s(2); // 等待DHT11上电稳定
	
	// 测试DHT11
	if(DHT11_Check() == 0)
	{
		printf("dht11 work!\r\n");
	}
	
	while(1)
	{
		delay_s(1);
		if(dht11_read_data(&temp,&humi) == 0)
		{
			printf("temp = %d,humi = %d\r\n",temp,humi);
			if(temp > 28)
				LED0 = 0;
			else
				LED0 = 1;
			if(humi > 50)
				LED1 = 0;
			else
				LED1 = 1;
		}
	}
}
