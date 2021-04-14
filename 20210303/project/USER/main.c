#include <myhead.h>

int main(void)
{
	uint32_t distance = 0;
	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	usart1_init(115200);	// 初始化串口1
	sr04_init();
	
	while(1)
	{
		distance = sr04_get_distance();
		
		if(distance>=20 && distance<=4000)
		{
			printf("distance: %d mm\r\n", distance);
			if(distance>=200)
				led_all_switch(0);
			else if(distance>=150 && distance<200)
				led_all_switch(1);
			else if(distance>=100 && distance<150)
				led_all_switch(2);
			else if(distance>=50 && distance<100)
				led_all_switch(3);
			else if(distance<50)
				led_all_switch(4);
			delay_ms(500);
		}
		else
		{
			printf("distance error\r\n");
		}
	}
}
