#include <myhead.h>

int main(void)
{
	// ѡ���ж����ȼ�2�飨4����ռ���ȼ���4����Ӧ���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart1_init(115200);
	ble_config();
	led_init();
	
	while(1)
	{

	}
}
