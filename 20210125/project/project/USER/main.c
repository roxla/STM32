#include <myhead.h>

int main(void)
{	
	int distance = 0;
	sr04_init();
	usart_init();
	
	while(1)
	{
		distance = get_distance();
		printf("distance = %d\r\n",distance);
		
		delay_s(1);
	}
	
	return 0;
}
