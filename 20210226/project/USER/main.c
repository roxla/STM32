#include <myhead.h>

int main(void)
{
	led_init();
	
	SysTick_Config(SystemCoreClock);
	
	while(1)
	{
		LED1 = ~LED1;
		delay_s(1);
	}
}
