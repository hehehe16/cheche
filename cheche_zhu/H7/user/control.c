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
		{out = 0.125 * err + 0.3 * (err - last_err);
			Tv_A = 50 + out;
			Tv_B = 50 - out;
		}
		else
		{
			out = 0.1 * err + 0.15 * (err - last_err);
			Tv_A = 20 + out;
			Tv_B = 20 - out;
		}
    
}





