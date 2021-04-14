#include <myhead.h>

int main(void)
{
	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	usart1_init(115200);
	
	while(1)
	{
		// 发送数据
		printf("hello teacher zeng!\r\n");
		delay_ms(1000);
	}
}
