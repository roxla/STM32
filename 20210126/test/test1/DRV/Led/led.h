#ifndef _LED_H_
#define _LED_H_

#define LED0 PFout(9)
#define LED1 PFout(10)
#define LED2 PEout(13)
#define LED3 PEout(14)

void led_init(void);
//num:1,2,3,4  state:0,1
void led_onoff(int led_num,int state);

#endif
