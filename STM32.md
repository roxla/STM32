I2C		

```c
//	毫秒级延时
void delay_ms(uint32_t n)
{
	//168MHz
	while(n--)
	{
		SysTick->CTRL = 0;			//	关闭系统定时器
		SysTick->LOAD = 168000-1;	//	计数值：16800 cycles
		SysTick->VAL = 0;			//	清空当前计数值
		SysTick->CTRL = 5;			//	选择168MHz 并打开定时器
		while((SysTick->CTRL & 0x00010000) == 0);
			SysTick->CTRL = 0;			//	关闭系统定时器
	}
}

// 微秒级延时
void delay_us(uint32_t n)
{
	while(n--)
	{
		SysTick->CTRL = 0; 		// 关闭系统定时器
		SysTick->LOAD = 168-1; 	// 计数值：168 cycles
		SysTick->VAL = 0; 		// 清空当前计数值
		SysTick->CTRL = 5; 		// 选择168MHz，并打开系统定时器
		while ((SysTick->CTRL & 0x00010000)==0);// 等待计数结束
		SysTick->CTRL = 0; 		// 关闭系统定时器
	}
}

//	初始化EEPROM
void at24c02_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//	使能端口的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//	配置GPIO（模拟I2C：IO口更加自由，代码配置更为简单）
		//	SCL：输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;				//	引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//	输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//	推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//	输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//	无上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//	一开始SDA和SCL初始电平为高电平
	SCL_W = 1;
	SDA_W = 1;
}

//	修改SDA引脚的输出/输入模式
void sda_pin_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;				//	引脚号
	GPIO_InitStructure.GPIO_Mode = mode;						//	输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//	推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//	输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//	无上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//	起始信号
void i2c_start(void)
{
	//	确保当前SDA为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	//	SCL、SDA电平为高
	SCL_W = 1;
	SDA_W = 1;
	delay_us(5);
	
	//	SDA 跳变为低电平
	SDA_W = 0;
	delay_us(5);
	
	//	让总线进入忙状态
	SCL_W = 0;
	delay_us(5);
}

//	结束信号
void i2c_stop(void)
{
	//	确保当前SDA为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	//	确保SCL为高电平、SDA为低电平
	SCL_W = 1;
	SDA_W = 0;
	delay_us(5);
	
	//	SDA 跳变为低电平
	SDA_W = 1;
	delay_us(5);
}

//	发送1个字节数据
void i2c_send_byte(uint8_t byte)
{
	int8_t i = 0;
	
	//	确保当前SDA为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	SCL_W = 0;
	SDA_W = 0;
	delay_us(5);
	
	//	开始发送数据（MSB最高有效位优先传输）
	for(i = 7; i >=0; i--)
	{
		//	检测对应的bit位是1还是0
		if(byte&(1<<i))
			SDA_W = 1;
		else
			SDA_W = 0;
		delay_us(5);
		
		//	时钟线拉高，数据有效
		SCL_W = 1;
		delay_us(5);
		
		//	时钟线拉低，数据更改
		SCL_W = 0;
		delay_us(5);
	}
}

//	接收1个字节数据
uint8_t i2c_recv_byte(void)
{
	uint8_t data=0;
	int i = 7;
	
	//	确保当前SDA为输入模式
	sda_pin_mode(GPIO_Mode_IN);
	
	//	开始发送数据（MSB最高有效位优先传输）
	for(i=7; i>=0; i--)
	{
		// 时钟线拉高，数据有效
		SCL_W = 1;
		delay_us(5);
		
		// 读取SDA的电平
		if(SDA_R)
			data |= 1<<i;
		
		// 时钟线拉低，数据更改。（保持占用总线、处于忙状态）
		SCL_W = 0;
		delay_us(5);
	}
	
	return data;
}

//	应答信号
void i2c_ack(uint8_t ack)
{
	//	确保当前SDA为输出模式
	sda_pin_mode(GPIO_Mode_OUT);
	
	SCL_W = 0;
	SDA_W = 0;
	delay_us(5);
	
	//发送高/低电平
	SDA_W = ack;
	delay_us(5);
	
	// 时钟线拉高，数据有效
	SCL_W = 1;
	delay_us(5);
	
	// 时钟线拉低，数据更改
	SCL_W = 0;
	delay_us(5);
}

//	等待从机应答信号
uint8_t i2c_wait_ack(void)
{
	uint8_t ack;
	//	确保当前SDA为输入模式
	sda_pin_mode(GPIO_Mode_IN);
	
	//	时钟线拉高，数据有效
		SCL_W = 1;
		delay_us(5);
	
	//	读取SDA引脚电平
	if(SDA_R)
		ack = 1;	//	无应答
	else
		ack = 0;	//	有应答
	
	//	让总线进入忙状态(保持占用总线)
	SCL_W = 0;
	delay_us(5);
	
	return ack;
}

//	发送数据
int32_t at24c02_write(uint8_t word_addr, uint8_t *data, uint8_t len)
{
	uint8_t ack = 0;
	uint8_t *p = data;
	
	//	发送起始信号
	i2c_start();
	
	//	发送设备寻址地址，从机写访问地址0xA0
	i2c_send_byte(0xA0);
	
	//	等待应答
	ack = i2c_wait_ack();
	if(ack)
	{
		printf("devie address error!\r\n");
		return -1;
	}
	
	//	发送数据存储地址
	i2c_send_byte(word_addr);
	
	//	等待应答
	ack = i2c_wait_ack();
	if(ack)
	{
		printf("word address error!\r\n");
		return -2;
	}
	
	//	连续写入数据
	while(len--)
	{
		i2c_send_byte(*p++);
		
		//	等待应答
		ack = i2c_wait_ack();
		if(ack)
		{
			printf("word address error!\r\n");
			return -3;
		}
	}
	
	//	结束信号
	i2c_stop();
	
	printf("success!\r\n");
	
	return 0;
}

//	读取数据
int32_t at24c02_read(uint8_t word_addr, uint8_t *data, uint8_t len)
{
	uint8_t ack = 0;
	uint8_t *p = data;
	
	//	发送起始信号
	i2c_start();
	
	//	发送设备寻址地址，从机写访问地址0xA0
	i2c_send_byte(0xA0);
	
	//	等待应答
	ack = i2c_wait_ack();
	if(ack)
	{
		printf("read: devie address with write error!\r\n");
		return -1;
	}
	
	//	发送数据存储地址
	i2c_send_byte(word_addr);
	
	//	等待应答
	ack = i2c_wait_ack();
	if(ack)
	{
		printf("read: word address error!\r\n");
		return -2;
	}
	
	//	发送起始信号
	i2c_start();
	
	//	发送设备寻址地址，从机读访问地址0xA1
	i2c_send_byte(0xA1);
	
	//	等待应答
	ack = i2c_wait_ack();
	if(ack)
	{
		printf("read: devie address with read error!\r\n");
		return -3;
	}
	
	//	连续读取数据
	len = len-1;	// （因最后一个数据无应答）
	while(len--)
	{
		*p++ = i2c_recv_byte();
		
		//	主动发送应答信号
		i2c_ack(0);
	}
	
	//	读取最后一个数据
	*p = i2c_recv_byte();
		
	 //	主动发送无应答信号
	i2c_ack(1);
		
	//	结束信号
	i2c_stop();
	
	printf("read successful\r\n");
	
	return 0;
	
}

int main()
{
	uint8_t i;
	uint8_t buf[8] = {1,2,3,4,5,6,7,8};
	uint8_t read_buf[8] = {0};
	
	//	初始化EEPROM
	at24c02_init();		
	
	printf("This is at24c02 test\r\n");
	
	// 写入
	at24c02_write(0, buf, 8);
	
	delay_ms(10);

	// 读取
	memset(read_buf, 0, sizeof(read_buf));
	at24c02_read(0, read_buf, 8);
	printf("read_buf: ");
	for(i=0; i<8; i++)
		printf("%02X ", read_buf[i]);
	printf("\r\n");

	while(1)
	{
		
	}
}
```
SPI

```c
void w25qxx_init(void)
{
	/*!< Enable the SPI1 clock ，使能SPI1的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/*!< Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/*!< SPI pins configuration ，配置SPI引脚*/
	
	/*!< Connect SPI pins to AF3 AF4 AF5 */  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
		
	/*!< SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< SPI MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< Configure sFLASH Card CS pin in output pushpull mode ,配置片选引脚为输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	/*!< Deselect the FLASH: Chip Select high ，由于M4芯片还没有真正开始跟从机工作，先让片选引脚为高电平*/
	W25QXX_SS=1;
	
	/*!< SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//设置SPI为全双工双线双向通信
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//配置M4为主机角色
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//SPI的发送和接收都是8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//串行时钟（SCLK线）空闲的时候是高电平还是低电平，主要按照外设的芯片手册决定[*]
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//读取MISO引脚的数据，在串行时钟的第二跳边沿进行数据采样（读取）
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//片选引脚是由软件来决定
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//SPI通信时钟=84MHz/16=5.25MHz，平常应用场景都会连接多个SPI设备，为了统一速率都是使用10Mbps左右[*]
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//最高有效位优先，根据外设的芯片手册决定[*]
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/*!< Enable the sFLASH_SPI  ，使能SPI1*/
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

int main(void)
{
	uint8_t buf[64]={0};
	uint32_t i=0;
	
	//为系统定时器选择时钟源为168MHz
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	//为系统定时器选择时钟源为168MHz/8=21MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	//配置中断优先级的分组
	//配置支持4个抢占优先级，支持4个响应优先级
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//spi flash初始化
	w25qxx_init();
	
	//打印id信息
	printf("w25qxx spi flash id=%04X\r\n",w25qxx_read_id());
	
	//从地址0进行扇区擦除，每次擦除4KB字节
	printf("w25qxx spi flash erase sector at addr 0\r\n");
	w25qxx_erase_sector(0);
	
	//从地址0写入64字节数据，写入数据位1~64
	for(i=0; i<64; i++)
		buf[i]='G';
	printf("w25qxx spi flash write data at addr 0,data is 1~64\r\n");
	w25qxx_write_page(0,buf,64);
	
	//从地址0读取64字节数据
	memset(buf,0,64);
	printf("w25qxx spi flash read data at addr 0:\r\n");
	w25qxx_read_data(0,buf,64);
	
	for(i=0; i<64; i++)
	{
		printf("%c ",buf[i]);
	
	}

	printf("\r\n");
	
	while(1);
}
```

CAN

```

```

