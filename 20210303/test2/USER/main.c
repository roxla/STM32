#include <myhead.h>

int main(void)
{
	uint8_t buf[5] = {0};
	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	usart1_init(115200);	// 初始化串口1
	dht11_set_io(GPIO_Mode_OUT);	//初始化温湿度模块
	
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
