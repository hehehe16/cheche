#include "control.h"
#include "motor.h"
#include "OLED.h"
int width = 0;
int location = 0;
float err = 0;
float last_err = 0;
volatile int now = 100;

void line_pid(int expect, float *A, float *B)
{

    float out = 0;

    last_err = err;
    err = now - expect;
    out = 0.1 * err + 2 * (err - last_err);

    Tv_A = exA + out;
    Tv_B = exB - out;
}

uint8_t turn(char direction)
{
    
    if (direction == 'R')
    {
        Tv_A = 40;
        Tv_B = 0;
    }
    else if (direction == 'L')
    {
        Tv_A = 0;
        Tv_B=40;
    }

    
    return direction;

}

void line_pid_daozhe(int expect, float *A, float *B)
{

    float out = 0;

    last_err = err;
    err = now - expect;
    out = 0.1 * err + 2 * (err - last_err);

    Tv_A = exA - out;
    Tv_B = exB + out;
}

