#include "ble.h"

void ble_config(void)
{
	delay_ms(500);
	uart3_puts("AT+NAME=roxla_ble");
	delay_ms(500);
	uart3_puts("AT+RX");
	delay_ms(500);
}

// ��ʼ������3
void uart3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// ����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// GPIO��ʼ���ɸ��ù���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// ����ӳ��Ϊ���ڹ���
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// ��ʼ������ 9600 8N1
	USART_InitStruct.USART_BaudRate = 9600; // ������
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8λ����λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1λֹͣλ
	USART_InitStruct.USART_Parity = USART_Parity_No; // ��У��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �������ͺͽ��չ���
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ������
	USART_Init(USART3, &USART_InitStruct);
	// ��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn; // USART3
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // ��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // ��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	// ʹ�ܴ���
	USART_Cmd(USART3,ENABLE);
}

// ����һ���ַ�
void uart3_putc(char ch)
{
	// �ȴ���һ���������
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
	// ��������
	USART_SendData(USART3, ch);
}

// ����һ���ַ���
void uart3_puts(const char *s)
{
	while(*s)
		uart3_putc(*s++);
}

// ����3�жϴ�����
void USART3_IRQHandler(void)
{	
	//�����ж�
	OSIntEnter();
	
	// �ж��ǲ��Ǵ��ڽ����ж�
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// ����жϱ�־
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// ��������
		uart3_buf[uart3_cnt++] = USART_ReceiveData(USART3);
		//�ж������Ƿ�������(�����Ի��з�����)
		if(uart3_buf[uart3_cnt - 1] == '#' || uart3_cnt >= sizeof(uart3_buf))
			uart3_flag = 1;
	}
	
	//�˳��ж�
	OSIntExit();
}

//�����յ������
void parse_cmd(void)
{	
	char *token, buf[64];
	OS_ERR err;
	uint32_t i;
	u8 card_writebuf[16];
	
	// �����������ɲŴ���
	if(uart3_flag)
	{
		if(strstr((const char *)uart3_buf, "lock_on"))	// lock_on-123456#
		{
			token = strtok((char *)uart3_buf, "-#");
			token = strtok(NULL, "-#");
			if(strcmp(g_pwd,token) == 0)
				OSFlagPost(&g_flag_grp, FLAG_OLED_BLE_LOCKON + FLAG_LOCKON_FLAG, OS_OPT_POST_FLAG_SET, &err);	// ���ñ�־��
		}
		else if(strstr((const char *)uart3_buf, "search_record"))	// search_record-n# n����ѯ����
		{
			token = strtok((char *)uart3_buf, "-#");
			token = strtok(NULL, "-#");
			for(i = 0; i < atoi(token); i++)
			{
				memset(buf,0,64);
				w25qxx_read_data(i*64,(u8 *)buf,64);
				uart3_puts((const char *)buf);
				delay_ms(100);
			}
		}
		else if(strstr((const char *)uart3_buf, "card_set"))	// card_set-n# n������
		{
			token = strtok((char *)uart3_buf, "-#");
			token = strtok(NULL, "-#");
			
			memset(card_writebuf, 0, 16);
			for(i = 0; i < strlen(token); i++)
				card_writebuf[i] = token[i] - '0';	
			
			// ����ʵʱʱ����ʾ
			OSTaskSuspend(&app_task_tcb_rtc,&err);
			OSTaskSuspend(&app_task_tcb_dht11,&err);
			OSTaskSuspend(&app_task_tcb_rfid,&err);
			
			//�����ȴ�������
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			OLED_Clear();
			OLED_ShowString(28,25,"pls set card!",12);//6*12
			OLED_Refresh();
			//�����ͷŻ�����
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			
			MFRC522Set(card_writebuf);
			
			// ���ñ�־��
			OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
			//�ָ�ʵʱʱ��
			OSTaskResume(&app_task_tcb_rtc,&err);
			OSTaskResume(&app_task_tcb_dht11,&err);
			OSTaskResume(&app_task_tcb_rfid,&err);
		}
		else
		{
			//�Ƿ�����
			printf("unknow command = %s\n", (const char *)uart3_buf);
		}
		// ��־��0
		uart3_flag = 0;
		// ��ջ������͸���
		memset((char *)uart3_buf,0,sizeof(uart3_buf));
		uart3_cnt = 0;
	}
}
