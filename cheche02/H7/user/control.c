#include "control.h"
#include "motor.h"
#include "OLED.h"
int width = 0;
int location = 0;
float err = 0;
float last_err = 0;
volatile int now = 83;

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
			out = 0.25 * err + 2 * (err - last_err);
			Tv_A = 40 + out;
			Tv_B = 40 - out;
		}
    
}





