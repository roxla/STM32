#include <stm32f4xx.h>
#include <beep.h>
#include <led.h>
#include <key.h>
#include <sys.h>

int main(void)
{
	int flag = 0;
	
	key_init();
	led_init();
	
	while(1)
	{
		if(PAin(0) == 0)
		{
			if(flag == 0)
			{
				led_onoff(1, 1);
				flag = 1;
			}
			else
			{
				led_onoff(1, 0);
				flag = 0;
			}
			
		}
		else
			delay(50);
	}
	
	return 0;
}
