#include "as608_fun.h"
SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чָ�Ƹ���
extern OS_Q	key_queue;					//key������Ϣ����
//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	printf("===== %s\r\n",(u8*)EnsureMessage(ensure));
}

//��ȡ������ֵ
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

//¼ָ��
uint32_t Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	
	u16 ID;
	
	char keybuf[3];
	//char keybuf[60]={0};//��Ű�������
	
	memset(keybuf,0,3);
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				OLED_Clear();
				OLED_ShowCHinese(28,3,34);  //��
				OLED_ShowCHinese(46,3,35); //��
				OLED_ShowCHinese(64,3,2); //ָ
				OLED_ShowCHinese(82,3,3); //��
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					PFout(8)=1;
					ensure=PS_GenChar(CharBuffer1);//��������
					PFout(8)=0;
					if(ensure==0x00)
					{
						OLED_Clear();
						OLED_ShowCHinese(28,3,2); //ָ
						OLED_ShowCHinese(46,3,3); //��
						OLED_ShowCHinese(64,3,36); //��
						OLED_ShowCHinese(82,3,37); //��
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				OLED_Clear();
				OLED_ShowCHinese(0,3,38);   //��
				OLED_ShowCHinese(18,3,39); //��
				OLED_ShowCHinese(36,3,40); //��
				OLED_ShowCHinese(54,3,41); //һ
				OLED_ShowCHinese(72,3,42); //��
				OLED_ShowCHinese(90,3,2); //ָ
				OLED_ShowCHinese(108,3,3);//��
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					PFout(8)=1;
					ensure=PS_GenChar(CharBuffer2);//��������
					PFout(8)=0;
					if(ensure==0x00)
					{
						OLED_Clear();
						OLED_ShowCHinese(28,3,2); //ָ
						OLED_ShowCHinese(46,3,3); //��
						OLED_ShowCHinese(64,3,36); //��
						OLED_ShowCHinese(82,3,37); //��
						i=0;
						processnum=2;//����������
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
						break;

			case 2:
				OLED_Clear();
				OLED_ShowCHinese(28,3,43); //��
				OLED_ShowCHinese(46,3,44); //��
				OLED_ShowCHinese(64,3,45); //��
				OLED_ShowCHinese(82,3,46); //��
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					OLED_Clear();
					OLED_ShowCHinese(28,3,45); //��
					OLED_ShowCHinese(46,3,46); //��
					OLED_ShowCHinese(64,3,47); //��
					OLED_ShowCHinese(82,3,48); //��
					processnum=3;//�������Ĳ�
				}
				else 
				{
					OLED_Clear();
					OLED_ShowCHinese(28,3,45); //��
					OLED_ShowCHinese(46,3,46); //��
					OLED_ShowCHinese(64,3,49); //ʧ
					OLED_ShowCHinese(82,3,50); //��
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;

			case 3:
				OLED_Clear();
				OLED_ShowCHinese(10,3,51); //��
				OLED_ShowCHinese(28,3,52); //��
				OLED_ShowCHinese(46,3,53); //��
				OLED_ShowCHinese(64,3,54); //��
				OLED_ShowCHinese(82,3,55); //ģ
				OLED_ShowCHinese(100,3,56);//��
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					OLED_Clear();
					OLED_ShowCHinese(10,3,53); //��
					OLED_ShowCHinese(28,3,54); //��
					OLED_ShowCHinese(46,3,55); //ģ
					OLED_ShowCHinese(64,3,56); //��
					OLED_ShowCHinese(82,3,47); //��
					OLED_ShowCHinese(100,3,48);//��
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				OLED_Clear();
				OLED_ShowCHinese(10,0,57);  //��
				OLED_ShowCHinese(28,0,58); //��
				OLED_ShowCHinese(46,0,59); //��
				OLED_ShowCHinese(64,0,60); //��
				OLED_ShowCHinese(82,0,61); //��
				OLED_ShowString(100,0,"I",16);//I
				OLED_ShowString(108,0,"D",16);//D
				OLED_ShowString(0,6,"ID<300",16); //ID<300
				OLED_ShowString(81,6,"#",16); //D
				OLED_ShowCHinese(90,6,16); //ȷ
				OLED_ShowCHinese(108,6,17);//��
				do{
					ID=GET_NUM();
					
					printf("ID:%d\r\n",ID);
					break;
				}
				while(!(ID<AS608Para.PS_max));//����ID����С��ģ������������ֵ
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
			printf("ensure = %x\r\n",ensure);
				if(ensure==0x00) 
				{			
					OLED_Clear();
					OLED_ShowCHinese(10,3,62); //¼
					OLED_ShowCHinese(28,3,63); //��
					OLED_ShowCHinese(46,3,64); //ָ
					OLED_ShowCHinese(64,3,65); //��
					OLED_ShowCHinese(82,3,66); //��
					OLED_ShowCHinese(100,3,67);//��
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					OLED_ShowCHinese(15,6,64); //ָ
					OLED_ShowCHinese(33,6,65); //��
					OLED_ShowCHinese(51,6,68); //��
					OLED_ShowCHinese(69,6,69); //��
					OLED_ShowNum(87,6,ValidN,5,16);
					delay_ms(1500);
					OLED_Clear();
					return 1;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(400);
		if(i==5)//����5��û�а���ָ���˳�
		{
			OLED_Clear();
			break;	
		}				
	}
	return 0;
}




//ˢָ��
uint32_t press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		//BEEP=1;//�򿪷�����	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			//BEEP=0;//�رշ�����	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				printf("ָ�ƿ�ƥ��ɹ���,ID:%d  ƥ��÷�:%d\r\n",seach.pageID,seach.mathscore);
			}
			else 
			{
				printf("ƥ��ʧ�ܣ�\r\n");
				return 1;
			}
								
		}
		else
			ShowErrMessage(ensure);
	 //BEEP=0;//�رշ�����
	 delay_ms(600);
	}	
	return 0;
}

//ɾ��ָ��
uint32_t Del_FR(void)
{
	u8  ensure;
	u16 num = 0;
	u16 vnum1; //ɾ��ǰ��ָ�Ƹ����Ա�
	u16 vnum2; //ɾ��ǰ��ָ�Ƹ����Ա�	
	OLED_Clear();
	OLED_ShowCHinese(10,0,70);  //��
	OLED_ShowCHinese(28,0,71); //��
	OLED_ShowCHinese(46,0,72); //��
	OLED_ShowCHinese(64,0,73); //ɾ
	OLED_ShowCHinese(82,0,74); //��
	OLED_ShowString(100,0,"I",16);//I
	OLED_ShowString(108,0,"D",16);//D
	OLED_ShowString(0,6,"ID<300",16); //ID<300
	OLED_ShowString(81,6,"#",16); //D
	OLED_ShowCHinese(90,6,16); //ȷ
	OLED_ShowCHinese(108,6,17);//��
	delay_ms(50);	
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���	
	vnum1=ValidN;
	
	printf("num1 = %d\r\n",num);

	num=GET_NUM();//��ȡ���ص���ֵ
	
	printf("num2 = %d\r\n",num);
	
	if(num==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(num,1);//ɾ������ָ��
	if(ensure==0)
	{
			PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���	
			vnum2=ValidN;	
			if(vnum1!=vnum2)
			{	
				OLED_Clear();
				OLED_ShowCHinese(10,3,73);  //ɾ
				OLED_ShowCHinese(28,3,74);  //��
				OLED_ShowCHinese(46,3,2);  //ָ
				OLED_ShowCHinese(64,3,3);  //��
				OLED_ShowCHinese(82,3,49);  //��
				OLED_ShowCHinese(100,3,50); //��	
				delay_ms(1000);
				OLED_Clear();
				PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
				OLED_ShowCHinese(20,4,64); //ָ
				OLED_ShowCHinese(38,4,65); //��
				OLED_ShowCHinese(56,4,68); //��
				OLED_ShowCHinese(74,4,69); //��
				OLED_ShowNum(90,4,ValidN,3,16);
				delay_ms(2000);	
				OLED_Clear();		
				return 1;
			}
			else
			{
				OLED_Clear();
				OLED_ShowCHinese(10,3,73);  //ɾ
				OLED_ShowCHinese(28,3,74);  //��
				OLED_ShowCHinese(46,3,2);  //ָ
				OLED_ShowCHinese(64,3,3);  //��
				OLED_ShowCHinese(82,3,47);  //��
				OLED_ShowCHinese(100,3,48); //��	
				delay_ms(1000);
				OLED_Clear();
				PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
				OLED_ShowCHinese(20,4,64); //ָ
				OLED_ShowCHinese(38,4,65); //��
				OLED_ShowCHinese(56,4,68); //��
				OLED_ShowCHinese(74,4,69); //��
				OLED_ShowNum(90,4,ValidN,3,16);
				delay_ms(2000);	
				OLED_Clear();
				return 2;				
			}
	}
	return 0;
}



