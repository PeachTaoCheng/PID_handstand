#include "pid.h"

void pid_calc(pid_struct *pid)
{
    pid->error_last = pid->error;
    pid->error = pid->target - pid->actual;

    if(pid->ki  != 0.0f){pid->integral += pid->error;}
    else {pid->integral = 0.0f;}

    pid->output = (pid->kp * pid->error) + (pid->ki * pid->integral) + (pid->kd * (pid->error - pid->error_last));

    if(pid->output > pid->output_max)
    {
        pid->output = pid->output_max;
    }
    else if(pid->output < pid->output_min)
    {
        pid->output = pid->output_min;
    }
    return;
}
