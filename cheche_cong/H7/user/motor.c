#include "motor.h"
#include "tim.h"
#include "gpio.h"


int ceh_A=0,ceh_B=0;
float Tv_A=60,Tv_B=60;
float exA=60,exB=60;

float v_A=0,v_B=0;
float error_v_A=0,error_v_B=0;
float last_error_v_A=0,last_error_v_B=0;
float pwmA=0,pwmB=0;

float pid_out_VA=0,pid_out_VB=0;

int lz_A=0,lz_B=0;
int z_A=10000,z_B=10000;
int fffff=0;

void motor_init()
{
    Tv_A = exA;
    Tv_B= exB;
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1|TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1|TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    TIM3->CNT  =10000;
    TIM4->CNT  =10000;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
}


void dianjiA(int s)
{
	s=-s;
		if(s>200)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, s);
		}
		
		else if(s<-200)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -s);
		}
		
		else
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 9999);
		}	
}
void dianjiB(int s)
{
//	s=-s;
		if(s>200)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, s);
		}
		
		else if(s<-200)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -s);
		}
		
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 9999);
		}	
}

void pid_VA(void)
{
	last_error_v_A=error_v_A;
	error_v_A=Tv_A-v_A;
	pid_out_VA= 10*(error_v_A-last_error_v_A)+5*error_v_A;
	pwmA=pwmA+pid_out_VA;
	 if(pwmA>9999)
	 	pwmA=9999;
     else if(pwmA<-9999)
	 	pwmA=-9999;
	dianjiA((int)pwmA);
}

void pid_VB(void)
{
	last_error_v_B=error_v_B;
	error_v_B=Tv_B-v_B;
	pid_out_VB= 10*(error_v_B-last_error_v_B)+5*error_v_B;
	pwmB=pwmB+pid_out_VB;
	 if(pwmB>9999)
	 	pwmB=9999;
	 else if(pwmB<-9999)
	 	pwmB=-9999;
	 

	if(Tv_B*pwmB<0)
	{
//	  pwmB=0;
	}
	dianjiB((int)pwmB);
}


void motor_handle()
{

		lz_A=z_A;
		lz_B=z_B;

		z_A=ceh_A*65535+TIM3->CNT;
		z_B=ceh_B*65535+TIM4->CNT;
		v_A=(z_A-lz_A)*80*100/5000;
		v_B=(z_B-lz_B)*80*100/5000;
			fffff++;
		if(fffff>2)
		{
			pid_VA();
			pid_VB();
			fffff=3;
		}

		HAL_TIM_Base_Start_IT(&htim4);
}


float 
			last_distence_A,last_distence_B,
			distence_A,distence_B,
			T_distence_A,T_distence_B,
			distence_kp=0.2,distence_ki=0,distence_kd=1,
			distence_A_er,distence_B_er,
			distence_A_last_er,distence_B_last_er,
			distence_A_i,distence_B_i,
			distence_A_d,distence_B_d;


void get_T_distence(float A,float B)
{
	T_distence_A=A;
	T_distence_B=B;
}

void distence_A_pid(void)   //λ��  pid
{
	float distence_pid_out;
	distence_A=ceh_A*65535+TIM3->CNT;
	distence_A_last_er=distence_A_er;
	distence_A_er=T_distence_A-distence_A;
	distence_A_d=distence_A_er-distence_A_last_er;
	if(0)
	{
		distence_A_i=distence_A_i+distence_A_er;
	}
	else
	{
		distence_A_i=0;
	}
	distence_pid_out=distence_kp*distence_A_er+distence_ki*distence_A_i+distence_kd*distence_A_d;
	
	if(distence_pid_out>80)
	{
		distence_pid_out=80;
	}
	else if(distence_pid_out<-80)
	{
		distence_pid_out=-80;
	}
	Tv_A=distence_pid_out;
}

void distence_B_pid(void)   //λ��  pid
{
	float distence_pid_out;
	distence_B=ceh_B*65535+TIM4->CNT;
	distence_B_last_er=distence_B_er;
	distence_B_er=T_distence_B-distence_B;
	distence_B_d=distence_B_er-distence_B_last_er;
	if(0)
	{
		distence_B_i=distence_B_i+distence_B_er;
	}
	else
	{
		distence_B_i=0;
	}
	distence_pid_out=distence_kp*distence_B_er+distence_ki*distence_B_i+distence_kd*distence_B_d;
	
	if(distence_pid_out>80)
	{
		distence_pid_out=80;
	}
	else if(distence_pid_out<-80)
	{
		distence_pid_out=-80;
	}
	Tv_B=distence_pid_out;
}




