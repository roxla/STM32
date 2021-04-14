#include "keyboard.h"

extern OS_FLAG_GRP g_flag_grp;

void keyboard_init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	//使能端口A/B的硬件时钟，端口A/B才能工作，说白了就是对端口A/B上电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | 
							RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	
	//列配置  (输出)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	// PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//PA4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PG15
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//PC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//行配置  (输入)
	//GPIOF9,F10初始化设置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;	//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入模式，
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，驱动LED需要电流驱动
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//初始化GPIOC，把配置的数据写入寄存器
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PB6
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//PE6
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//PA8
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//列电平全部设为高电平
	PBout(7) = 1;
	PAout(4) = 1;
	PGout(15) = 1;
	PCout(7) = 1;
}

//识别对应的按键
void get_key(void)
{
	PBout(7)=0; //第一列
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;	
	while(PCin(9)==0) //(列:0,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"1");
		break;
	}
	while(PBin(6)==0)//(列:0,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"4");		
		break;			
	}		
	while(PEin(6)==0)//(列:0,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"7");		
		break;			
	}		
	while(PAin(8)==0)//(列:0,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"*");		
		break;			
	}	
	
	PBout(7)=1; //第二列
	PAout(4)=0;
	PGout(15)=1;
	PCout(7)=1;	
	while(PCin(9)==0) //(列:1,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"2");
		break;
	}
	while(PBin(6)==0)//(列:1,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"5");		
		break;			
	}		
	while(PEin(6)==0)//(列:1,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"8");		
		break;			
	}		
	while(PAin(8)==0)//(列:1,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"0");		
		break;			
	}
	
	PBout(7)=1; //第三列
	PAout(4)=1;
	PGout(15)=0;
	PCout(7)=1;			
	while(PCin(9)==0) //(列:2,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"3");
		break;
	}
	while(PBin(6)==0)//(列:2,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"6");		
		break;			
	}		
	while(PEin(6)==0)//(列:2,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"9");		
		break;			
	}		
	while(PAin(8)==0)//(列:2,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"#");		
		break;			
	}		
	
	PBout(7)=1; //第四列
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=0;	
	while(PCin(9)==1) //(列:2,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"A");
		break;
	}
//	while(PBin(6)==0)//(列:3,行:1)
//	{
//		key_stabilize(GPIOB,GPIO_Pin_6,"B");		
//		break;			
//	}		
//	while(PEin(6)==0)//(列:3,行:2)
//	{
//		key_stabilize(GPIOE,GPIO_Pin_6,"C");		
//		break;			
//	}		
//	while(PAin(8)==0)//(列:3,行:3)
//	{
//		key_stabilize(GPIOA,GPIO_Pin_8,"D");		
//		break;			
//	}		
}

//按键消抖及发送消息队列
void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch)
{
	OS_ERR err;//错误码

	//延时消抖
	delay_ms(20);
		
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
	{
		OSQPost(&g_queue_key,ch,strlen(ch),OS_OPT_POST_FIFO,&err);
		//等待按键1释放
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
		{
			delay_us(200);
		}
	}	
	//清空数据
	memset(ch,0,sizeof ch);
}

//获取键盘输入的密码
u8 getpassword_key(char *pwd)
{
	OS_ERR err;
	char *p=NULL;
	OS_MSG_SIZE msg_size;
	u8 i = 15;
	
	//清零计数值
//	TIM_SetCounter(TIM3, 0);
	while(i)
	{
		p = OSQPend(&g_queue_key,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);
		if(p && msg_size)
		{
//			TIM_SetCounter(TIM3, 0);
			if(strcmp(p,"#")==0)  //密码输完后确认键
			{
				return 1;
			}
			if(strcmp(p,"1")==0)
			{
				*pwd = '1';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"1",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"1",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"4")==0)
			{
				*pwd = '4';	
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"4",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"4",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"7")==0)
			{
				*pwd = '7';	
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"7",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"7",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"2")==0)
			{
				*pwd = '2';	
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"2",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"2",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"5")==0)
			{
				*pwd = '5';	
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"5",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"5",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"8")==0)
			{
				*pwd = '8';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"8",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"8",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"0")==0)
			{
				*pwd = '0';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"0",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"0",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"3")==0)
			{
				*pwd = '3';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"3",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"3",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"6")==0)
			{
				*pwd = '6';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"6",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"6",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"9")==0)
			{
				*pwd = '9';
				//阻塞等待互斥锁
				OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
				if(i > 7)
					OLED_ShowString((15-i)*15+15,25,"9",12);//6*12
				else
					OLED_ShowString((7-i)*15+15,40,"9",12);//6*12
				OLED_Refresh();
				//立即释放互斥锁
				OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			}
			if(strcmp(p,"*")==0)
			{
				return 0;
			}
			
			//阻塞等待互斥锁
//			OSMutexPend(&g_mutex_oled,0,OS_OPT_PEND_BLOCKING,NULL,&err);
//			if(i > 7)
//				OLED_ShowString((15-i)*15+15,25,"*",12);//6*12
//			else
//				OLED_ShowString((7-i)*15+15,40,"*",12);//6*12
//			OLED_Refresh();
//			//立即释放互斥锁
//			OSMutexPost(&g_mutex_oled,OS_OPT_POST_NONE,&err);
			
			pwd++;	
			i--;
		}
	}
	
	return 1;
}
