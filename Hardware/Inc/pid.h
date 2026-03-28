#ifndef __PID_H__
#define __PID_H_

#include "main.h"

#define angle_target    2038

typedef struct
{
    float kp;
    float ki;
    float kd;

    float target;
    float actual;

    float error;
    float error_last;

    float integral;
    float output;

    float output_max;
    float output_min;
}pid_struct;

void pid_calc(pid_struct *pid);

#endif
