#ifndef _DHT11_H_
#define _DHT11_H_

#define DQ_OUT PGout(9)
#define DQ_IN PGin(9)

// 测试DHT11是否有响应，返回0正常，非0表示无响应
u8 DHT11_Check(void);
u8 dht11_read_data(u8 *temp,u8 *humi);

#endif
