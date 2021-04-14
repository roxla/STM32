#include <usart.h>

void usart1_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 1.使能端口、串口1的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// 2.配置GPIO为复用功能
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.使能串口
	USART_Cmd(USART1, ENABLE);
}

// 发送数据
void usart1_send_str(char *str)
{
	char *p = str;
	
	while(*p != '\0')
	{
		USART_SendData(USART1, *p);
		p++;
		
		// 等待串口发送成功
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}
