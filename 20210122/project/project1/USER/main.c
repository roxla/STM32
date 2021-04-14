#include <myhead.h>

int main(void)
{
	//”≈œ»º∂2:2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	timer1_init();
	timer2_init();
	timer8_init();
	timer3_init();
	
	while(1);
}
