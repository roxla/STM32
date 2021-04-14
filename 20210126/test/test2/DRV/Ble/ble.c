#include <myhead.h>

void ble_config(void)
{
	usart3_init();
	
	delay_ms(500);
	uart3_puts("AT+NAME=roxla_ble");
	delay_ms(500);
	uart3_puts("AT+RX");
}