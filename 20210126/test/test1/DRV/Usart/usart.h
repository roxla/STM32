#ifndef _USART_H_
#define _USART_H_

//关闭半主机模式
#pragma import(__use_no_semihosting)

// 初始化
void usart_init(void);
// 发送一个字符
void uart1_putc(char ch);
// 发送一个字符串
void uart1_puts(const char *s);

#endif
