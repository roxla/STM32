#include <myhead.h>

int main(void)
{
	// ”≈œ»º∂2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	usart1_init();
	systick_init();
	
	ble_config();
	while(1)
	{
		
	}
}
