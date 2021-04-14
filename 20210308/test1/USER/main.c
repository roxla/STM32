#include <myhead.h>

int main(void)
{
	int32_t flag = 0, i;
	volatile uint32_t *p_data = (volatile uint32_t *)0x08010000;
	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	usart1_init(115200);	// 初始化串口1
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
