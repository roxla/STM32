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

// ����3�жϴ�����(������Ϣ)
void USART3_IRQHandler(void)
{
	u8 data = 0;
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// ��������
		data = USART_ReceiveData(USART3);
		// ����ȡ�������ݷ��͸�PC
		USART_SendData(USART1, data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//	��ձ�־λ
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
