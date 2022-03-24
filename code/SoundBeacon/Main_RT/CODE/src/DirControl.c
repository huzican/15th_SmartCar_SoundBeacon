#include "DirControl.h"
#include "Motor.h"
#include "VoiceProcess.h"
#include "Pid.h"
#include "SEEKFREE_MPU6050.h"
#include "stdlib.h"

#define DIR_MAX 2000
#define DIR_MIN -2000

#define TRANSLATE_TH 60 //方向环变成横移档

dir_pid_t pid_dir;
float dir_error[5];

//陀螺仪辅助
void direction_control()
{
//  filter_error();

  if((abs(maxacor_index[0]-maxacor_index[3])<= 1)|| (abs(maxacor_index[1]-maxacor_index[2])<= 1))
  {
    pid_dir.kp = 380;
    pid_dir.kd = 0.36;
  }
  else
  {
    pid_dir.kp = 360.0;
    pid_dir.kd = 0.39;
  }
  
  get_gyro();
  pid_dir.result = (int16)((pid_dir.kp)*(beacon_error)-pid_dir.kd * mpu_gyro_z);
  
  if(pid_dir.result > DIR_MAX)//防止车转太快
    pid_dir.result = DIR_MAX;
  if(pid_dir.result < DIR_MIN)
    pid_dir.result = DIR_MIN;
  
  motor.speed_set_F_L -= pid_dir.result;
  motor.speed_set_F_R += pid_dir.result;
  motor.speed_set_B_L -= pid_dir.result;
  motor.speed_set_B_R += pid_dir.result;
}

////偏差之差
//void direction_control()
//{
//  pid_dir.kp = 120.0;
//  pid_dir.kd = 10;
//  
//  pid_dir.err_last=pid_dir.err_current;
//  pid_dir.err_current=beacon_error;
//  
//  pid_dir.result = (int16)((pid_dir.kp)*(beacon_error)+pid_dir.kd * (pid_dir.err_current-pid_dir.err_last));
//  
//  if(pid_dir.result > DIR_MAX)
//    pid_dir.result = DIR_MAX;
//  if(pid_dir.result < DIR_MIN)
//    pid_dir.result = DIR_MIN;
//  
//  motor.speed_set_F_L -= pid_dir.result;
//  motor.speed_set_F_R += pid_dir.result;
//  motor.speed_set_B_L -= pid_dir.result;
//  motor.speed_set_B_R += pid_dir.result;
//}

void filter_error()
{
  dir_error[0]=beacon_error;
  static float a = 0;
  float sigal_error;
  sigal_error = dir_error[0] - dir_error[1];
  a = sigal_error/10;                //分母越小越滞后
  if(a < 0)
    a = -a;
  if(a > 0.85)
    a = 0.85;
  dir_error[0] = (1-a)*dir_error[0]+a*dir_error[1];
  dir_error[4] = dir_error[3];
  dir_error[3] = dir_error[2];
  dir_error[2] = dir_error[1];
  dir_error[1] = dir_error[0];
}
