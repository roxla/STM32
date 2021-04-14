#include <beep.h>

//��ʱ
void delay(unsigned int ms)
{
	int i,j;
	
	for(i=0;i<ms;i++){
		for(j=0;j<5000;j++);
	}
}
// ��ʼ��
void beep_init(void)
{
	// BEEP --- PF8
	GPIO_InitTypeDef GPIO_InitStruct;
	// RCC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	// GPIO_INIT
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
}
// ����
void beep_switch(int state)
{
	// PF8�ߵ�ƽ��ͨ���͵�ƽ�Ͽ�
	if(state)
	{
//		GPIOF->BSRRL |= (0x1 << 8);
		GPIO_SetBits(GPIOF,GPIO_Pin_8);
	}
	else
	{
//		GPIOF->BSRRH |= (0x1 << 8);
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	}
}
