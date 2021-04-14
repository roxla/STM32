#include "ble.h"

void ble_config(void)
{
	delay_ms(500);
	uart3_puts("AT+NAME=roxla_ble");
	delay_ms(500);
	uart3_puts("AT+RX");
	delay_ms(500);
}

// 初始化串口3
void uart3_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// GPIO初始化成复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// 复用映射为串口功能
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// 初始化串口 9600 8N1
	USART_InitStruct.USART_BaudRate = 9600; // 波特率
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1位停止位
	USART_InitStruct.USART_Parity = USART_Parity_No; // 无校验
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 开启发送和接收功能
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
	USART_Init(USART3, &USART_InitStruct);
	// 初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn; // USART3
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	// 使能串口接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	// 使能串口
	USART_Cmd(USART3,ENABLE);
}

// 发送一个字符
void uart3_putc(char ch)
{
	// 等待上一个发送完成
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
	// 发送数据
	USART_SendData(USART3, ch);
}

// 发送一个字符串
void uart3_puts(const char *s)
{
	while(*s)
		uart3_putc(*s++);
}

// 串口3中断处理函数
void USART3_IRQHandler(void)
{	
	//进入中断
	OSIntEnter();
	
	// 判断是不是串口接收中断
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// 清除中断标志
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// 接收数据
		uart3_buf[uart3_cnt++] = USART_ReceiveData(USART3);
		//判断数据是否接收完成(数据以换行符结束)
		if(uart3_buf[uart3_cnt - 1] == '#' || uart3_cnt >= sizeof(uart3_buf))
			uart3_flag = 1;
	}
	
	//退出中断
	OSIntExit();
}

//处理收到命令函数
void parse_cmd(void)
{	
	char *token, buf[64];
	OS_ERR err;
	uint32_t i;
	u8 card_writebuf[16];
	
	// 当命令接收完成才处理
	if(uart3_flag)
	{
		if(strstr((const char *)uart3_buf, "lock_on"))	// lock_on-123456#
		{
			token = strtok((char *)uart3_buf, "-#");
			token = strtok(NULL, "-#");
			if(strcmp(g_pwd,token) == 0)
				OSFlagPost(&g_flag_grp, FLAG_OLED_BLE_LOCKON + FLAG_LOCKON_FLAG, OS_OPT_POST_FLAG_SET, &err);	// 设置标志组
		}
		else if(strstr((const char *)uart3_buf, "search_record"))	// search_record-n# n：查询条数
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
		else if(strstr((const char *)uart3_buf, "card_set"))	// card_set-n# n：密码
		{
			token = strtok((char *)uart3_buf, "-#");
			token = strtok(NULL, "-#");
			
			memset(card_writebuf, 0, 16);
			for(i = 0; i < strlen(token); i++)
				card_writebuf[i] = token[i] - '0';	
			
			// 挂起实时时间显示
			OSTaskSuspend(&app_task_tcb_rtc,&err);
			OSTaskSuspend(&app_task_tcb_dht11,&err);
			OSTaskSuspend(&app_task_tcb_rfid,&err);
			
			//阻塞等待互斥锁
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			OLED_Clear();
			OLED_ShowString(28,25,"pls set card!",12);//6*12
			OLED_Refresh();
			//立即释放互斥锁
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			
			MFRC522Set(card_writebuf);
			
			// 设置标志组
			OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
			//恢复实时时钟
			OSTaskResume(&app_task_tcb_rtc,&err);
			OSTaskResume(&app_task_tcb_dht11,&err);
			OSTaskResume(&app_task_tcb_rfid,&err);
		}
		else
		{
			//非法命令
			printf("unknow command = %s\n", (const char *)uart3_buf);
		}
		// 标志清0
		uart3_flag = 0;
		// 清空缓冲区和个数
		memset((char *)uart3_buf,0,sizeof(uart3_buf));
		uart3_cnt = 0;
	}
}
