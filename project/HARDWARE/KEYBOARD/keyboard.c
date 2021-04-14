#include "keyboard.h"

extern OS_FLAG_GRP g_flag_grp;

void keyboard_init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	//ʹ�ܶ˿�A/B��Ӳ��ʱ�ӣ��˿�A/B���ܹ�����˵���˾��ǶԶ˿�A/B�ϵ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | 
							RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	
	//������  (���)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	// PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//PA4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PG15
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//PC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//������  (����)
	//GPIOF9,F10��ʼ������ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;	//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����ģʽ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������������LED��Ҫ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��GPIOC�������õ�����д��Ĵ���
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PB6
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PE6
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//PA8
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//�е�ƽȫ����Ϊ�ߵ�ƽ
	PBout(7) = 1;
	PAout(4) = 1;
	PGout(15) = 1;
	PCout(7) = 1;
}

//ʶ���Ӧ�İ���
void get_key(void)
{
	PBout(7)=0; //��һ��
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;	
	while(PCin(9)==0) //(��:0,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"1");
		break;
	}
	while(PBin(6)==0)//(��:0,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"4");		
		break;			
	}		
	while(PEin(6)==0)//(��:0,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"7");		
		break;			
	}		
	while(PAin(8)==0)//(��:0,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"*");		
		break;			
	}	
	
	PBout(7)=1; //�ڶ���
	PAout(4)=0;
	PGout(15)=1;
	PCout(7)=1;	
	while(PCin(9)==0) //(��:1,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"2");
		break;
	}
	while(PBin(6)==0)//(��:1,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"5");		
		break;			
	}		
	while(PEin(6)==0)//(��:1,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"8");		
		break;			
	}		
	while(PAin(8)==0)//(��:1,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"0");		
		break;			
	}
	
	PBout(7)=1; //������
	PAout(4)=1;
	PGout(15)=0;
	PCout(7)=1;			
	while(PCin(9)==0) //(��:2,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"3");
		break;
	}
	while(PBin(6)==0)//(��:2,��:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"6");		
		break;			
	}		
	while(PEin(6)==0)//(��:2,��:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"9");		
		break;			
	}		
	while(PAin(8)==0)//(��:2,��:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"#");		
		break;			
	}		
	
	PBout(7)=1; //������
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=0;	
	while(PCin(9)==1) //(��:2,��:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"A");
		break;
	}
//	while(PBin(6)==0)//(��:3,��:1)
//	{
//		key_stabilize(GPIOB,GPIO_Pin_6,"B");		
//		break;			
//	}		
//	while(PEin(6)==0)//(��:3,��:2)
//	{
//		key_stabilize(GPIOE,GPIO_Pin_6,"C");		
//		break;			
//	}		
//	while(PAin(8)==0)//(��:3,��:3)
//	{
//		key_stabilize(GPIOA,GPIO_Pin_8,"D");		
//		break;			
//	}		
}

//����������������Ϣ����
void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch)
{
	OS_ERR err;//������

	//��ʱ����
	delay_ms(20);
		
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
	{
		OSQPost(&g_queue_key,ch,strlen(ch),OS_OPT_POST_FIFO,&err);
		//�ȴ�����1�ͷ�
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
		{
			delay_us(200);
		}
	}	
	//�������
	memset(ch,0,sizeof ch);
}

//��ȡ�������������
u8 getpassword_key(char *pwd)
{
	OS_ERR err;
	char *p=NULL;
	OS_MSG_SIZE msg_size;
	u8 i = 15;
	
	//�������ֵ
//	TIM_SetCounter(TIM3, 0);
	while(i)
	{
		p = OSQPend(&g_queue_key,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
//			TIM_SetCounter(TIM3, 0);
			if(strcmp(p,"#")==0)  //���������ȷ�ϼ�
			{
				return 1;
			}
			if(strcmp(p,"1")==0)
			{
				*pwd = '1';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"1",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"1",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"4")==0)
			{
				*pwd = '4';	
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"4",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"4",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"7")==0)
			{
				*pwd = '7';	
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"7",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"7",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"2")==0)
			{
				*pwd = '2';	
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"2",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"2",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"5")==0)
			{
				*pwd = '5';	
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"5",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"5",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"8")==0)
			{
				*pwd = '8';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"8",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"8",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"0")==0)
			{
				*pwd = '0';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"0",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"0",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"3")==0)
			{
				*pwd = '3';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"3",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"3",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"6")==0)
			{
				*pwd = '6';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"6",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"6",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"9")==0)
			{
				*pwd = '9';
				//�����ȴ�������
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"9",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"9",12);//6*12
				OLED_Refresh();
				//�����ͷŻ�����
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"*")==0)
			{
				return 0;
			}
			
			//�����ȴ�������
//			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
//			if(i > 7)
//				OLED_ShowString((15-i)*15+15,25,"*",12);//6*12
//			else
//				OLED_ShowString((7-i)*15+15,40,"*",12);//6*12
//			OLED_Refresh();
//			//�����ͷŻ�����
//			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			
			pwd++;	
			i--;
		}
	}
	
	return 1;
}
