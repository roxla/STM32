#ifndef _USART_H_
#define _USART_H_

//�رհ�����ģʽ
#pragma import(__use_no_semihosting)

// ��ʼ��
void uart1_init(void);
// ����һ���ַ�
void uart1_putc(char ch);
// ����һ���ַ���
void uart1_puts(const char *s);

// ��ʼ��
void uart3_init(void);
// ����һ���ַ�
void uart3_putc(char ch);
// ����һ���ַ���
void uart3_puts(const char *s);

//�����յ������
void parse_cmd(void);

#endif
