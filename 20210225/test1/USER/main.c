#include <myhead.h>

void delay_time(void)
{
	uint32_t i = 10000000;
	
	while(i--);
}

int main(void)
{
	led_init();
	beep_init();
	btn_init();
	
	while(1)
	{
		if(KEY1 == 0)
		{
			BEEP = 1;
		}
		else if(KEY2 == 0)
		{
			LED1 = ~LED1;
			delay_time();
		}
		else if(KEY3 == 0)
		{
			LED1 = ~LED1;
			LED2 = ~LED2;
			LED3 = ~LED3;
			delay_time();
		}
		else if(KEY4 == 0)
		{
			LED1 = ~LED1;
			LED2 = ~LED2;
			LED3 = ~LED3;
			LED4 = ~LED4;
			delay_time();
		}
		else
		{
			BEEP = 0;
			LED1 = 1;
			LED2 = 1;
			LED3 = 1;
			LED4 = 1;
		}
	}
}
