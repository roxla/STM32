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
// 初始化串口
void usart1_init(void)
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
	// 发送换行
//	uart1_puts("\r\n");
}
// 
void usart3_init(void)
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
	u8 data;
	
	// 判断是不是串口接收中断
	if(USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		// 清除中断标志
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// 接收数据
		data = USART_ReceiveData(USART3);
		// 发送到调试口
		uart1_putc(data);
	}
}
