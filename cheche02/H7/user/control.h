#ifndef __CONTROL_H
#define __CONTROL_H



#include "main.h"

extern int width;
extern int location;

extern float last_err;
extern  volatile int now;
uint8_t turn(char direction);
void line_pid(int expect,float* A,float* B);
void line_pid_daozhe(int expect, float *A, float *B);

#endif