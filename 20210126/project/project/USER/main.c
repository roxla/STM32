#include <myhead.h>

int main(void)
{
	// 优先级2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	uart1_init();
	uart3_init();
	systick_init();
	beep_init();
	led_init();
	timer2_init();
	sr04_init();
	
	delay_s(2); // 等待DHT11上电稳定
	//测试DHT11
	if(DHT11_Check() == 0)
	{
		printf("dht11 work!\r\n");
	}
	
	while(1)
	{
		// 100ms处理一次命令
		parse_cmd();
		delay_ms(100);
	}
}
