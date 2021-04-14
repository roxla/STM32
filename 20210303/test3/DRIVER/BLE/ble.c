#include <ble.h>

void ble_config(void)
{
	usart3_init(9600);
	
	delay_ms(500);
	usart3_send_str("AT+NAME=roxla_ble");
	delay_ms(500);
	usart3_send_str("AT+RX");
	delay_ms(500);
	usart3_send_str("AT+SUUID=?");
}

// 串口3中断处理函数(接收信息)
void USART3_IRQHandler(void)
{
	u8 data = 0;
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// 接收数据
		data = USART_ReceiveData(USART3);
		// 将读取到的数据发送给PC
		USART_SendData(USART1, data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//	清空标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
