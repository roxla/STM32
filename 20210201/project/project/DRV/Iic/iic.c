#include <myhead.h>

//初始化IO口
void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;//开漏
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//PB8 PB9
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
}

//设置SDA的方向
void iic_sda_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = mode;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PB9 --- SDA
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//起始信号
void iic_start(void)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	//总线空闲
	SCL = 1;
	SDA_OUT = 1;
	my_delay_us(5);
	
	//SDA拉低
	SDA_OUT = 0;
	my_delay_us(5);
	
	//SDA拉低之后，在拉低SDA，钳住总线
	SCL = 0;
}

//停止信号
void iic_stop(void)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;
	my_delay_us(5);
	
	//拉高SCL
	SCL = 1;
	my_delay_us(5);
	
	//拉高SDA，此时SCL和SDA均为高，总线空闲
	SDA_OUT = 1;
}

//等待从设备应答，返回0表示有效应答，非0无效应答
u8 iic_wait_ack(void)
{
	u8 ack = 0;
	
	//SDA为输入模式
	iic_sda_mode(GPIO_Mode_IN);
	
	//拉高SCL，允许操作SDA
	SCL = 1;
	my_delay_us(5);
	
	//读取SDA的电平
	if(SDA_IN==1){
		ack = 1;//无效应答
		//停止传输
		iic_stop();
	}
	else
		ack = 0;
	
	//钳住总线
	SCL = 0;
	my_delay_us(5);
	
	return ack;
}

//产生应答信号，传参ack=0有效应答，否则无效应答
void iic_ack(u8 ack)
{
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	my_delay_us(5);
	
	//设置SDA电平
	if(ack)
		SDA_OUT = 1;//无效应答
	else
		SDA_OUT = 0;//有效应答
	
	my_delay_us(5);
	//拉高SCL，保持让对方获取应答
	SCL = 1;
	my_delay_us(5);
	
	SCL = 0;
}

//发送一个字节 --------- 高位先出
void iic_send_byte(u8 txd)
{
	u8 i;
	
	//SDA输出模式
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	my_delay_us(5);
	
	//从高位到低位依次发送每一位
	for(i=0;i<8;i++){
		if(txd&1<<(7-i))
			SDA_OUT = 1;//当前位是1，给高电平
		else
			SDA_OUT = 0;
		
		my_delay_us(5);
		
		//拉高时钟线，对方读走这一位
		SCL = 1;
		//延时保持数据线稳定
		my_delay_us(5);
		//拉低时钟线，准备发送下一位
		SCL = 0;
	}
}

//接收一个字节
u8 iic_recv_byte(void)
{
	u8 rxd = 0,i;
	
	//SDA输入模式
	iic_sda_mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	//从高到低依次接收每一位
	for(i=0;i<8;i++){
		//等待对方设置好电平
		my_delay_us(5);
		//SCL高电平期间读取一位数据
		SCL = 1;
		//当高电平修改rxd
		if(SDA_IN)
			rxd |= 1<<(7-i);
		
		//准备接收下一位
		my_delay_us(5);
		SCL = 0;
	}
	
	return rxd;
}

//at24c02写1字节的数据
void at24c02_write_byte(u8 addr,u8 data)
{
	u8 ack;
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和写信号(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\r\n");
		return;
	}
	
	//发送写数据的芯片内部地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\r\n");
		return;
	}
	
	//发送写的数据
	iic_send_byte(data);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\r\n");
		return;
	}
	
	//停止信号
	iic_stop();
}

//at24c02连续写的数据
void at24c02_write_page(u8 addr,u8 *data,u8 len)
{
	u8 ack;
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和写信号(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\r\n");
		return;
	}
	
	//发送写数据的芯片内部地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\r\n");
		return;
	}
	
	//连续发送写的数据
	while(len--){
		iic_send_byte(*data++);
		//等待ACK
		ack = iic_wait_ack();
		if(ack==1){
			printf("none ack 3!\r\n");
			return;
		}
	}
	
	//停止信号
	iic_stop();
}

//at24c02读1字节的数据
u8 at24c02_read_byte(u8 addr)
{
	u8 ack,data;
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和写信号(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\n");
		return 0;
	}
	
	//发送读数据的芯片内部地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\n");
		return 0;
	}
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和读信号(1) 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\n");
		return 0;
	}
	
	//读1字节数据
	data = iic_recv_byte();
	//无效应答
	iic_ack(1);
	
	//停止信号
	iic_stop();
	
	return data;
}

//at24c02连续读数据
void at24c02_read_page(u8 addr,u8 *data,u8 len)
{
	u8 ack;
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和写信号(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\n");
		return;
	}
	
	//发送读数据的芯片内部地址
	iic_send_byte(addr);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\n");
		return;
	}
	
	//起始信号
	iic_start();
	//发送从设备地址(0x50)和读信号(1) 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//等待ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\n");
		return;
	}
	
	//连续读取数据
	while(len--){
		//读一个字节
		*data++ = iic_recv_byte();
		//最后一次无效应答，其他有效应答
		if(len>0){
			iic_ack(0);
		}
		else{
			iic_ack(1);
		}
		
	}
	
	//停止信号
	iic_stop();
}
