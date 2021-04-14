#include "w25q128.h"

void w25qxx_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// 1.ʹ������GPIOB��SPI1��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// 2.����SPIӲ����GPIO��PB3/4/5��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 3.���������ӵ���Ӧ���ù�������
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	// 4.����ѡ������(PB14)�����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 5.ѡ��PB14��ʼ��ƽΪ�ߵ�ƽ����ʱ��ͼ�д𰸡���
	W25QXX_SS=1;
	
	// 6.SPI����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// ȫ˫��ͨ��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		// ��������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 8λ����λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		// CPOL=1�����ݵ�ǰ�ӻ�оƬ����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	// CPHA=1�����ݵ�ǰ�ӻ�оƬ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		// Ƭѡѡ��Ĭ��ͨ���������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	// Ӳ��ʱ�ӣ�84MHz/4=21MHz
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	// ���ݴӻ�оƬ����
	//SPI_InitStructure.SPI_CRCPolynomial = 7;	//��ǰ�ӻ�оƬ��֧��CRC
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// 7.ʹ��SPI1
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_SendByte(uint8_t byte)
{
	/*!< Loop while DR register in not emplty ���ȴ����ͻ�����Ϊ��*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*!< Send byte through the SPI1 peripheral ��ͨ��SPI1�����ֽ�*/
	SPI_I2S_SendData(SPI1, byte);

	/*!< Wait to receive a byte ��������ͬ��ͨ�ţ��ȴ������ֽ�*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/*!< Return the byte read from the SPI bus ���ɹ����մӻ����ֽڲ�����*/
	return SPI_I2S_ReceiveData(SPI1);
}

uint16_t w25qxx_read_id(void)
{
	uint16_t id=0;

	//Ƭѡ��������
	W25QXX_SS=0;
	
	//����0x90
	SPI1_SendByte(0x90);

	//����24bit�ĵ�ַ��ȫΪ0
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	
	//��ȡ����id����������Ϊ�������
	id = SPI1_SendByte(0xFF)<<8;
	
	//��ȡ�豸id
	id|= SPI1_SendByte(0xFF);

	//Ƭѡ��������	
	W25QXX_SS=1;
	
	return id;
}


void w25qxx_read_data(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	//Ƭѡ��������
	W25QXX_SS=0;
	
	//����0x03
	SPI1_SendByte(0x03);

	//����24bit�ĵ�ַ
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//��ȡ����
	while(len--)
		*pbuf++= SPI1_SendByte(0xFF);

	//Ƭѡ��������	
	W25QXX_SS=1;
}


//��״̬�Ĵ���1
uint8_t w25qxx_read_status1(void)
{
	uint8_t status=0;
	
	//Ƭѡ��������
	W25QXX_SS=0;
	
	//����0x05
	SPI1_SendByte(0x05);

	//���������������ȡ״̬�Ĵ�����ֵ
	status=SPI1_SendByte(0xFF);
	
	//Ƭѡ��������	
	W25QXX_SS=1;
	
	return status;
}


//дʹ��
void w25qxx_write_enable(void)
{
	//Ƭѡ��������
	W25QXX_SS=0;
	
	
	//����0x06
	SPI1_SendByte(0x06);


	//Ƭѡ��������	
	W25QXX_SS=1;
}


//дʧ��
void w25qxx_write_disable(void)
{
	//Ƭѡ��������
	W25QXX_SS=0;
	
	//����0x04
	SPI1_SendByte(0x04);


	//Ƭѡ��������	
	W25QXX_SS=1;
}


//����
void w25qxx_erase_sector(uint32_t addr)
{
	uint8_t status=0;

	//ִ��дʹ��ָ��
	w25qxx_write_enable();
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_us(10);

	//Ƭѡ��������
	W25QXX_SS=0;
	
	
	//����0x20
	SPI1_SendByte(0x20);
	
	//����24bit�ĵ�ַ
	SPI1_SendByte((addr>>16)&0xFF);	// 1111 1111
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);

	//Ƭѡ��������	
	W25QXX_SS=1;
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_us(10);
	
	
	//��鵱ǰ�Ƿ��Ѿ������ɹ�
	while(1)
	{
		status=w25qxx_read_status1();
		
		if((status & 0x01) ==0)
			break;
	
		//ÿ��1������в�ѯ
		delay_ms(1);
	
	}
	
	//����д����
	w25qxx_write_disable();
}

void w25qxx_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	uint8_t status=0;

	//ִ��дʹ��ָ��
	w25qxx_write_enable();
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_us(10);

	//Ƭѡ��������
	W25QXX_SS=0;
	
	
	//����0x02
	SPI1_SendByte(0x02);
	
	//����24bit�ĵ�ַ
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//��������
	while(len--)
		SPI1_SendByte(*pbuf++);

	//Ƭѡ��������	
	W25QXX_SS=1;
	
	//��ʱһ�������Ƭѡ�����ܹ������ָߵ�ƽһ��ʱ�䣬��w25qxxʶ��
	delay_us(10);
	
	
	//��鵱ǰ�Ƿ��Ѿ�д��ɹ�
	while(1)
	{
		status=w25qxx_read_status1();
		
		if((status & 0x01) ==0)
			break;
	
		//ÿ��1������в�ѯ
		delay_ms(1);
	}
	//����д����
	w25qxx_write_disable();
}
