#include "Pid.h"
#include "Motor.h"

pid_t pid_F_L;
pid_t pid_F_R;
pid_t pid_B_L;
pid_t pid_B_R;


float F_L_P = 8.5;
float F_L_I = 0.023;
float F_L_D = 0.0;
float F_L_D_FILTER = 1;

float F_R_P = 8.5;
float F_R_I = 0.025;
float F_R_D = 0.0;
float F_R_D_FILTER = 1;

float B_L_P = 8.5;
float B_L_I = 0.023;
float B_L_D = 0.0;
float B_L_D_FILTER = 1;

float B_R_P = 8.7;
float B_R_I = 0.025;
float B_R_D = 0.0;
float B_R_D_FILTER = 1;


//pid��ʼ��
void pid_init()
{
  pid_F_L.kp = F_L_P;
  pid_F_L.ki = F_L_I;
  pid_F_L.kd = F_L_D;
  pid_F_L.err = 0;
  pid_F_L.err_1 = 0;
  pid_F_L.err_2 = 0;
  pid_F_L.err_3 = 0;
  pid_F_L.d_filter = F_L_D_FILTER;
  pid_F_L.last_d = 0;
  pid_F_L.result = 0;
  
  pid_F_R.kp = F_R_P;
  pid_F_R.ki = F_R_I;
  pid_F_R.kd = F_R_D;
  pid_F_R.err = 0;
  pid_F_R.err_1 = 0;
  pid_F_R.err_2 = 0;
  pid_F_R.err_3 = 0;
  pid_F_R.d_filter = F_R_D_FILTER;
  pid_F_R.last_d = 0;
  pid_F_R.result = 0;
    
  pid_B_L.kp = B_L_P;
  pid_B_L.ki = B_L_I;
  pid_B_L.kd = B_L_D;
  pid_B_L.err = 0;
  pid_B_L.err_1 = 0;
  pid_B_L.err_2 = 0;
  pid_B_L.err_3 = 0;
  pid_B_L.d_filter = B_L_D_FILTER;
  pid_B_L.last_d = 0;
  pid_B_L.result = 0;
  
  pid_B_R.kp = B_R_P;
  pid_B_R.ki = B_R_I;
  pid_B_R.kd = B_R_D;
  pid_B_R.err = 0;
  pid_B_R.err_1 = 0;
  pid_B_R.err_2 = 0;
  pid_B_R.err_3 = 0;
  pid_B_R.d_filter = B_R_D_FILTER;
  pid_B_R.last_d = 0;
  pid_B_R.result = 0;
}


//pid��λ
void pid_reset(pid_t *pid)
{
  pid->err_1 = 0;
  pid->err_2 = 0;
  pid->err_3 = 0;
  pid->last_d = 0;
  pid->result = 0;
}


//pid����
void pid_control(pid_t *pid, int16 actual, int16 set)
{
  float p=0, i=0, d=0;
  pid->err = set - actual;
  p = pid->err - pid->err_1;
  i = pid->err;
  d = pid->err - 2*pid->err_1 +pid->err_2;//��һ�׵�ͨ�˲�
  pid->last_d = d;
  pid->result += pid->kp*p + pid->ki*i + pid->kd*d;
  
  if(pid->result > SPEED_MAX) 
    pid->result = SPEED_MAX;
  else if(pid->result < SPEED_MIN)
    pid->result = SPEED_MIN;
  
  pid->err_3 = pid->err_2;
  pid->err_2 = pid->err_1;
  pid->err_1 = pid->err;
}


