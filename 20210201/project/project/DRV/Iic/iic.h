#ifndef _IIC_H_
#define _IIC_H_

#include <stm32f4xx.h>
#include <sys.h>

#define SCL PBout(8)
#define SDA_IN PBin(9)
#define SDA_OUT PBout(9)

void iic_init(void);
void at24c02_write_byte(u8 addr,u8 data);
void at24c02_write_page(u8 addr,u8 *data,u8 len);
u8 at24c02_read_byte(u8 addr);
void at24c02_read_page(u8 addr,u8 *data,u8 len);


#endif
