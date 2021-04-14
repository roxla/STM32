#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "beep.h"
#include "pwm.h"
#include "rtc.h"
#include "includes.h"
#include "flash.h"
#include "oled.h"
#include "keyboard.h"
#include "w25q128.h"
#include "tim.h"
#include "ble.h"
#include "rfid.h"

//��ʼ�����ƿ�
OS_TCB app_task_tcb_init;
void app_task_init(void *parg);
CPU_STK app_task_stk_init[128];			//��ʼ���������ջ����СΪ128�֣�Ҳ����512�ֽ�

//��ʪ�ȿ��ƿ�
OS_TCB app_task_tcb_dht11;
void app_task_dht11(void *parg);
CPU_STK app_task_stk_dht11[128];		//��ʪ�ȵ������ջ����СΪ128�֣�Ҳ����512�ֽ�

//led���ƿ�
OS_TCB app_task_tcb_led;
void app_task_led(void *parg);
CPU_STK app_task_stk_led[128];			//��ʪ�ȵ������ջ����СΪ128�֣�Ҳ����512�ֽ�

//���������ƿ�
OS_TCB app_task_tcb_beep;
void app_task_beep(void *parg);
CPU_STK app_task_stk_beep[128];			//��ʪ�ȵ������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����rtc���ƿ�
OS_TCB app_task_tcb_rtc;
void app_task_rtc(void *parg);
CPU_STK app_task_stk_rtc[128];			//rtc�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����flash���ƿ�
OS_TCB app_task_tcb_flash;
void app_task_flash(void *parg);
CPU_STK app_task_stk_flash[128];		//flash�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//�����ⲿflash���ƿ�
OS_TCB app_task_tcb_w25q128;
void app_task_w25q128(void *parg);
CPU_STK app_task_stk_w25q128[128];		//�ⲿflash�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//���������̿��ƿ�
OS_TCB app_task_tcb_keyboard;
void app_task_keyboard(void *parg);
CPU_STK app_task_stk_keyboard[128];		//������̵������ջ����СΪ128�֣�Ҳ����512�ֽ�

//�����������ƿ�
OS_TCB app_task_tcb_ble;
void app_task_ble(void *parg);
CPU_STK app_task_stk_ble[128];			//�����������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����OLED���ƿ�
OS_TCB app_task_tcb_oled;
void app_task_oled(void *parg);
CPU_STK app_task_stk_oled[128];			//OLED�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����������ƿ�
OS_TCB app_task_tcb_lockon;
void app_task_lockon(void *parg);
CPU_STK app_task_stk_lockon[128];		//�����������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����������ƿ�
OS_TCB app_task_tcb_rfid;
void app_task_rfid(void *parg);
CPU_STK app_task_stk_rfid[128];			//�����������ջ����СΪ128�֣�Ҳ����512�ֽ�

OS_FLAG_GRP				g_flag_grp;			//�¼���־��Ķ���

OS_MUTEX				g_mutex_oled;		//�������Ķ���

OS_SEM					g_sem_led;			//�ź����Ķ���
OS_SEM					g_sem_beep;			//�ź����Ķ���

OS_Q					g_queue_key;		//��Ϣ���еĶ���
OS_Q					g_queue_flash;		//��Ϣ���еĶ���

char					g_pwd[7];			//����(ȫ��)
char 					g_cardData[7]; 		//������(ȫ��)

//������
int main(void)
{
	OS_ERR err;

	systick_init();  													//ʱ�ӳ�ʼ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//�жϷ�������
	
	usart_init(9600);  				 									//���ڳ�ʼ��
	
	LED_Init();         												//LED��ʼ��	

	//OS��ʼ�������ǵ�һ�����еĺ���,��ʼ�����ֵ�ȫ�ֱ����������ж�Ƕ�׼����������ȼ����洢��
	OSInit(&err);


	//��������1
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_init,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_init",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_init,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)5,											 	//��������ȼ�5		
					(CPU_STK *)app_task_stk_init,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
					
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_init create fail\r\n");
		
		while(1);
	
	}

	//����OS�������������
	OSStart(&err);
					
					
	printf(".......\r\n");
					
	while(1);
	
}


void app_task_init(void *parg)
{
	OS_ERR err;
	
	// ��ʪ�ȳ�ʼ��
	dht11_set_io(GPIO_Mode_OUT);
	
	// ����LED1Ϊ����ģʽ
	led1_af_init();
	// ����PWM
	tim14_pwm_config();
	
	// ����beepΪ����ģʽ
	beep_af_init();
	// ����PWM
	tim13_pwm_config();
	
	// RTCʱ�ӳ�ʼ��
	// �жϱ��ݼĴ���0�Ƿ�Ϊ0x9527
		// ���ǣ�����RTC��ʱ����Ҫ����
		// �ǣ�����RTCʱ�������ã���������
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x9527)
		rtc_init_set();
	else
		rtc_init();
	
	// ��ʼ��OLED
	OLED_Init(); 
	
	// ��ʼ���ⲿflash
	w25qxx_init();
	
	// ���̳�ʼ��
	keyboard_init();
	// ��ʼ����������
	flash_set_pwd(123456);
	// ��ʼ����ʱ��3
//	TIM3_Init();
	
	// ������ʼ��
	uart3_init();
	ble_config();
	
	//�����¼���־�飬���б�־λ��ֵΪ0
	OSFlagCreate(&g_flag_grp,"g_flag_grp",0,&err);
	
	//������Ϣ���У����ڴ����������
	OSQCreate(&g_queue_key,"g_queue_key",16,&err);
	//������Ϣ���У����ڴ���flash����
	OSQCreate(&g_queue_flash,"g_queue_flash",16,&err);
	
	//����������
	OSMutexCreate(&g_mutex_oled, "g_mutex_oled",&err);
	
	// ��ʪ�����񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_dht11,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_dht11",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_dht11,							//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_dht11,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_dht11 create fail\r\n");
		while(1);
	}
	
	// led���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_led,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_led",								//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_led,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_led,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_led create fail\r\n");
		while(1);
	}
	
	// beep���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_beep,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_beep",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_beep,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_beep,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_beep create fail\r\n");
		while(1);
	}
	
	// rtc���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_rtc,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_rtc",								//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_rtc,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_rtc,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_rtc create fail\r\n");
		while(1);
	}
	
	// flash���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_flash,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_flash",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_flash,							//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_flash,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_flash create fail\r\n");	
		while(1);
	}
	
	// �ⲿflash���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_w25q128,						//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_w25q128",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_w25q128,							//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_w25q128,						//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_w25q128 create fail\r\n");
		while(1);
	}
	
	// ����������񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_keyboard,						//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_keyboard",						//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_keyboard,							//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_keyboard,						//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// �������񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_ble,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_ble",								//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_ble,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_ble,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// OLED���񴴽�
	// ���ñ�־��
	OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_oled,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_oled",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_oled,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_oled,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// �������񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_lockon,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_lockon",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_lockon,							//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_lockon,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}
	
	// RFID���񴴽�
	OSTaskCreate(	(OS_TCB *)&app_task_tcb_rfid,							//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"app_task_rfid",							//��������֣����ֿ����Զ����
					(OS_TASK_PTR)app_task_rfid,								//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)7,											 	//��������ȼ�7		
					(CPU_STK *)app_task_stk_rfid,							//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	if(err!=OS_ERR_NONE)
	{
		printf("app_task_keyboard create fail\r\n");
		while(1);
	}

	//ɾ���������񣬽�������̬
	OSTaskDel(NULL,&err);
}

void app_task_dht11(void *parg)
{
	uint8_t buf[5] = {0};
	char temp[30];
	OS_ERR err;
	
	while(1)
	{
		dht11_read_data(buf);
		snprintf(temp, sizeof(temp), "T: %d.%d  H: %d.%d", buf[2], buf[3], buf[0], buf[1]);
		//�����ȴ�������
		OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
		
		OLED_ShowString(18,28,(u8 *)temp,12);
		OLED_Refresh();
		//�����ͷŻ�����
		OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		
		delay_ms(6000);
	}
}

void app_task_led(void *parg)
{
	
}

void app_task_beep(void *parg)
{
	
}

void app_task_rtc(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	char tim[30];

	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_RTC_WKUP,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_RTC_WKUP)
		{
			//�����ȴ�������
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			
			RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
			RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
			
			snprintf(tim, sizeof(tim), 
					"20%02X-%02X-%02X %02X:%02X:%02X", 
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					
			OLED_ShowString(8,53,(u8 *)tim,12);//6*12
			OLED_Refresh();
			
			//�����ͷŻ�����
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		}
	}
}

void app_task_oled(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	
	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_OLED_MAIN_SHOW,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_OLED_MAIN_SHOW)
		{
			//�����ȴ�������
			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
			
			OLED_Clear();
			OLED_ShowChinese(18,0,26,16);//��
			OLED_ShowChinese(36,0,27,16);//��
			OLED_ShowChinese(54,0,18,16);//��
			OLED_ShowChinese(72,0,19,16);//��
			OLED_ShowChinese(90,0,21,16);//��
			OLED_Refresh();
			
			//�����ͷŻ�����
			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
		}
	}
}

void app_task_flash(void *parg)
{
	uint32_t o_pwd = 0, i;
	char *p=NULL;
	OS_ERR  err;
	OS_MSG_SIZE msg_size;
	char str[20];
	// ��ȡ����
	flash_get_pwd(&o_pwd);
	sprintf(g_pwd, "%d", o_pwd);
	
	while(1)
	{
		memset(str, 0 ,sizeof(str));
		p = OSQPend(&g_queue_key,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
			printf("%s\r\n",p);
			if(strcmp(p,"A")==0)  //������������
			{
				// ��������
				OSTaskSuspend(&app_task_tcb_rtc,&err);
				OSTaskSuspend(&app_task_tcb_dht11,&err);
				
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowChinese(18,0,15,16);//��
				OLED_ShowChinese(36,0,16,16);//��
				OLED_ShowChinese(54,0,17,16);//��
				OLED_ShowChinese(72,0,18,16);//��
				OLED_ShowChinese(90,0,19,16);//��
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				i = getpassword_key(str);
				printf("pwd = %s\r\n",str);
				printf("g_pwd = %s\r\n",g_pwd);
				if(i == 1)
				{
					i = virtual_pwd(str, g_pwd);
					if(i == 1)
					{	
						// д���¼��flash��
						OSQPost(&g_queue_flash,"key",3,OS_OPT_POST_FIFO,&err);
					
						//�����ȴ�������
						OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
						OLED_Clear();
						OLED_ShowChinese(27,0,20,16);//��
						OLED_ShowChinese(45,0,21,16);//��
						OLED_ShowChinese(63,0,22,16);//��
						OLED_ShowChinese(81,0,23,16);//��
						OLED_Refresh();
						//�����ͷŻ�����
						OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);

						LED2 = ~LED2;
						delay_ms(500);
						LED2 = ~LED2;
					}
					else
					{
						//�����ȴ�������
						OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
						OLED_Clear();
						OLED_ShowChinese(27,0,20,16);//��
						OLED_ShowChinese(45,0,21,16);//��
						OLED_ShowChinese(63,0,24,16);//ʧ
						OLED_ShowChinese(81,0,25,16);//��
						OLED_Refresh();
						//�����ͷŻ�����
						OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
						
						LED2 = ~LED2;
						delay_ms(1000);
						LED2 = ~LED2;
					}
				}
				// ���ñ�־��
				OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
				//�ָ�����
				OSTaskResume(&app_task_tcb_rtc,&err);
				OSTaskResume(&app_task_tcb_dht11,&err);
			}
		}
	}
}

void app_task_w25q128(void *parg)
{
	OS_ERR err;
	uint16_t temp;
	char buf[64], unlock[20];
	uint32_t mem_rec_cnt = 0, i;
	char *p=NULL;
	OS_MSG_SIZE msg_size;
	
	//���뱣��CPU��״̬
	CPU_SR_ALLOC();
	
	temp = w25qxx_read_id();
	printf("0x%X\r\n",temp);
	
//	w25qxx_erase_sector(0);
	
	//���Ի�ȡ100����¼
	for(i=0;i<100;i++)
	{
		//��ȡ�洢�ļ�¼
		w25qxx_read_data(i*64,(u8 *)buf,64);
		
		//����¼�Ƿ���ڻ��з��ţ��������򲻴�ӡ���
		if(strstr((const char *)buf,"\n") == 0)
			break;	
	}
	mem_rec_cnt = i;	// flash�洢�Ĳ�����¼������
	
	while(1)
	{
		p = OSQPend(&g_queue_flash,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
			if(mem_rec_cnt < 100)
			{
				RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
				RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
				// �������
				if(strcmp(p,"key") == 0)
					snprintf(unlock, 20, "%s", "�������");
				// ��������
				if(strcmp(p,"ble") == 0)
					snprintf(unlock, 20, "%s", "��������");
				// RFID����
				if(strcmp(p,"rfid") == 0)
					snprintf(unlock, 20, "%s", "RFID����");
				// ָ�ƽ���
				if(strcmp(p,"as608") == 0)
					snprintf(unlock, 20, "%s", "ָ�ƽ���");
				
				snprintf(buf, 64, "20%02X-%02X-%02X %02X:%02X:%02X  %s\r\n", 
						RTC_DateStructure.RTC_Year,
						RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date,
						RTC_TimeStructure.RTC_Hours,
						RTC_TimeStructure.RTC_Minutes,
						RTC_TimeStructure.RTC_Seconds,
						unlock);
				
				OS_CRITICAL_ENTER();	// flashд�����ݣ������ٽ���
				w25qxx_write_page(mem_rec_cnt*64,(u8 *)buf,64);
				OS_CRITICAL_EXIT() ;  //flashд��������ɣ��˳��ٽ���
				
				mem_rec_cnt++;	// д�����+1
			}
			else
			{
				// ����ʵʱʱ����ʾ
				OSTaskSuspend(&app_task_tcb_rtc,&err);
				OSTaskSuspend(&app_task_tcb_dht11,&err);
			
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowString(28,50,"data full!",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				OS_CRITICAL_ENTER();	// flash�������ݣ������ٽ���
				w25qxx_erase_sector(0);
				OS_CRITICAL_EXIT() ;  //flash����������ɣ��˳��ٽ���
				
				mem_rec_cnt = 0;
				delay_ms(100);
				// ���ñ�־��
				OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
				OSQPost(&g_queue_flash,p,strlen(p),OS_OPT_POST_FIFO,&err);
				//�ָ�ʵʱʱ��
				OSTaskResume(&app_task_tcb_rtc,&err);
				OSTaskResume(&app_task_tcb_dht11,&err);
			}
		}
	}
	

}

void app_task_keyboard(void *parg)
{
	while(1)
	{
		get_key();
		delay_ms(50);
	}
}

void app_task_ble(void *parg)
{	
	while(1)
	{
		parse_cmd();
		delay_ms(100);
	}
}

void app_task_lockon(void *parg)
{
	OS_ERR err;
	OS_FLAGS flags = 0;
	while(1)
	{
		flags = OSFlagPend(&g_flag_grp,FLAG_LOCKON_ALL,0,OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,NULL,&err);
		if(flags & FLAG_LOCKON_FLAG)
		{
			// ����ʵʱʱ����ʾ
			OSTaskSuspend(&app_task_tcb_rtc,&err);
			OSTaskSuspend(&app_task_tcb_dht11,&err);

			// BLE
			if(flags & FLAG_OLED_BLE_LOCKON)
			{
				// д���¼��flash��
				OSQPost(&g_queue_flash,"ble",3,OS_OPT_POST_FIFO,&err);
			
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				OLED_Clear();
				OLED_ShowChinese(27,0,20,16);//��
				OLED_ShowChinese(45,0,21,16);//��
				OLED_ShowChinese(63,0,22,16);//��
				OLED_ShowChinese(81,0,23,16);//��
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
				
				LED2 = ~LED2;
				delay_ms(500);
				LED2 = ~LED2;
			}
			// RFID
			if(flags & FLAG_OLED_RFID_LOCKON)
			{
				if(strcmp(g_cardData,"123456") == 0)
				{
					// д���¼��flash��
					OSQPost(&g_queue_flash,"rfid",4,OS_OPT_POST_FIFO,&err);
					
					//�����ȴ�������
					OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
					OLED_Clear();
					OLED_ShowChinese(27,0,20,16);//��
					OLED_ShowChinese(45,0,21,16);//��
					OLED_ShowChinese(63,0,22,16);//��
					OLED_ShowChinese(81,0,23,16);//��
					OLED_Refresh();
					//�����ͷŻ�����
					OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
					
					LED2 = ~LED2;
					delay_ms(500);
					LED2 = ~LED2;
				}
				else
				{
					//�����ȴ�������
					OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
					OLED_Clear();
					OLED_ShowChinese(27,0,20,16);//��
					OLED_ShowChinese(45,0,21,16);//��
					OLED_ShowChinese(63,0,24,16);//ʧ
					OLED_ShowChinese(81,0,25,16);//��
					OLED_Refresh();
					//�����ͷŻ�����
					OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
					
					LED2 = ~LED2;
					delay_ms(500);
					LED2 = ~LED2;
					
				}
				memset(g_cardData, 0, 7);
			}
			
			// ���ñ�־��
			OSFlagPost(&g_flag_grp, FLAG_OLED_MAIN_SHOW, OS_OPT_POST_FLAG_SET, &err);
			//�ָ�ʵʱʱ��
			OSTaskResume(&app_task_tcb_rtc,&err);
			OSTaskResume(&app_task_tcb_dht11,&err);
		}
	}
}

void app_task_rfid(void *parg)
{
	while(1)
	{
		MFRC522_Initializtion();			// ��ʼ��MFRC522
		MFRC522Get(g_cardData);
		delay_ms(1000);
	}
}
