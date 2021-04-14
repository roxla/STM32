#include <myhead.h>

void iwdg_init(void)
{
	//1.����Ĵ���д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//2.����Ԥ��Ƶֵ�ͳ�ʼ����ֵ
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);//1s
	
	//3.ι��
	IWDG_ReloadCounter();
	
	//4.ʹ�ܿ��Ź�
	IWDG_Enable();
}
