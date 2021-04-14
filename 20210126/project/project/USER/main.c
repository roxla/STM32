#include <myhead.h>

int main(void)
{
	// ���ȼ�2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	uart1_init();
	uart3_init();
	systick_init();
	beep_init();
	led_init();
	timer2_init();
	sr04_init();
	
	delay_s(2); // �ȴ�DHT11�ϵ��ȶ�
	//����DHT11
	if(DHT11_Check() == 0)
	{
		printf("dht11 work!\r\n");
	}
	
	while(1)
	{
		// 100ms����һ������
		parse_cmd();
		delay_ms(100);
	}
}
