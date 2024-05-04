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

void dianjiA(int s);
void dianjiB(int s);
void pid_VA(void);
void pid_VB(void);
void motor_init();
void motor_handle();

