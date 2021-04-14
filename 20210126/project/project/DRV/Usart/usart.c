#include <myhead.h>

struct __FILE
{
	int handle;
};

FILE __stdout;

// 实现_sys_exit函数，避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 实现fputc
int fputc(int ch, FILE *f)
{
    //通过串口1发送ch
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)!=SET);
	
	return ch;
}
// 初始化串口1
void uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// 开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// GPIO初始化成复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 复用映射为串口功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// 初始化串口 9600 8N1
	USART_InitStruct.USART_BaudRate = 9600; // 波特率
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1位停止位
	USART_InitStruct.USART_Parity = USART_Parity_No; // 无校验
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
	USART_Init(USART1, &USART_InitStruct);
	// 初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn; // USART1
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2; // 响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	// 使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// 使能串口
	USART_Cmd(USART1,ENABLE);
}
// 发送一个字符
void uart1_putc(char ch)
{
	// 等待上一个发送完成
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	// 发送数据
	USART_SendData(USART1, ch);
}
// 发送一个字符串
void uart1_puts(const char *s)
{
	while(*s)
		uart1_putc(*s++);
}
// 串口1中断处理函数
void USART1_IRQHandler(void)
{
	u8 data;
	
	// 判断是不是串口接收中断
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
	{
		// 清除中断标志
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		// 接收数据
		data = USART_ReceiveData(USART1);
		// 发送到uart3
		uart3_putc(data);
	}
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
// 
volatile u32 uart3_cnt = 0;//记录串口3收到的数据长度
volatile u8 uart3_buf[64] = {0};//记录uart3收到的数据
volatile u32 uart3_flag = 0;//记录uart3收到完整数据 1 ----- 完整
// 串口3中断处理函数
void USART3_IRQHandler(void)
{	
	// 判断是不是串口接收中断
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// 清除中断标志
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// 接收数据
		uart3_buf[uart3_cnt++] = USART_ReceiveData(USART3);
		//判断数据是否接收完成(数据以换行符结束)
		if(uart3_buf[uart3_cnt - 1] == '\n' || uart3_cnt >= sizeof(uart3_buf))
			uart3_flag = 1;
	}
}
//处理收到命令函数
void parse_cmd(void)
{
	int distance = 0;
	u8 temp_buf[100] = {0};
	u8 temp = 0, humi = 0;
	
	// 当命令接收完成才处理
	if(uart3_flag)
	{
		if(strstr((const char *)uart3_buf, "beep"))
		{
			if(strstr((const char *)uart3_buf, "on"))
			{
				BEEP = 1;
				printf("BEEP ON!\r\n");
			}
			if(strstr((const char *)uart3_buf, "off"))
			{
				BEEP = 0;
				printf("BEEP OFF!\r\n");
			}
		}
		else if(strstr((const char *)uart3_buf, "getdistance"))
		{
			if((distance = get_distance()) != -1)
			{
				snprintf((char *)temp_buf,100, "distance=%d", distance);
			}
			else
			{
				snprintf((char *)temp_buf,100,"getdistance error");
			}
			uart3_puts((char *)temp_buf);
			printf("%s\r\n",(char *)temp_buf);
		}
		else if(strstr((const char *)uart3_buf, "led"))
		{
			if(strstr((const char *)uart3_buf, "1"))
			{
				if(strstr((const char *)uart3_buf, "on"))
				{
					LED0 = 0;
					printf("LED1 ON!\r\n");
				}
				if(strstr((const char *)uart3_buf, "off"))
				{
					LED0 = 1;
					printf("LED1 OFF!\r\n");
				}
			}
			if(strstr((const char *)uart3_buf, "2"))
			{
				if(strstr((const char *)uart3_buf, "on"))
				{
					LED1 = 0;
					printf("LED2 ON!\r\n");
				}
				if(strstr((const char *)uart3_buf, "off"))
				{
					LED1 = 1;
					printf("LED2 OFF!\r\n");
				}
			}
			if(strstr((const char *)uart3_buf, "3"))
			{
				if(strstr((const char *)uart3_buf, "on"))
				{
					LED2 = 0;
					printf("LED3 ON!\r\n");
				}
				if(strstr((const char *)uart3_buf, "off"))
				{
					LED2 = 1;
					printf("LED3 OFF!\r\n");
				}
			}
			if(strstr((const char *)uart3_buf, "4"))
			{
				if(strstr((const char *)uart3_buf, "on"))
				{
					LED3 = 0;
					printf("LED4 ON!\r\n");
				}
				if(strstr((const char *)uart3_buf, "off"))
				{
					LED3 = 1;
					printf("LED4 OFF!\r\n");
				}
			}
		}
		else if(strstr((const char *)uart3_buf, "getweather"))
		{
			delay_s(1);
			if(dht11_read_data(&temp, &humi) == 0)
			{
				snprintf((char *)temp_buf,100,"temp=%d, humi=%d", temp,humi);
			}
			else
			{
				snprintf((char *)temp_buf,100,"readWeather error");
			}
			uart3_puts((char *)temp_buf);
			printf("%s\r\n",(char *)temp_buf);
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
