#include <myhead.h>

int main(void)
{
	// ѡ���ж����ȼ�2�飨4����ռ���ȼ���4����Ӧ���ȼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	usart1_init(115200);
	
	while(1)
	{
		// ��������
		printf("hello teacher zeng!\r\n");
		delay_ms(1000);
	}
}
