#include <myhead.h>

int main(void)
{
	u32 comp = 0;
	
	pwm_init();
	systick_init();
	
	while(1){
		//���������
		while(comp<1000){
			TIM_SetCompare4(TIM1, comp++);
			delay_ms(2);
		}
		
		//���������
		while(comp>0){
			TIM_SetCompare4(TIM1, comp--);
			delay_ms(2);
		}
		
		delay_ms(200);
	}
}