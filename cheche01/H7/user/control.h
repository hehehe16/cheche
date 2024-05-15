#ifndef __CONTROL_H
#define __CONTROL_H



#include "main.h"

extern int width;
extern int location;
extern float last_err;
extern  volatile int now;
void line_pid(int expect,int zhuangtai);

uint8_t fangxiang(uint8_t f,int ba,int zhuangtai);
#endif