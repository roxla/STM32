#ifndef _USART_H_
#define _USART_H_

//关闭半主机模式
#pragma import(__use_no_semihosting)

// 初始化
void uart1_init(void);
// 发送一个字符
void uart1_putc(char ch);
// 发送一个字符串
void uart1_puts(const char *s);

// 初始化
void uart3_init(void);
// 发送一个字符
void uart3_putc(char ch);
// 发送一个字符串
void uart3_puts(const char *s);

//处理收到命令函数
void parse_cmd(void);

#endif
