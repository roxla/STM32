#include <myhead.h>

int main(void)
{
	int i;

	// ѡ���ж����ȼ�2�飨4����ռ���ȼ���4����Ӧ���ȼ���
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
			TIM_SetCompare1(TIM14, i);//����i��ֵ���ñȽ�ֵ������ռ�ձ�
			
			delay_ms(20);//����ʵ��������е���������۲�Ч��
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
