#ifndef _LED_H_
#define _LED_H_

void delay(unsigned int ms);
void led_init(void);
//num:1,2,3,4  state:0,1
void led_onoff(int led_num,int state);

#endif
