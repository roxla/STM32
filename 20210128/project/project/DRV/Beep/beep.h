#ifndef _BEEP_H_
#define _BEEP_H_

#define BEEP PFout(8)

// ��ʼ��
void beep_init(void);
// ����
void beep_switch(int state);

#endif
