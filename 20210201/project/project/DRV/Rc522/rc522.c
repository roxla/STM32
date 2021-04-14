#include <myhead.h>
					  
//SPI????,
void MF522SPI_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//??GPIOC ?GPIOD??

	//GPIO PC6 ???????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					//PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//??
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//???
	
	//GPIO PC8 ???????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;				//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//??
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//???
	
	//GPIO PD6 PD7?????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		//PD6 PD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//??
	GPIO_Init(GPIOD, &GPIO_InitStructure);						//???
 
}   


//SPI���ͽ��պ���
uint8_t MF522SPI_ReadWriteByte(uint8_t TxData)
{		 			 

	uint8_t i=0,d=0;
	
	//����ʱ����
	MF522_SCK=0;
	my_delay_us(5);
	for(i=0; i<8; i++)
	{
		//��Ҫ���͵�һλ���ݷŵ�MOSI
		if(TxData & (1<<(7-i)))
		{
			 MF522_MOSI=1;
		}
		else
		{
			 MF522_MOSI=0;
		}

		//ʱ�������ߣ������ط��͸�λ����
		MF522_SCK=1;
		my_delay_us(5);
	
		//ͬʱ����MISO�ϵ�һλ����
		if(MF522_MISO)
		{
			 d|=1<<(7-i);
		}
			
		//����ʱ����
		MF522_SCK=0;
		my_delay_us(5);
	}	
	
	
	return d;	
}

//RC522��ʼ��
void MF522_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	MF522SPI_Init();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//??GPIOC ?GPIOD??

	//GPIO PC11 ???????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					//PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//??
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//???
	
	MF522_RST=1; //??

	MFRC522_Reset();         

}

void MF522SPI_Send(uint8_t val)  
{ 
	MF522SPI_ReadWriteByte(val);
}
//
uint8_t MF522SPI_Recv(void)  
{ 
	uint8_t temp; 

	temp=MF522SPI_ReadWriteByte(0xFF);

	return temp; 
}

//??????MFRC522?????????????
//?????addr--??????val--?????
void Write_MFRC522(uint8_t addr, uint8_t val) 
{
	//?????0XXXXXX0  
	MF522_NSS=0;  
	my_delay_us(5);
	MF522SPI_Send((addr<<1)&0x7E);  
	MF522SPI_Send(val);  
	my_delay_us(5);
	MF522_NSS=1; 
}
//??????MFRC522?????????????
//?????addr--?????
//? ? ?????????????? 
uint8_t Read_MFRC522(uint8_t addr) 
{  
	uint8_t val;
	//?????1XXXXXX0   
	MF522_NSS=0; 
	my_delay_us(5);
	MF522SPI_Send(((addr<<1)&0x7E)|0x80);   
	val=MF522SPI_Recv();   
	my_delay_us(5);
	MF522_NSS=1; 
	//   
	return val;  
}
//??????????????
//??????RC522????
//?????reg--?????;mask--???
void SetBitMask(uint8_t reg, uint8_t mask)   
{     
	uint8_t tmp=0;
	//     
	tmp=Read_MFRC522(reg);     
	Write_MFRC522(reg,tmp|mask);  // set bit mask 
}
//??????RC522????
//?????reg--?????;mask--???
void ClearBitMask(uint8_t reg, uint8_t mask)   
{     
	uint8_t tmp=0;
	//     
	tmp=Read_MFRC522(reg);     
	Write_MFRC522(reg,tmp&(~mask));  //clear bit mask 
}
//?????????,?????????????????1ms???
void AntennaOn(void) 
{  
	uint8_t temp;
	//   
	temp=Read_MFRC522(TxControlReg);  
	if ((temp&0x03)==0)  
	{   
		SetBitMask(TxControlReg,0x03);  
	}
}
//?????????,?????????????????1ms???
void AntennaOff(void) 
{  
	ClearBitMask(TxControlReg,0x03);
}
//???????MFRC522
void MFRC522_Reset(void) 
{ 
	//???????
	MF522_RST=1;
	my_delay_us(1);
	MF522_RST=0;
	my_delay_us(1);
	MF522_RST=1;
	my_delay_us(1); 
	//???   
	Write_MFRC522(CommandReg, PCD_RESETPHASE); 
	
	//Timer: TPrescaler*TreloadVal/6.78MHz = 0xD3E*0x32/6.78=25ms     
	Write_MFRC522(TModeReg,0x8D);				//TAuto=1???????,?????????4????????4?
	//Write_MFRC522(TModeReg,0x1D);				//TAutoRestart=1???????,0x0D3E?0.5ms?????//test    
	Write_MFRC522(TPrescalerReg,0x3E); 			//??????8?     
	Write_MFRC522(TReloadRegL,0x32);			//?????8?                
	Write_MFRC522(TReloadRegH,0x00);			//?????8?       
	Write_MFRC522(TxAutoReg,0x40); 				//100%ASK     
	Write_MFRC522(ModeReg,0x3D); 				//CRC???0x6363
	Write_MFRC522(CommandReg,0x00);				//??MFRC522  
	//Write_MFRC522(RFCfgReg, 0x7F);    		//RxGain = 48dB???????      
	AntennaOn();          						//???? 		
}
//

//?????RC522?ISO14443???
//?????command--MF522???
//					sendData--??RC522????????
//					sendLen--???????
//					BackData--??????????
//					BackLen--????????
//? ? ??????MI_O
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen) 
{
	uint8_t  status=MI_ERR;
	uint8_t  irqEn=0x00;
	uint8_t  waitIRq=0x00;
	uint8_t  lastBits;
	uint8_t  n;
	uint16_t i;
	//?????????
	switch (command)     
	{         
	case PCD_AUTHENT:  		//????   
		irqEn 	= 0x12;			//    
		waitIRq = 0x10;			//    
		break;
	case PCD_TRANSCEIVE: 	//??FIFO???      
		irqEn 	= 0x77;			//    
		waitIRq = 0x30;			//    
		break;      
	default:    
		break;     
	}
	//
	Write_MFRC522(ComIEnReg, irqEn|0x80);		//??????     
	ClearBitMask(ComIrqReg, 0x80);  				//?????????               	
	SetBitMask(FIFOLevelReg, 0x80);  				//FlushBuffer=1, FIFO???
	Write_MFRC522(CommandReg, PCD_IDLE); 		//?MFRC522??   
	//?FIFO?????     
	for (i=0; i<sendLen; i++)
	Write_MFRC522(FIFODataReg, sendData[i]);
	//????
	Write_MFRC522(CommandReg, command);
	//??????     
	if (command == PCD_TRANSCEIVE)					//?????????,MFRC522?????????      
	SetBitMask(BitFramingReg, 0x80);  		//StartSend=1,transmission of data starts      
	//????????     
	i = 10000; //i???????????M1???????25ms     
	do      
	{        
		n = Read_MFRC522(ComIrqReg);
		//irq_regdata=n;	//test         
		i--;
		//wait_count=i;		//test		     
	}while ((i!=0) && !(n&0x01) && !(n&waitIRq));	//??????n=0x64
	//????
	ClearBitMask(BitFramingReg, 0x80);   		//StartSend=0
	//???25ms????
	if (i != 0)     
	{            
		if(!(Read_MFRC522(ErrorReg) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr         
		{            
			if (n & irqEn & 0x01)			//                  
			status = MI_NOTAGERR;		//
			//
			if (command == PCD_TRANSCEIVE)             
			{                 
				n = Read_MFRC522(FIFOLevelReg);		//n=0x02                
				lastBits = Read_MFRC522(ControlReg) & 0x07;	//lastBits=0               
				if (lastBits!=0)                         
				*backLen = (n-1)*8 + lastBits; 
				else
				*backLen = n*8;									//backLen=0x10=16
				//
				if (n == 0)                         
				n = 1;                        
				if (n > MAX_LEN)         
				n = MAX_LEN;
				//
				for (i=0; i<n; i++)                 
				backData[i] = Read_MFRC522(FIFODataReg); 
			}
			//
			status = MI_OK;		
		}
		else
		status = MI_ERR;
	}	
	//
	Write_MFRC522(ControlReg,0x80);				//timer stops     
	Write_MFRC522(CommandReg, PCD_IDLE);	//
	//
	return status;
}
/////////////////////////////////////////////////////////////////////
//?    ?:??
//????: req_code[IN]:????
//                0x52 = ?????????14443A????
//                0x26 = ??????????
//          pTagType[OUT]:??????
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////	
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType)
{  
	uint8_t  status;    
	uint16_t backBits;   //????????
	//   
	Write_MFRC522(BitFramingReg, 0x07);  //TxLastBists = BitFramingReg[2..0]   
	TagType[0] = reqMode;  
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits); 
	// 
	if ((status != MI_OK) || (backBits != 0x10))  
	{       
		status = MI_ERR;
	}
	//  
	return status; 
}
//??????????????????????
//?????serNum--??4??????,?5???????
//? ? ??????MI_OK
uint8_t MFRC522_Anticoll(uint8_t *serNum) 
{     
	uint8_t  status;     
	uint8_t  i;     
	uint8_t  serNumCheck=0;     
	uint16_t unLen;
	//           
	ClearBitMask(Status2Reg, 0x08);  			//TempSensclear     
	ClearBitMask(CollReg,0x80);   				//ValuesAfterColl  
	Write_MFRC522(BitFramingReg, 0x00);  	//TxLastBists = BitFramingReg[2..0]
	serNum[0] = PICC_ANTICOLL1;     
	serNum[1] = 0x20;     
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
	//      
	if (status == MI_OK)
	{   
		//??????   
		for(i=0;i<4;i++)   
		serNumCheck^=serNum[i];
		//
		if(serNumCheck!=serNum[i])        
		status=MI_ERR;
	}
	SetBitMask(CollReg,0x80);  //ValuesAfterColl=1
	//      
	return status;
}
//??????MF522??CRC
//?????pIndata--???CRC????len--?????pOutData--???CRC??
void CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData) 
{     
	uint16_t i;
	uint8_t  n;
	//      
	ClearBitMask(DivIrqReg, 0x04);   			//CRCIrq = 0     
	SetBitMask(FIFOLevelReg, 0x80);   		//?FIFO??     
	Write_MFRC522(CommandReg, PCD_IDLE);   
	//?FIFO?????      
	for (i=0; i<len; i++)
	Write_MFRC522(FIFODataReg, *(pIndata+i));
	//??RCR??
	Write_MFRC522(CommandReg, PCD_CALCCRC);
	//??CRC????     
	i = 1000;     
	do      
	{         
		n = Read_MFRC522(DivIrqReg);         
		i--;     
	}while ((i!=0) && !(n&0x04));   //CRCIrq = 1
	//??CRC????     
	pOutData[0] = Read_MFRC522(CRCResultRegL);     
	pOutData[1] = Read_MFRC522(CRCResultRegH);
	Write_MFRC522(CommandReg, PCD_IDLE);
}
//????????????????
//?????serNum--??????
//? ? ?????????
uint8_t MFRC522_SelectTag(uint8_t *serNum) 
{     
	uint8_t  i;     
	uint8_t  status;     
	uint8_t  size;     
	uint16_t recvBits;     
	uint8_t  buffer[9];
	//     
	buffer[0] = PICC_ANTICOLL1;	//???1     
	buffer[1] = 0x70;
	buffer[6] = 0x00;						     
	for (i=0; i<4; i++)					
	{
		buffer[i+2] = *(serNum+i);	//buffer[2]-buffer[5]?????
		buffer[6]  ^=	*(serNum+i);	//????
	}
	//
	CalulateCRC(buffer, 7, &buffer[7]);	//buffer[7]-buffer[8]?RCR???
	ClearBitMask(Status2Reg,0x08);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
	//
	if ((status == MI_OK) && (recvBits == 0x18))    
	size = buffer[0];     
	else    
	size = 0;
	//	     
	return size; 
}
//???????????
//?????authMode--??????
//					0x60 = ??A??
//					0x61 = ??B??
//					BlockAddr--???
//					Sectorkey--????
//					serNum--??????4??
//? ? ??????MI_OK
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum) 
{     
	uint8_t  status;     
	uint16_t recvBits;     
	uint8_t  i;  
	uint8_t  buff[12];    
	//????+???+????+????     
	buff[0] = authMode;		//????     
	buff[1] = BlockAddr;	//???     
	for (i=0; i<6; i++)
	buff[i+2] = *(Sectorkey+i);	//????
	//
	for (i=0; i<4; i++)
	buff[i+8] = *(serNum+i);		//????
	//
	status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
	//      
	if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
	status = MI_ERR;
	//
	return status;
}
//?????????
//?????blockAddr--???;recvData--??????
//? ? ??????MI_OK
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData) 
{     
	uint8_t  status;     
	uint16_t unLen;
	//      
	recvData[0] = PICC_READ;     
	recvData[1] = blockAddr;     
	CalulateCRC(recvData,2, &recvData[2]);     
	status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
	//
	if ((status != MI_OK) || (unLen != 0x90))
	status = MI_ERR;
	//
	return status;
}
//?????????
//?????blockAddr--???;writeData--???16????
//? ? ??????MI_OK
uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData) 
{     
	uint8_t  status;     
	uint16_t recvBits;     
	uint8_t  i;  
	uint8_t  buff[18];
	//           
	buff[0] = PICC_WRITE;     
	buff[1] = blockAddr;     
	CalulateCRC(buff, 2, &buff[2]);     
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
	//
	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
	status = MI_ERR;
	//
	if (status == MI_OK)     
	{         
		for (i=0; i<16; i++)  //?FIFO?16Byte??                     
		buff[i] = *(writeData+i);
		//                     
		CalulateCRC(buff, 16, &buff[16]);         
		status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);           
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))               
		status = MI_ERR;         
	}          
	return status;
}
//???????????????
void MFRC522_Halt(void) 
{    
	uint16_t unLen;     
	uint8_t  buff[4];
	//       
	buff[0] = PICC_HALT;     
	buff[1] = 0;     
	CalulateCRC(buff, 2, &buff[2]);       
	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}

