#include <myhead.h>

void delay(unsigned int ms)
{
	uint32_t i, j;
	for(i=0; i < ms; i++)
		for(j=0; j < 5000; j++);
}

int main(void)
{
	led_init();
	
	while(1)
	{
		led_control(1, 1); // LED1 ON
		delay(500);
		led_control(1, 0); // LED1 OFF
		led_control(2, 1); // LED2 ON
		delay(500);
		led_control(2, 0); // LED2 OFF
		led_control(3, 1); // LED3 ON
		delay(500);
		led_control(3, 0); // LED3 OFF
		led_control(4, 1); // LED4 ON
		delay(500);
		led_control(4, 0); // LED4 OFF
	}
}
