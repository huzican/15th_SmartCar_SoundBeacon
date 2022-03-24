#ifndef  _DIRCONTROL_H_
#define  _DIRCONTROL_H_
#include "Pid.h"

typedef struct DIR_PID
{
  int16 err_last;   //�ϴ�ƫ��
  int16 err_current;     //��ǰƫ��
  float kp,kd;
  int16 result;
}dir_pid_t;

extern dir_pid_t pid_dir;
extern float dir_error[5];

void direction_control();
void filter_error();


#endif