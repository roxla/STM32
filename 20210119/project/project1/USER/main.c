#include <stm32f4xx.h>
#include <beep.h>

int main(void)
{
	beep_init();
	
	while(1)
	{
		beep_switch(1);
		delay(1000);
		beep_switch(0);
		delay(1000);
		
//		break;
	}
	
	return 0;
}
