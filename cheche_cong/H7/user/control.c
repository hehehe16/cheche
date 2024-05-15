#include "control.h"
#include "motor.h"
#include "OLED.h"
int width = 0;
int location = 0;
float err = 0;
float last_err = 0;
volatile int now = 83;
extern int set_jvli_A,set_jvli_B;
void line_pid(int expect, int zhuangtai)
{

    float out = 0;

    last_err = err;
    err = now - expect;
		if (zhuangtai==0)
		{out = 0.1 * err + 0.9 * (err - last_err);
			Tv_A = 80 + out;
			Tv_B = 80 - out;
		}
		else
		{
			out = 0.25 * err + 3 * (err - last_err);
			Tv_A = 40 + out;
			Tv_B = 40 - out;
		}
    
}
/*
				if(back==0)
				{
					if(zhuanwan[lukou-1]=='A')
					{
						if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(83,zhuangtai);}
						
					else 
					{
					get_T_distence(set_jvli_A+700,set_jvli_B+700+1670);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						zhuangtai = 7;						
					}
					
					}
					else if(zhuanwan[lukou-1]=='B')
					{
					if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(83,zhuangtai);}
					else 
					{
					get_T_distence(set_jvli_A+700+1670,set_jvli_B+700);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						zhuangtai = 7;						
					}
					}
					else 
						zhuangtai = 0;
				}
*/

uint8_t fangxiang(uint8_t f,int ba,int zhuangtai)
{
				if(ba==0)
				{
					if(f=='A')
					{
						if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(83,zhuangtai);}
						
					else 
					{
					get_T_distence(set_jvli_A+700,set_jvli_B+700+1670);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						return 1;						
					}
					
					}
					else if(f=='B')
					{
					if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(83,zhuangtai);}
					else 
					{
					get_T_distence(set_jvli_A+700+1670,set_jvli_B+700);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						return 1	;					
					}
					}
					else 
						return 2;//zhuangtai = 0;	
				}
				else if(ba==1)
				{
					if(f=='A')
					{
						if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 )
					{
					get_T_distence(set_jvli_A+700+1670,set_jvli_B+700);
					line_pid(83,zhuangtai);
					}	
					else
					{
						distence_A_pid();
						distence_B_pid();
						if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						{
						return 1;
						}
					}				
					}
					else if(f=='B')
					{
					if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 )
					{
					get_T_distence(set_jvli_A+700,set_jvli_B+700+1670);
					line_pid(83,zhuangtai);
					}
					else
					{
						distence_A_pid();
						distence_B_pid();
						if(distence_A_er*distence_A_er<200&&distence_B_er*distence_B_er<200)	
						{
						return 1;
						}
					}	
					}
					else 
						return 2;
				}
				return 0;
}


