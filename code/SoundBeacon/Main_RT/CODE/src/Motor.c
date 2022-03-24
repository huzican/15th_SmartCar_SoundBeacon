#include "Motor.h"
#include "DirControl.h"
#include "Pid.h"
#include "zf_pwm.h"
#include "FuzzySet_Speed.h"
#include "VoiceProcess.h"
#include "Avoid.h"

int16 X_SPEED_S;
int16 X_SPEED_D;

int8 motor_dir=1;       //电机运动方向
int8 stop_flag=1;       //停车标志
int8 speed_level;
motor_status motor;

void motor_init()
{
  pwm_init(F_R_GO_PWMCH,MOTOR_FRE,INIT_DUTY);
  pwm_init(F_R_BACK_PWMCH,MOTOR_FRE,INIT_DUTY);
  
  pwm_init(F_L_GO_PWMCH,MOTOR_FRE,INIT_DUTY);
  pwm_init(F_L_BACK_PWMCH,MOTOR_FRE,INIT_DUTY);
  
  pwm_init(B_L_GO_PWMCH,MOTOR_FRE,INIT_DUTY);
  pwm_init(B_L_BACK_PWMCH,MOTOR_FRE,INIT_DUTY);
  
  pwm_init(B_R_GO_PWMCH,MOTOR_FRE,INIT_DUTY);
  pwm_init(B_R_BACK_PWMCH,MOTOR_FRE,INIT_DUTY);
}

void speed_control()
{
//  int16 basic_speed = 3000; //限制速度，速度大了影响声音定位
//  motor.speed_set = 10 * motor_dir * FuzzySet_Speed(0,(int16)beacon_distance);
//  if(motor.speed_set>basic_speed)
//    motor.speed_set = basic_speed;
//  else if(motor.speed_set < (-basic_speed))
//    motor.speed_set = (-basic_speed);
  
  int16 basic_speed = 2500;
  motor.speed_set = basic_speed * motor_dir;
  

  motor.speed_set_F_L = motor.speed_set;
  motor.speed_set_F_R = motor.speed_set;
  motor.speed_set_B_L = motor.speed_set;
  motor.speed_set_B_R = motor.speed_set; 
}

//一阶滞后滤波法
//方法：取a = 0~1，本次滤波结果 = a*本次采样值 + (1-a)*上次滤波结果
//优点：对周期性干扰具有良好的抑制作用，适用于波动频率较高的场合
//缺点：相位滞后，灵敏度低，滞后程度取决于a的大小，不能消除滤波频率高于采样频率1/2的干扰信号
void filter_speed()
{
}

void filter_speed_F_L()
{
  static float a = 0;
  float speed_error;
  speed_error = motor.speed_set_F_L - motor.speed_F_L_save[0];
  a = speed_error/30;//分母如何选定 每个周期减下来的速度*2+一些 具体需要调试
  if(a < 0)         a = -a;
  if(a > 0.85)      a = 0.85;
  motor.speed_set_F_L = (int16)(motor.speed_set_F_L*(1-a) + (motor.speed_F_L_save[0]+motor.speed_F_L_save[1]+motor.speed_F_L_save[2])*a/3);
  motor.speed_F_L_save[2] = motor.speed_F_L_save[1];
  motor.speed_F_L_save[1] = motor.speed_F_L_save[0];
  motor.speed_F_L_save[0] = motor.speed_set_F_L;
}

void filter_speed_F_R()
{
  static float a = 0;
  float speed_error;
  speed_error = motor.speed_set_F_R - motor.speed_F_R_save[0];
  a = speed_error/30;//分母如何选定 每个周期减下来的速度*2+一些 具体需要调试
  if(a < 0)         a = -a;
  if(a > 0.85)      a = 0.85;
  motor.speed_set_F_R = (int16)(motor.speed_set_F_R*(1-a) + (motor.speed_F_R_save[0]+motor.speed_F_R_save[1]+motor.speed_F_R_save[2])*a/3);
  motor.speed_F_R_save[2] = motor.speed_F_R_save[1];
  motor.speed_F_R_save[1] = motor.speed_F_R_save[0];
  motor.speed_F_R_save[0] = motor.speed_set_F_R;
}
void filter_speed_B_L()
{
  static float a = 0;
  float speed_error;
  speed_error = motor.speed_set_B_L - motor.speed_B_L_save[0];
  a = speed_error/30;//分母如何选定 每个周期减下来的速度*2+一些 具体需要调试
  if(a < 0)         a = -a;
  if(a > 0.85)      a = 0.85;
  motor.speed_set_B_L = (int16)(motor.speed_set_B_L*(1-a) + (motor.speed_B_L_save[0]+motor.speed_B_L_save[1]+motor.speed_B_L_save[2])*a/3);
  motor.speed_B_L_save[2] = motor.speed_B_L_save[1];
  motor.speed_B_L_save[1] = motor.speed_B_L_save[0];
  motor.speed_B_L_save[0] = motor.speed_set_B_L;
}
void filter_speed_B_R()
{
  static float a = 0;
  float speed_error;
  speed_error = motor.speed_set_B_R - motor.speed_B_R_save[0];
  a = speed_error/30;//分母如何选定 每个周期减下来的速度*2+一些 具体需要调试
  if(a < 0)         a = -a;
  if(a > 0.85)      a = 0.85;
  motor.speed_set_B_R = (int16)(motor.speed_set_B_R*(1-a) + (motor.speed_B_R_save[0]+motor.speed_B_R_save[1]+motor.speed_B_R_save[2])*a/3);
  motor.speed_B_R_save[2] = motor.speed_B_R_save[1];
  motor.speed_B_R_save[1] = motor.speed_B_R_save[0];
  motor.speed_B_R_save[0] = motor.speed_set_B_R;
}


void judge_speed_max()
{
  if(motor.speed_set_F_L > SPEED_MAX)
    motor.speed_set_F_L = SPEED_MAX;
  else if(motor.speed_set_F_L < SPEED_MIN)
    motor.speed_set_F_L = SPEED_MIN;
  
  if(motor.speed_set_F_R > SPEED_MAX)
    motor.speed_set_F_R = SPEED_MAX;
  else if(motor.speed_set_F_R < SPEED_MIN)
    motor.speed_set_F_R = SPEED_MIN;
  
  if(motor.speed_set_B_L > SPEED_MAX)
    motor.speed_set_B_L = SPEED_MAX;
  else if(motor.speed_set_B_L < SPEED_MIN)
    motor.speed_set_B_L = SPEED_MIN;
  
  if(motor.speed_set_B_R > SPEED_MAX)
    motor.speed_set_B_R = SPEED_MAX;
  else if(motor.speed_set_B_R < SPEED_MIN)
    motor.speed_set_B_R = SPEED_MIN;
}

void break_down()
{
  pwm_duty(F_R_GO_PWMCH,0);
  pwm_duty(F_R_BACK_PWMCH,0);
  
  pwm_duty(F_L_GO_PWMCH,0);
  pwm_duty(F_L_BACK_PWMCH,0);
  
  pwm_duty(B_L_GO_PWMCH,0);
  pwm_duty(B_L_BACK_PWMCH,0);
  
  pwm_duty(B_R_GO_PWMCH,0);
  pwm_duty(B_R_BACK_PWMCH,0);
}

void pull_up()
{
//  motor.speed_set = 0;
  motor.speed_set_F_L = 0;
  motor.speed_set_F_R = 0;
  motor.speed_set_B_L = 0;
  motor.speed_set_B_R = 0;
}

void roll_wheels()
{
  judge_speed_max();
  filter_speed_F_L();
  filter_speed_F_R();
  filter_speed_B_L();
  filter_speed_B_R();
  pid_control(&pid_F_L, motor.speed_F_L, motor.speed_set_F_L);
  pid_control(&pid_F_R, motor.speed_F_R, motor.speed_set_F_R);
  pid_control(&pid_B_L, motor.speed_B_L, motor.speed_set_B_L);
  pid_control(&pid_B_R, motor.speed_B_R, motor.speed_set_B_R);
  
  if(pid_F_L.result > 0)
  {
    if(pid_F_L.result > SPEED_MAX)
      pid_F_L.result = SPEED_MAX;
    pwm_duty(F_L_GO_PWMCH,(uint32)pid_F_L.result);
    pwm_duty(F_L_BACK_PWMCH,0);
  }
  else
  {
    if(pid_F_L.result < SPEED_MIN)
      pid_F_L.result = SPEED_MIN;
    pwm_duty(F_L_BACK_PWMCH,(uint32)(-pid_F_L.result));
    pwm_duty(F_L_GO_PWMCH,0);
  }
  
  if(pid_F_R.result > 0)
  {
    if(pid_F_R.result > SPEED_MAX)
      pid_F_R.result = SPEED_MAX;
    pwm_duty(F_R_GO_PWMCH,(uint32)pid_F_R.result);
    pwm_duty(F_R_BACK_PWMCH,0);
  }
  else
  {
    if(pid_F_R.result < SPEED_MIN)
      pid_F_R.result = SPEED_MIN;
    pwm_duty(F_R_BACK_PWMCH,(uint32)(-pid_F_R.result));
    pwm_duty(F_R_GO_PWMCH,0);
  }
  
  if(pid_B_L.result > 0)
  {
    if(pid_B_L.result > SPEED_MAX)
      pid_B_L.result = SPEED_MAX;
    pwm_duty(B_L_GO_PWMCH,(uint32)pid_B_L.result);
    pwm_duty(B_L_BACK_PWMCH,0);
  }
  else
  {
    if(pid_B_L.result < SPEED_MIN)
      pid_B_L.result = SPEED_MIN;
    pwm_duty(B_L_BACK_PWMCH,(uint32)(-pid_B_L.result));
    pwm_duty(B_L_GO_PWMCH,0);
  }
  
  if(pid_B_R.result > 0)
  {
    if(pid_B_R.result > SPEED_MAX)
      pid_B_R.result = SPEED_MAX;
    pwm_duty(B_R_GO_PWMCH,(uint32)pid_B_R.result);
    pwm_duty(B_R_BACK_PWMCH,0);
  }
  else
  {
    if(pid_B_R.result < SPEED_MIN)
      pid_B_R.result = SPEED_MIN;
    pwm_duty(B_R_BACK_PWMCH,(uint32)(-pid_B_R.result));
    pwm_duty(B_R_GO_PWMCH,0);
  }
}



void mecanum_control()
{
  if(motor.speed_set_F_L >= 0)//左前
  {
    if(motor.speed_set_F_L > SPEED_MAX)
      motor.speed_set_F_L = SPEED_MAX;
    
    pwm_duty(F_L_GO_PWMCH, motor.speed_set_F_L);
    pwm_duty(F_L_BACK_PWMCH,0);
  }
  else
  {
    if(motor.speed_set_F_L < SPEED_MIN)
      motor.speed_set_F_L = SPEED_MIN;
    
    pwm_duty(F_L_BACK_PWMCH, (-motor.speed_set_F_L));
    pwm_duty(F_L_GO_PWMCH,0);
  }
  
  if(motor.speed_set_F_R >= 0)   //右前
  {
    if(motor.speed_set_F_R > SPEED_MAX)
      motor.speed_set_F_R = SPEED_MAX;
    
    pwm_duty(F_R_GO_PWMCH, motor.speed_set_F_R);
    pwm_duty(F_R_BACK_PWMCH,0);
  }
  else
  {
    if(motor.speed_set_F_R < SPEED_MIN)
      motor.speed_set_F_R = SPEED_MIN;
    
    pwm_duty(F_R_BACK_PWMCH, (-motor.speed_set_F_R));
    pwm_duty(F_R_GO_PWMCH,0);
  }
  
  if(motor.speed_set_B_L >= 0)
  {
    if(motor.speed_set_B_L > SPEED_MAX)
      motor.speed_set_B_L = SPEED_MAX;
    
    pwm_duty(B_L_GO_PWMCH, motor.speed_set_B_L);
    pwm_duty(B_L_BACK_PWMCH,0);
  }
  else
  {
    if(motor.speed_set_B_L < SPEED_MIN)
      motor.speed_set_B_L = SPEED_MIN;
    
    pwm_duty(B_L_BACK_PWMCH, (-motor.speed_set_B_L));
    pwm_duty(B_L_GO_PWMCH,0);
  }
  
  if(motor.speed_set_B_R >= 0)
  {
    if(motor.speed_set_B_R > SPEED_MAX)
      motor.speed_set_B_R = SPEED_MAX;
    
    pwm_duty(B_R_GO_PWMCH, motor.speed_set_B_R);
    pwm_duty(B_R_BACK_PWMCH,0);
  }
  else
  {
    if(motor.speed_set_B_R < SPEED_MIN)
      motor.speed_set_B_R = SPEED_MIN;
    
    pwm_duty(B_R_BACK_PWMCH, (-motor.speed_set_B_R));
    pwm_duty(B_R_GO_PWMCH,0);
  }
}

void car_control()
{
  if(stop_flag)
  {
    break_down();
  }
  else
  {
    speed_control();
    direction_control();
    avoid_obstacle();
//      mecanum_control(); //开环控制
    roll_wheels();      //PI闭环控制
  }

}