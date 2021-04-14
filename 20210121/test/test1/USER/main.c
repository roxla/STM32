#include <myhead.h>

int main(void)
{
	led_init();
	
	while(1)
	{
		led_onoff(1,1);
		delay_s(1);
		led_onoff(1,0);
		delay_s(1);
	}
}
