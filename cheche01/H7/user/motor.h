#ifndef _MOTOR__H
#define _MOTOR__H

#include "main.h"
extern float exA,exB;
extern int ceh_A,ceh_B;
extern float Tv_A,Tv_B;

extern float v_A,v_B;
extern float error_v_A,error_v_B;
extern float last_error_v_A,last_error_v_B;
extern float pwmA,pwmB;

extern float pid_out_VA,pid_out_VB;

extern int lz_A,lz_B;
extern int z_A,z_B;

extern float 
			last_distence_A,last_distence_B,
			distence_A,distence_B,
			T_distence_A,T_distence_B,
			distence_A_er,distence_B_er,
			distence_A_last_er,distence_B_last_er,
			distence_A_i,distence_B_i,
			distence_A_d,distence_B_d;

void dianjiA(int s);
void dianjiB(int s);
void pid_VA(void);
void pid_VB(void);
void motor_init();
void motor_handle();
void get_T_distence(float A,float B);
void distence_A_pid(void);
void distence_B_pid(void);


#endif