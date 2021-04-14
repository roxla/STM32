#include <myhead.h>

void ble_config(void)
{
	delay_ms(500);
	uart3_puts("AT+NAME=roxla_ble");
	delay_ms(500);
	uart3_puts("AT+RX");
	delay_ms(500);
}
