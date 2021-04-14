#include <myhead.h>

int main(void)
{
	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart1_init(115200);
	ble_config();
	led_init();
	
	while(1)
	{

	}
}
