#include <myhead.h>

void delay_time(void)
{
	uint32_t i = 2000000;
	
	while(i--);
}

int main(void)
{
	led_init();
	
	while(1)
	{
		PFout(9) = ~PFout(9);
		delay_time();
		PFout(10) = ~PFout(10);
		delay_time();
		PEout(13) = ~PEout(13);
		delay_time();
		PEout(14) = ~PEout(14);
		delay_time();
	}
}
