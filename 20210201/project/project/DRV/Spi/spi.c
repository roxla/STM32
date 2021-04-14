#include <myhead.h>

void spi_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct; 
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	/*STM32F103C8T6芯片的硬件SPI: PB3 4 5 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//复用映射为I2C功能
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//PB14片选 ----- 输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	W25Q128_CS = 1;//关闭片选
	
	//初始化SPI
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//两线全双工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//主设备
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//数据长度8bit
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//极性，默认高电平
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//相位，第二个边沿采样
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//片选信号由软件控制
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//84M/16 = 5.25M
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//高位先出
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//使能SPI
	SPI_Cmd(SPI1,ENABLE);
}

//发送接收数据
//data就是要发的数据，返回收到的数据
u8 spi_send_recv_byte(u8 data)
{
	//等待上一个数据发送完成
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);
	//发送数据
	SPI_I2S_SendData(SPI1,data);
	//接收和发送同时进行
	//等待收到数据
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);
	//接收数据
	return SPI_I2S_ReceiveData(SPI1);
}

//读取W25Q128的ID
u16 w25q128_read_id(void)
{
	u16 id = 0;
	
	//片选选中
	W25Q128_CS = 0;
	
	//发送90H
	spi_send_recv_byte(0x90);
	
	//发送24位0地址
	spi_send_recv_byte(0x00);
	spi_send_recv_byte(0x00);
	spi_send_recv_byte(0x00);
	
	//接收厂商ID和设备ID
	id |= spi_send_recv_byte(0xff)<<8;
	id |= spi_send_recv_byte(0xff);
	
	//关闭片选
	W25Q128_CS = 1;
	
	return id;
}

//读数据
void w25q128_read_data(u32 addr,u8 *data,u8 len)
{
	//片选选中
	W25Q128_CS = 0;
	
	//发送03H
	spi_send_recv_byte(0x03);
	
	//发送24位地址,高位先发
	spi_send_recv_byte((addr>>16)&0xff);//16~23位
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//读取数据
	while(len--){
		*data++ = spi_send_recv_byte(0xff);
	}
	
	//关闭片选
	W25Q128_CS = 1;
}

//写使能开启/禁止  0 --- 开启写使能  1 --- 关闭写使能
void w25q128_write_enable_disable(u8 enable)
{
	//片选选中
	W25Q128_CS = 0;
	
	if(enable){
		spi_send_recv_byte(0x04);
	}
	else{
		spi_send_recv_byte(0x06);
	}
	
	//关闭片选
	W25Q128_CS = 1;
}

//读状态寄存器1
u8 w25q128_read_status1(void)
{
	u8 status = 0;
	
	//片选选中
	W25Q128_CS = 0;
	
	//发送05H
	spi_send_recv_byte(0x05);
	//读取寄存器
	status = spi_send_recv_byte(0xff);
	
	//关闭片选
	W25Q128_CS = 1;
	
	return status;
}

//擦除扇区
void w25q128_erase_sector(u32 addr)
{
	//开启写使能
	w25q128_write_enable_disable(0);
	
	//延时，让片选信号操作生效
	my_delay_us(50);
	//片选选中
	W25Q128_CS = 0;
	
	//发送20H
	spi_send_recv_byte(0x20);
	//发送24位地址,高位先发
	spi_send_recv_byte((addr>>16)&0xff);//16~23位
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//关闭片选
	W25Q128_CS = 1;
	my_delay_us(50);
	
	//等待擦除完成
	while(w25q128_read_status1()&0x1){
		my_delay_ms(1);
	}
	
	my_delay_us(50);
	//关闭写使能
	w25q128_write_enable_disable(1);
}

//写一页
void w25q128_write_page(u32 addr,u8 *data,u8 len)
{
		//开启写使能
	w25q128_write_enable_disable(0);
	
	//延时，让片选信号操作生效
	my_delay_us(50);
	//片选选中
	W25Q128_CS = 0;
	
	//发送02H
	spi_send_recv_byte(0x02);
	//发送24位地址,高位先发
	spi_send_recv_byte((addr>>16)&0xff);//16~23位
	spi_send_recv_byte((addr>>8)&0xff);//8~15
	spi_send_recv_byte((addr>>0)&0xff);//0~7
	
	//发送写的数据
	while(len--){
		spi_send_recv_byte(*data++);
	}
	
	//关闭片选
	W25Q128_CS = 1;
	my_delay_us(50);
	
	//等待写完成
	while(w25q128_read_status1()&0x1){
		my_delay_ms(1);
	}
	
	my_delay_us(50);
	//关闭写使能
	w25q128_write_enable_disable(1);
}
