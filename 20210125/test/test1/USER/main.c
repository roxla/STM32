#include <myhead.h>

int main(void)
{	
	systick_init();
	usart_init();
	
	while(1)
	{
//		uart1_puts("beybey");
		delay_s(1);
	}
	
	return 0;
}
