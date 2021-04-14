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
		//	key1
		if(PAin(0) == 0)
		{
			PFout(8) = 1;
		}
		//	key2
		else if(PEin(2) == 0)
		{
			PFout(9) = ~PFout(9);
			delay_time();
		}
		//	key3
		else if(PEin(3) == 0)
		{
			PFout(9) = ~PFout(9);
			PFout(10) = ~PFout(10);
			PEout(13) = ~PEout(13);
			delay_time();
		}
		//	key4
		else if(PEin(4) == 0)
		{
			PFout(9) = ~PFout(9);
			PFout(10) = ~PFout(10);
			PEout(13) = ~PEout(13);
			PEout(14) = ~PEout(14);
			delay_time();
		}
		else
		{
			PFout(8) = 0;
			PFout(9) = 1;
			PFout(10) = 1;
			PEout(13) = 1;
			PEout(14) = 1;
		}
	}
}
