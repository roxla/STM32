#include <myhead.h>

int main(void)
{
	int i;

	// 选择中断优先级2组（4级抢占优先级、4级响应优先级）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	led1_af_init();
//	led3_af_init();
//	led4_af_init();
	
	tim14_pwm_config();
//	tim1_pwm_config();
	
	while(1)
	{
		for(i=1; i<1000; i+=10)
		{
//			TIM_SetCompare3(TIM1, i);
//			TIM_SetCompare4(TIM1, i);
			TIM_SetCompare1(TIM14, i);//根据i的值设置比较值，调整占空比
			
			delay_ms(20);//根据实际情况进行调整，方便观察效果
		}
		
		for(i=1000; i>0; i-=10)
		{
//			TIM_SetCompare3(TIM1, i);
//			TIM_SetCompare4(TIM1, i);
			TIM_SetCompare1(TIM14, i);
			
			delay_ms(20);
		}
	}
}
