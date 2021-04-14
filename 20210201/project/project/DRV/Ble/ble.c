#include <myhead.h>

void ble_config(void)
{
	my_delay_ms(500);
	uart3_puts("AT+NAME=roxla_ble");
	my_delay_ms(500);
	uart3_puts("AT+RX");
	my_delay_ms(500);
}
