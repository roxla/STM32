#include <usart.h>

void usart1_init(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 1.ʹ�ܶ˿ڡ�����1��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// 2.����GPIOΪ���ù���
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 6.ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
}

// ��������
void usart1_send_str(char *str)
{
	char *p = str;
	
	while(*p != '\0')
	{
		USART_SendData(USART1, *p);
		p++;
		
		// �ȴ����ڷ��ͳɹ�
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}
