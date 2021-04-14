#include <myhead.h>

//��ʼ��IO��
void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;//��©
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//PB8 PB9
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
}

//����SDA�ķ���
void iic_sda_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = mode;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//PB9 --- SDA
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//��ʼ�ź�
void iic_start(void)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	//���߿���
	SCL = 1;
	SDA_OUT = 1;
	my_delay_us(5);
	
	//SDA����
	SDA_OUT = 0;
	my_delay_us(5);
	
	//SDA����֮��������SDA��ǯס����
	SCL = 0;
}

//ֹͣ�ź�
void iic_stop(void)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);

	SCL = 0;
	SDA_OUT = 0;
	my_delay_us(5);
	
	//����SCL
	SCL = 1;
	my_delay_us(5);
	
	//����SDA����ʱSCL��SDA��Ϊ�ߣ����߿���
	SDA_OUT = 1;
}

//�ȴ����豸Ӧ�𣬷���0��ʾ��ЧӦ�𣬷�0��ЧӦ��
u8 iic_wait_ack(void)
{
	u8 ack = 0;
	
	//SDAΪ����ģʽ
	iic_sda_mode(GPIO_Mode_IN);
	
	//����SCL���������SDA
	SCL = 1;
	my_delay_us(5);
	
	//��ȡSDA�ĵ�ƽ
	if(SDA_IN==1){
		ack = 1;//��ЧӦ��
		//ֹͣ����
		iic_stop();
	}
	else
		ack = 0;
	
	//ǯס����
	SCL = 0;
	my_delay_us(5);
	
	return ack;
}

//����Ӧ���źţ�����ack=0��ЧӦ�𣬷�����ЧӦ��
void iic_ack(u8 ack)
{
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	my_delay_us(5);
	
	//����SDA��ƽ
	if(ack)
		SDA_OUT = 1;//��ЧӦ��
	else
		SDA_OUT = 0;//��ЧӦ��
	
	my_delay_us(5);
	//����SCL�������öԷ���ȡӦ��
	SCL = 1;
	my_delay_us(5);
	
	SCL = 0;
}

//����һ���ֽ� --------- ��λ�ȳ�
void iic_send_byte(u8 txd)
{
	u8 i;
	
	//SDA���ģʽ
	iic_sda_mode(GPIO_Mode_OUT);
	
	SCL = 0;
	my_delay_us(5);
	
	//�Ӹ�λ����λ���η���ÿһλ
	for(i=0;i<8;i++){
		if(txd&1<<(7-i))
			SDA_OUT = 1;//��ǰλ��1�����ߵ�ƽ
		else
			SDA_OUT = 0;
		
		my_delay_us(5);
		
		//����ʱ���ߣ��Է�������һλ
		SCL = 1;
		//��ʱ�����������ȶ�
		my_delay_us(5);
		//����ʱ���ߣ�׼��������һλ
		SCL = 0;
	}
}

//����һ���ֽ�
u8 iic_recv_byte(void)
{
	u8 rxd = 0,i;
	
	//SDA����ģʽ
	iic_sda_mode(GPIO_Mode_IN);
	
	SCL = 0;
	
	//�Ӹߵ������ν���ÿһλ
	for(i=0;i<8;i++){
		//�ȴ��Է����úõ�ƽ
		my_delay_us(5);
		//SCL�ߵ�ƽ�ڼ��ȡһλ����
		SCL = 1;
		//���ߵ�ƽ�޸�rxd
		if(SDA_IN)
			rxd |= 1<<(7-i);
		
		//׼��������һλ
		my_delay_us(5);
		SCL = 0;
	}
	
	return rxd;
}

//at24c02д1�ֽڵ�����
void at24c02_write_byte(u8 addr,u8 data)
{
	u8 ack;
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)��д�ź�(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\r\n");
		return;
	}
	
	//����д���ݵ�оƬ�ڲ���ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\r\n");
		return;
	}
	
	//����д������
	iic_send_byte(data);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\r\n");
		return;
	}
	
	//ֹͣ�ź�
	iic_stop();
}

//at24c02����д������
void at24c02_write_page(u8 addr,u8 *data,u8 len)
{
	u8 ack;
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)��д�ź�(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\r\n");
		return;
	}
	
	//����д���ݵ�оƬ�ڲ���ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\r\n");
		return;
	}
	
	//��������д������
	while(len--){
		iic_send_byte(*data++);
		//�ȴ�ACK
		ack = iic_wait_ack();
		if(ack==1){
			printf("none ack 3!\r\n");
			return;
		}
	}
	
	//ֹͣ�ź�
	iic_stop();
}

//at24c02��1�ֽڵ�����
u8 at24c02_read_byte(u8 addr)
{
	u8 ack,data;
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)��д�ź�(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\n");
		return 0;
	}
	
	//���Ͷ����ݵ�оƬ�ڲ���ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\n");
		return 0;
	}
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)�Ͷ��ź�(1) 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\n");
		return 0;
	}
	
	//��1�ֽ�����
	data = iic_recv_byte();
	//��ЧӦ��
	iic_ack(1);
	
	//ֹͣ�ź�
	iic_stop();
	
	return data;
}

//at24c02����������
void at24c02_read_page(u8 addr,u8 *data,u8 len)
{
	u8 ack;
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)��д�ź�(0) 0x50<<1|0 = 0xa0
	iic_send_byte(0xa0);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 1!\n");
		return;
	}
	
	//���Ͷ����ݵ�оƬ�ڲ���ַ
	iic_send_byte(addr);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 2!\n");
		return;
	}
	
	//��ʼ�ź�
	iic_start();
	//���ʹ��豸��ַ(0x50)�Ͷ��ź�(1) 0x50<<1|1 = 0xa1
	iic_send_byte(0xa1);
	//�ȴ�ACK
	ack = iic_wait_ack();
	if(ack==1){
		printf("none ack 3!\n");
		return;
	}
	
	//������ȡ����
	while(len--){
		//��һ���ֽ�
		*data++ = iic_recv_byte();
		//���һ����ЧӦ��������ЧӦ��
		if(len>0){
			iic_ack(0);
		}
		else{
			iic_ack(1);
		}
		
	}
	
	//ֹͣ�ź�
	iic_stop();
}
