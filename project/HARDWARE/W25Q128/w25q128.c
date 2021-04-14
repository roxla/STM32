#include "w25q128.h"

void w25qxx_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// 1.使能所有GPIOB和SPI1的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// 2.配置SPI硬件的GPIO（PB3/4/5），复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 3.将引脚连接到对应复用功能引脚
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	// 4.配置选择引脚(PB14)，输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 5.选择PB14初始电平为高电平。（时序图有答案。）
	W25QXX_SS=1;
	
	// 6.SPI配置
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 全双工通信
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		// 配置主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 8位数据位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		// CPOL=1，根据当前从机芯片设置
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	// CPHA=1，根据当前从机芯片设置
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		// 片选选择，默认通过代码控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	// 硬件时钟：84MHz/4=21MHz
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	// 根据从机芯片设置
	//SPI_InitStructure.SPI_CRCPolynomial = 7;	//当前从机芯片不支持CRC
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// 7.使能SPI1
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_SendByte(uint8_t byte)
{
	/*!< Loop while DR register in not emplty ，等待发送缓冲区为空*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*!< Send byte through the SPI1 peripheral ，通过SPI1发送字节*/
	SPI_I2S_SendData(SPI1, byte);

	/*!< Wait to receive a byte ，由于是同步通信，等待接收字节*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/*!< Return the byte read from the SPI bus ，成功接收从机的字节并返回*/
	return SPI_I2S_ReceiveData(SPI1);
}

uint16_t w25qxx_read_id(void)
{
	uint16_t id=0;

	//片选引脚拉低
	W25QXX_SS=0;
	
	//发送0x90
	SPI1_SendByte(0x90);

	//发送24bit的地址，全为0
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	
	//读取厂商id，参数可以为任意参数
	id = SPI1_SendByte(0xFF)<<8;
	
	//读取设备id
	id|= SPI1_SendByte(0xFF);

	//片选引脚拉高	
	W25QXX_SS=1;
	
	return id;
}


void w25qxx_read_data(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	//片选引脚拉低
	W25QXX_SS=0;
	
	//发送0x03
	SPI1_SendByte(0x03);

	//发送24bit的地址
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//读取数据
	while(len--)
		*pbuf++= SPI1_SendByte(0xFF);

	//片选引脚拉高	
	W25QXX_SS=1;
}


//读状态寄存器1
uint8_t w25qxx_read_status1(void)
{
	uint8_t status=0;
	
	//片选引脚拉低
	W25QXX_SS=0;
	
	//发送0x05
	SPI1_SendByte(0x05);

	//发送任意参数，读取状态寄存器的值
	status=SPI1_SendByte(0xFF);
	
	//片选引脚拉高	
	W25QXX_SS=1;
	
	return status;
}


//写使能
void w25qxx_write_enable(void)
{
	//片选引脚拉低
	W25QXX_SS=0;
	
	
	//发送0x06
	SPI1_SendByte(0x06);


	//片选引脚拉高	
	W25QXX_SS=1;
}


//写失能
void w25qxx_write_disable(void)
{
	//片选引脚拉低
	W25QXX_SS=0;
	
	//发送0x04
	SPI1_SendByte(0x04);


	//片选引脚拉高	
	W25QXX_SS=1;
}


//擦除
void w25qxx_erase_sector(uint32_t addr)
{
	uint8_t status=0;

	//执行写使能指令
	w25qxx_write_enable();
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_us(10);

	//片选引脚拉低
	W25QXX_SS=0;
	
	
	//发送0x20
	SPI1_SendByte(0x20);
	
	//发送24bit的地址
	SPI1_SendByte((addr>>16)&0xFF);	// 1111 1111
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);

	//片选引脚拉高	
	W25QXX_SS=1;
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_us(10);
	
	
	//检查当前是否已经擦除成功
	while(1)
	{
		status=w25qxx_read_status1();
		
		if((status & 0x01) ==0)
			break;
	
		//每隔1毫秒进行查询
		delay_ms(1);
	
	}
	
	//开启写保护
	w25qxx_write_disable();
}

void w25qxx_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	uint8_t status=0;

	//执行写使能指令
	w25qxx_write_enable();
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_us(10);

	//片选引脚拉低
	W25QXX_SS=0;
	
	
	//发送0x02
	SPI1_SendByte(0x02);
	
	//发送24bit的地址
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//发送数据
	while(len--)
		SPI1_SendByte(*pbuf++);

	//片选引脚拉高	
	W25QXX_SS=1;
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_us(10);
	
	
	//检查当前是否已经写入成功
	while(1)
	{
		status=w25qxx_read_status1();
		
		if((status & 0x01) ==0)
			break;
	
		//每隔1毫秒进行查询
		delay_ms(1);
	}
	//开启写保护
	w25qxx_write_disable();
}
