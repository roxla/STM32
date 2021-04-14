#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "stdlib.h"  

//-----------------OLED端口定义----------------

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int

extern void OLED_ClearPoint(u8 x,u8 y);
extern void OLED_ColorTurn(u8 i);
extern void OLED_DisplayTurn(u8 i);
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_WaitAck(void);
extern void Send_Byte(u8 dat);
extern void OLED_WR_Byte(u8 dat,u8 cmd);
extern void OLED_DisPlay_On(void);
extern void OLED_DisPlay_Off(void);
extern void OLED_Refresh(void);
extern void OLED_Clear(void);
extern void OLED_DrawPoint(u8 x,u8 y);
extern void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
extern void OLED_DrawCircle(u8 x,u8 y,u8 r);
extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
extern void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
extern void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
extern void OLED_ScrollDisplay(u8 num,u8 space);
extern void OLED_WR_BP(u8 x,u8 y);
extern void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
extern void OLED_Init(void);

#endif
