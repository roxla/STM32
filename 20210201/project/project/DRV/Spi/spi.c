#include <myhead.h>

void spi_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct; 
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	/*STM32F103C8T6оƬ��Ӳ��SPI: PB3 4 5 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//����ӳ��ΪI2C����
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//PB14Ƭѡ ----- ���
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	W25Q128_CS = 1;//�ر�Ƭѡ
	
	//��ʼ��SPI
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����ȫ˫��
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//���豸
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//���ݳ���8bit
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//���ԣ�Ĭ�ϸߵ�ƽ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//��λ���ڶ������ز���
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//Ƭѡ�ź����������
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//84M/16 = 5.25M
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//��λ�ȳ�
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//ʹ��SPI
	SPI_Cmd(SPI1,ENABLE);
}

//���ͽ�������
//data����Ҫ�������ݣ������յ�������
u8 spi_send_recv_byte(u8 data)
{
	//�ȴ���һ�����ݷ������
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
	//��������
	SPI_I2S_SendData(SPI1,data);
	//���պͷ���ͬʱ����
	//�ȴ��յ�����
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
	//��������
	return SPI_I2S_ReceiveData(SPI1);
}

//��ȡW25Q128��ID
u16 w25q128_read_id(void)
{
	u16 id = 0;
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����90H
	spi_send_recv_byte(0x90);
	
	//����24λ0��ַ
	spi_send_recv_byte(0x00);
	spi_send_recv_byte(0x00);
	spi_send_recv_byte(0x00);
	
	//���ճ���ID���豸ID
	id |= spi_send_recv_byte(0xff)<<8;
	id |= spi_send_recv_byte(0xff);
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	return id;
}

//������
void w25q128_read_data(u32 addr,u8 *data,u8 len)
{
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����03H
	spi_send_recv_byte(0x03);
	
	//����24λ��ַ,��λ�ȷ�
	spi_send_recv_byte((addr>>16)&0xff);//16~23λ
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//��ȡ����
	while(len--){
		*data++ = spi_send_recv_byte(0xff);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
}

//дʹ�ܿ���/��ֹ  0 --- ����дʹ��  1 --- �ر�дʹ��
void w25q128_write_enable_disable(u8 enable)
{
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	if(enable){
		spi_send_recv_byte(0x04);
	}
	else{
		spi_send_recv_byte(0x06);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
}

//��״̬�Ĵ���1
u8 w25q128_read_status1(void)
{
	u8 status = 0;
	
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����05H
	spi_send_recv_byte(0x05);
	//��ȡ�Ĵ���
	status = spi_send_recv_byte(0xff);
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	
	return status;
}

//��������
void w25q128_erase_sector(u32 addr)
{
	//����дʹ��
	w25q128_write_enable_disable(0);
	
	//��ʱ����Ƭѡ�źŲ�����Ч
	my_delay_us(50);
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����20H
	spi_send_recv_byte(0x20);
	//����24λ��ַ,��λ�ȷ�
	spi_send_recv_byte((addr>>16)&0xff);//16~23λ
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	my_delay_us(50);
	
	//�ȴ��������
	while(w25q128_read_status1()&0x1){
		my_delay_ms(1);
	}
	
	my_delay_us(50);
	//�ر�дʹ��
	w25q128_write_enable_disable(1);
}

//дһҳ
void w25q128_write_page(u32 addr,u8 *data,u8 len)
{
		//����дʹ��
	w25q128_write_enable_disable(0);
	
	//��ʱ����Ƭѡ�źŲ�����Ч
	my_delay_us(50);
	//Ƭѡѡ��
	W25Q128_CS = 0;
	
	//����02H
	spi_send_recv_byte(0x02);
	//����24λ��ַ,��λ�ȷ�
	spi_send_recv_byte((addr>>16)&0xff);//16~23λ
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//����д������
	while(len--){
		spi_send_recv_byte(*data++);
	}
	
	//�ر�Ƭѡ
	W25Q128_CS = 1;
	my_delay_us(50);
	
	//�ȴ�д���
	while(w25q128_read_status1()&0x1){
		my_delay_ms(1);
	}
	
	my_delay_us(50);
	//�ر�дʹ��
	w25q128_write_enable_disable(1);
}
