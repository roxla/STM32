#include <myhead.h>

int main(void)
{	
	int distance = 0;
	beep_init();
	led_init();
	sr04_init();
	usart_init();
	
	while(1)
	{
		distance = get_distance();
		if(distance > 150)
		{
			led_onoff(1, 1);
			beep_switch(0);
		}
		else if(distance < 30)
		{
			led_onoff(1, 0);
			beep_switch(1);
		}
		else
		{
			led_onoff(1, 0);
			beep_switch(0);
		}
		
		delay_s(1);
	}
	
	return 0;
}
