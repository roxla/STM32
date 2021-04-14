#include <usart.h>

void usart1_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 1.ʹ�ܶ˿ڡ�����1��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// 2.����GPIOΪ���ù��ܣ�(TX:PA9		RX:PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3.���������ӵ���Ӧ���ù�������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// 4.���ô�����Ϣ�������ʡ�����λ��У��λ
	USART_InitStructure.USART_BaudRate = baud;	// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// 1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	// ��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ�����أ���
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// ���ڷ��ͺͽ���ģʽ
	USART_Init(USART1, &USART_InitStructure);

	// 5.����NVIC�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void usart3_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 1.ʹ�ܶ˿ڡ�����1��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// 2.����GPIOΪ���ù��ܣ�(TX:PB10		RX:PB11)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;		//���ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//������ʣ����ģ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 3.���������ӵ���Ӧ���ù�������
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// 4.���ô�����Ϣ�������ʡ�����λ��У��λ
	USART_InitStructure.USART_BaudRate = baud;	// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	// 1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	// ��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ�����أ���
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// ���ڷ��ͺͽ���ģʽ
	USART_Init(USART3, &USART_InitStructure);
	
	// 5.����NVIC�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

// ��������
void usart1_send_str(char *str)
{
	char *p = str;
	
	while( *p != '\0')
	{
		USART_SendData(USART1, *p);
		p++;
		
		//	�ȴ����ڷ��ͳɹ�
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

// ��������
void usart3_send_str(char *str)
{
	char *p = str;
	
	while(*p != '\0')
	{
		USART_SendData(USART3, *p);
		p++;
		
		// �ȴ����ڷ��ͳɹ�
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

// ����1�жϴ�����(������Ϣ)
void USART1_IRQHandler(void)
{
	u8 data = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		// ��������
		data = USART_ReceiveData(USART1);
		// ����ȡ�������ݷ��͸�PC
		USART_SendData(USART1, data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//	��ձ�־λ
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
