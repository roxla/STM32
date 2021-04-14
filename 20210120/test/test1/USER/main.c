#include <stm32f4xx.h>
#include <beep.h>
#include <led.h>
#include <key.h>
#include <sys.h>

int main(void)
{
	key_init();
	led_init();
	
	while(1)
	{
//		int i = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
//		if(i == 0)
//			led_onoff(1, 1);
//		else
//			led_onoff(1, 0);

//		if(GPIOA->IDR & 0x1)
//			led_onoff(1, 0);
//		else
//			led_onoff(1, 1);

		if(PAin(0) == 0)
			led_onoff(1, 1);
		else
			led_onoff(1, 0);
	}
	
	return 0;
}
