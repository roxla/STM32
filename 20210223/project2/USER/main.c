#include <myhead.h>

int main(void)
{
	led_init();
	beep_init();
	btn_init();
	
	while(1)
	{
		// btn1
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			beep_switch(1);
		else
			beep_switch(0);
		
		// btn2
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
			led_control(1, 1); // ²Î1 led±àºÅ£¬²Î2 ledÁÁÃð×´Ì¬
		else
			led_control(1, 0);
			
		// btn3
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{
			led_control(1, 1);
			led_control(2, 1);
		}
		else
		{
			led_control(1, 0);
			led_control(2, 0);
		}
			
		// btn4
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			led_control(1, 1);
			led_control(2, 1);
			led_control(3, 1);
			led_control(4, 1);
		}
		else
		{
			led_control(1, 0);
			led_control(2, 0);
			led_control(3, 0);
			led_control(4, 0);
		}
	}
}
