#include <usart.h>

void usart1_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 1.使能端口、串口1的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// 2.配置GPIO为复用功能：(TX:PA9		RX:PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3.将引脚连接到对应复用功能引脚
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// 4.配置串口信息：波特率、数据位、校验位
	USART_InitStructure.USART_BaudRate = baud;	// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// 1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	// 无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控：无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 串口发送和接收模式
	USART_Init(USART1, &USART_InitStructure);

	// 5.配置NVIC中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.使能串口
	USART_Cmd(USART1, ENABLE);
	//使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void usart3_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 1.使能端口、串口1的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// 2.配置GPIO为复用功能：(TX:PB10		RX:PB11)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 3.将引脚连接到对应复用功能引脚
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// 4.配置串口信息：波特率、数据位、校验位
	USART_InitStructure.USART_BaudRate = baud;	// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// 1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	// 无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控：无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 串口发送和接收模式
	USART_Init(USART3, &USART_InitStructure);
	
	// 5.配置NVIC中断
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.使能串口
	USART_Cmd(USART3, ENABLE);
	//使能串口接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

// 发送数据
void usart1_send_str(char *str)
{
	char *p = str;
	
	while( *p != '\0')
	{
		USART_SendData(USART1, *p);
		p++;
		
		//	等待串口发送成功
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

// 发送数据
void usart3_send_str(char *str)
{
	char *p = str;
	
	while(*p != '\0')
	{
		USART_SendData(USART3, *p);
		p++;
		
		// 等待串口发送成功
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

// 串口1中断处理函数(接收信息)
void USART1_IRQHandler(void)
{
	u8 data = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		// 接收数据
		data = USART_ReceiveData(USART1);
		// 将读取到的数据发送给PC
		USART_SendData(USART1, data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//	清空标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
