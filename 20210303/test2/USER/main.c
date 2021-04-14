#include <myhead.h>

int main(void)
{
	uint8_t buf[5] = {0};
	// ѡ���ж����ȼ�2�飨4����ռ���ȼ���4����Ӧ���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	usart1_init(115200);	// ��ʼ������1
	dht11_set_io(GPIO_Mode_OUT);	//��ʼ����ʪ��ģ��
	
	while(1)
	{
		if(dht11_read_data(buf))
		{
			printf("T:%d.%d H:%d.%d \r\n", buf[2], buf[3], buf[0], buf[1]);
		}
		
		delay_s(6);
		LED1=~LED1;
	}
}
