#include <myhead.h>

int main(void)
{
	int i, arr[] = {523, 587, 659, 698, 784, 880, 988};

	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	beep_af_init();
	tim13_pwm_config();
	
	while(1)
	{
		for(i = 0; i < 7; i++)
		{
			TIM_SetCompare1(TIM13, arr[i]);
			delay_ms(500);
			TIM_SetCompare1(TIM13, 0);
			delay_ms(500);
		}
	}
}
