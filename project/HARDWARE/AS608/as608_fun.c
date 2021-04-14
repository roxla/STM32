#include "as608_fun.h"
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效指纹个数
extern OS_Q	key_queue;					//key按键消息队列
//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	printf("===== %s\r\n",(u8*)EnsureMessage(ensure));
}

//获取键盘数值
u16 GET_NUM(void)
{
	char keybuf[3];
	int m=0;
	uint32_t rt;
	memset(keybuf,0,3);
	//bzero(keybuf,3);
	while(1)
	{
		rt = getpassword_key();
		printf("rt:%d\r\n",rt);
		if(rt==1)
		{	
			//printf("1\r\n");
			keybuf[m]='1';		
			m++;
			OLED_ShowString(m*18,3,"1",strlen(keybuf));						
		}	
		if(rt==4)
		{	
		//	printf("4\r\n");
			keybuf[m]='4';							
			m++;
			OLED_ShowString(m*18,3,"4",strlen(keybuf));						
		}
		if(rt==7)
		{	
		//	printf("7\r\n");
			keybuf[m]='7';					
			m++;		
			OLED_ShowString(m*18,3,"7",strlen(keybuf));					
		}
		if(rt==2)
		{	
		//	printf("2\r\n");
			keybuf[m]='2';						
			m++;	
			OLED_ShowString(m*18,3,"2",strlen(keybuf));				
		}
		if(rt==5)
		{	
		//	printf("5\r\n");
			keybuf[m]='5';						
			m++;		
			OLED_ShowString(m*18,3,"5",strlen(keybuf));				
		}			
		if(rt==8)
		{	
			//printf("8\r\n");
			keybuf[m]='8';				
			m++;
			OLED_ShowString(m*18,3,"8",strlen(keybuf));						
		}		
		if(rt==0)
		{	
		//	printf("0\r\n");
			keybuf[m]='0';					
			m++;		
			OLED_ShowString(m*18,3,"0",strlen(keybuf));					
		}			
		
		if(rt==3)
		{	
		//	printf("3\r\n");
			keybuf[m]='3';						
			m++;
			OLED_ShowString(m*18,3,"3",strlen(keybuf));				
		}
		if(rt==6)
		{	
			//printf("6\r\n");
			keybuf[m]='6';					
			m++;
			OLED_ShowString(m*18,3,"6",strlen(keybuf));					
		}			
		if(rt==9)
		{	
			//printf("9\r\n");
			keybuf[m]='9';					
			m++;		
			OLED_ShowString(m*18,3,"9",strlen(keybuf));					
		}
		
		if(rt == 13)
		{
			return atoi(keybuf);
			//OLED_Clear();
		}
	}

	
}

//录指纹
uint32_t Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	
	u16 ID;
	
	char keybuf[3];
	//char keybuf[60]={0};//存放按键数据
	
	memset(keybuf,0,3);
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				OLED_Clear();
				OLED_ShowCHinese(28,3,34);  //请
				OLED_ShowCHinese(46,3,35); //按
				OLED_ShowCHinese(64,3,2); //指
				OLED_ShowCHinese(82,3,3); //纹
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					PFout(8)=1;
					ensure=PS_GenChar(CharBuffer1);//生成特征
					PFout(8)=0;
					if(ensure==0x00)
					{
						OLED_Clear();
						OLED_ShowCHinese(28,3,2); //指
						OLED_ShowCHinese(46,3,3); //纹
						OLED_ShowCHinese(64,3,36); //正
						OLED_ShowCHinese(82,3,37); //常
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				OLED_Clear();
				OLED_ShowCHinese(0,3,38);   //请
				OLED_ShowCHinese(18,3,39); //再
				OLED_ShowCHinese(36,3,40); //按
				OLED_ShowCHinese(54,3,41); //一
				OLED_ShowCHinese(72,3,42); //次
				OLED_ShowCHinese(90,3,2); //指
				OLED_ShowCHinese(108,3,3);//纹
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					PFout(8)=1;
					ensure=PS_GenChar(CharBuffer2);//生成特征
					PFout(8)=0;
					if(ensure==0x00)
					{
						OLED_Clear();
						OLED_ShowCHinese(28,3,2); //指
						OLED_ShowCHinese(46,3,3); //纹
						OLED_ShowCHinese(64,3,36); //正
						OLED_ShowCHinese(82,3,37); //常
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
						break;

			case 2:
				OLED_Clear();
				OLED_ShowCHinese(28,3,43); //正
				OLED_ShowCHinese(46,3,44); //在
				OLED_ShowCHinese(64,3,45); //对
				OLED_ShowCHinese(82,3,46); //比
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					OLED_Clear();
					OLED_ShowCHinese(28,3,45); //对
					OLED_ShowCHinese(46,3,46); //比
					OLED_ShowCHinese(64,3,47); //成
					OLED_ShowCHinese(82,3,48); //功
					processnum=3;//跳到第四步
				}
				else 
				{
					OLED_Clear();
					OLED_ShowCHinese(28,3,45); //对
					OLED_ShowCHinese(46,3,46); //比
					OLED_ShowCHinese(64,3,49); //失
					OLED_ShowCHinese(82,3,50); //败
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;

			case 3:
				OLED_Clear();
				OLED_ShowCHinese(10,3,51); //正
				OLED_ShowCHinese(28,3,52); //在
				OLED_ShowCHinese(46,3,53); //生
				OLED_ShowCHinese(64,3,54); //成
				OLED_ShowCHinese(82,3,55); //模
				OLED_ShowCHinese(100,3,56);//板
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					OLED_Clear();
					OLED_ShowCHinese(10,3,53); //生
					OLED_ShowCHinese(28,3,54); //成
					OLED_ShowCHinese(46,3,55); //模
					OLED_ShowCHinese(64,3,56); //板
					OLED_ShowCHinese(82,3,47); //成
					OLED_ShowCHinese(100,3,48);//功
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				OLED_Clear();
				OLED_ShowCHinese(10,0,57);  //请
				OLED_ShowCHinese(28,0,58); //输
				OLED_ShowCHinese(46,0,59); //入
				OLED_ShowCHinese(64,0,60); //储
				OLED_ShowCHinese(82,0,61); //存
				OLED_ShowString(100,0,"I",16);//I
				OLED_ShowString(108,0,"D",16);//D
				OLED_ShowString(0,6,"ID<300",16); //ID<300
				OLED_ShowString(81,6,"#",16); //D
				OLED_ShowCHinese(90,6,16); //确
				OLED_ShowCHinese(108,6,17);//定
				do{
					ID=GET_NUM();
					
					printf("ID:%d\r\n",ID);
					break;
				}
				while(!(ID<AS608Para.PS_max));//输入ID必须小于模块容量最大的数值
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
			printf("ensure = %x\r\n",ensure);
				if(ensure==0x00) 
				{			
					OLED_Clear();
					OLED_ShowCHinese(10,3,62); //录
					OLED_ShowCHinese(28,3,63); //入
					OLED_ShowCHinese(46,3,64); //指
					OLED_ShowCHinese(64,3,65); //纹
					OLED_ShowCHinese(82,3,66); //成
					OLED_ShowCHinese(100,3,67);//功
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					OLED_ShowCHinese(15,6,64); //指
					OLED_ShowCHinese(33,6,65); //纹
					OLED_ShowCHinese(51,6,68); //数
					OLED_ShowCHinese(69,6,69); //量
					OLED_ShowNum(87,6,ValidN,5,16);
					delay_ms(1500);
					OLED_Clear();
					return 1;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(400);
		if(i==5)//超过5次没有按手指则退出
		{
			OLED_Clear();
			break;	
		}				
	}
	return 0;
}




//刷指纹
uint32_t press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		//BEEP=1;//打开蜂鸣器	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			//BEEP=0;//关闭蜂鸣器	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{				
				printf("指纹库匹配成功！,ID:%d  匹配得分:%d\r\n",seach.pageID,seach.mathscore);
			}
			else 
			{
				printf("匹配失败！\r\n");
				return 1;
			}
								
		}
		else
			ShowErrMessage(ensure);
	 //BEEP=0;//关闭蜂鸣器
	 delay_ms(600);
	}	
	return 0;
}

//删除指纹
uint32_t Del_FR(void)
{
	u8  ensure;
	u16 num = 0;
	u16 vnum1; //删除前后指纹个数对比
	u16 vnum2; //删除前后指纹个数对比	
	OLED_Clear();
	OLED_ShowCHinese(10,0,70);  //请
	OLED_ShowCHinese(28,0,71); //输
	OLED_ShowCHinese(46,0,72); //入
	OLED_ShowCHinese(64,0,73); //删
	OLED_ShowCHinese(82,0,74); //除
	OLED_ShowString(100,0,"I",16);//I
	OLED_ShowString(108,0,"D",16);//D
	OLED_ShowString(0,6,"ID<300",16); //ID<300
	OLED_ShowString(81,6,"#",16); //D
	OLED_ShowCHinese(90,6,16); //确
	OLED_ShowCHinese(108,6,17);//定
	delay_ms(50);	
	PS_ValidTempleteNum(&ValidN);//读库指纹个数	
	vnum1=ValidN;
	
	printf("num1 = %d\r\n",num);

	num=GET_NUM();//获取返回的数值
	
	printf("num2 = %d\r\n",num);
	
	if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
			PS_ValidTempleteNum(&ValidN);//读库指纹个数	
			vnum2=ValidN;	
			if(vnum1!=vnum2)
			{	
				OLED_Clear();
				OLED_ShowCHinese(10,3,73);  //删
				OLED_ShowCHinese(28,3,74);  //除
				OLED_ShowCHinese(46,3,2);  //指
				OLED_ShowCHinese(64,3,3);  //纹
				OLED_ShowCHinese(82,3,49);  //成
				OLED_ShowCHinese(100,3,50); //功	
				delay_ms(1000);
				OLED_Clear();
				PS_ValidTempleteNum(&ValidN);//读库指纹个数
				OLED_ShowCHinese(20,4,64); //指
				OLED_ShowCHinese(38,4,65); //纹
				OLED_ShowCHinese(56,4,68); //数
				OLED_ShowCHinese(74,4,69); //量
				OLED_ShowNum(90,4,ValidN,3,16);
				delay_ms(2000);	
				OLED_Clear();		
				return 1;
			}
			else
			{
				OLED_Clear();
				OLED_ShowCHinese(10,3,73);  //删
				OLED_ShowCHinese(28,3,74);  //除
				OLED_ShowCHinese(46,3,2);  //指
				OLED_ShowCHinese(64,3,3);  //纹
				OLED_ShowCHinese(82,3,47);  //成
				OLED_ShowCHinese(100,3,48); //功	
				delay_ms(1000);
				OLED_Clear();
				PS_ValidTempleteNum(&ValidN);//读库指纹个数
				OLED_ShowCHinese(20,4,64); //指
				OLED_ShowCHinese(38,4,65); //纹
				OLED_ShowCHinese(56,4,68); //数
				OLED_ShowCHinese(74,4,69); //量
				OLED_ShowNum(90,4,ValidN,3,16);
				delay_ms(2000);	
				OLED_Clear();
				return 2;				
			}
	}
	return 0;
}



