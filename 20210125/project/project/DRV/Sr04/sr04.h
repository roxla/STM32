#ifndef _SR04_H_
#define _SR04_H_

#define TRIG PCout(7)
#define ECHO PCin(9)

// ��ʼ��
void sr04_init(void);
// ���
int get_distance(void);

#endif