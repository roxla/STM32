#include <myhead.h>

int main(void)
{
	int32_t flag = 0, i;
	volatile uint32_t *p_data = (volatile uint32_t *)0x08010000;
	// ѡ���ж����ȼ�2�飨4����ռ���ȼ���4����Ӧ���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	usart1_init(115200);	// ��ʼ������1
	flag = flash_test();
	
	for(i = 0; i < 64; i++)
	{
		if(flag != -1)
		{
			printf("%d\r\n", *(p_data+i));
		}
	}
	
	while(1)
	{
	
	}
}
