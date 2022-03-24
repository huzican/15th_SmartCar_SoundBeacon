#ifndef __PID_H__
#define __PID_H__

#include "common.h"


typedef struct PID
{
  float err_1,err_2,err_3;
  float err;
  float kp,ki,kd;
  float last_d,d_filter;
  int16 result;
}pid_t;


extern pid_t pid_F_L;
extern pid_t pid_F_R;
extern pid_t pid_B_L;
extern pid_t pid_B_R;


void pid_init();
void pid_reset(pid_t *pid);
void pid_control(pid_t *pid, int16 actual, int16 set);

#endif