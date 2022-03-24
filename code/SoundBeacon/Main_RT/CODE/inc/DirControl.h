#ifndef  _DIRCONTROL_H_
#define  _DIRCONTROL_H_
#include "Pid.h"

typedef struct DIR_PID
{
  int16 err_last;   //上次偏差
  int16 err_current;     //当前偏差
  float kp,kd;
  int16 result;
}dir_pid_t;

extern dir_pid_t pid_dir;
extern float dir_error[5];

void direction_control();
void filter_error();


#endif