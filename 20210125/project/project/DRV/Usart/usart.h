#ifndef _USART_H_
#define _USART_H_

//�رհ�����ģʽ
#pragma import(__use_no_semihosting)

// ��ʼ��
void usart_init(void);
// ����һ���ַ�
void uart1_putc(char ch);
// ����һ���ַ���
void uart1_puts(const char *s);

#endif
