#include <myhead.h>

void iwdg_init(void)
{
	//1.解除寄存器写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//2.设置预分频值和初始计数值
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);//1s
	
	//3.喂狗
	IWDG_ReloadCounter();
	
	//4.使能看门狗
	IWDG_Enable();
}
