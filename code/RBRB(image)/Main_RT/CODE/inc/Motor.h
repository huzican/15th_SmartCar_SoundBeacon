#ifndef   _MOTOR_H_
#define   _MOTOR_H_

#include "common.h"

//电机驱动频率
#define         MOTOR_FRE       10000

//初始化频率
#define         INIT_DUTY       0

//PWM_DUTY_MAX在fsl_pwm.h文件中 改为10000
#define         PWM_PRECISON    PWM_DUTY_MAX    //PWM精度

//速度限幅
#define         SPEED_MAX   9999
#define         SPEED_MIN  -9999

//左前电机PWM通道
#define         F_L_GO_PWMCH      PWM1_MODULE0_CHB_D13
#define         F_L_BACK_PWMCH    PWM1_MODULE0_CHA_D12

//右前电机PWM通道
#define         F_R_GO_PWMCH      PWM2_MODULE0_CHB_C7
#define         F_R_BACK_PWMCH    PWM2_MODULE0_CHA_C6

//左后电机PWM通道
#define         B_L_GO_PWMCH      PWM3_MODULE0_CHB_E30
#define         B_L_BACK_PWMCH    PWM3_MODULE0_CHA_E29

//右后电机PWM通道
#define         B_R_GO_PWMCH      PWM4_MODULE0_CHB_E1
#define         B_R_BACK_PWMCH    PWM4_MODULE0_CHA_B24

extern int8 speed_level;

typedef struct motor_status
{
  int16 speed_F_L;
  int16 speed_set_F_L;
  int16 speed_F_L_save[3];
  
  int16 speed_F_R;
  int16 speed_set_F_R;
  int16 speed_F_R_save[3];
  
  int16 speed_B_L;
  int16 speed_set_B_L;
  int16 speed_B_L_save[3];
  
  int16 speed_B_R;
  int16 speed_set_B_R;
  int16 speed_B_R_save[3];
  
  int16 speed_set;
}motor_status;

extern motor_status motor;

void motor_init();//初始化电机
void speed_control();//速度控制
void filter_speed();//速度滤波
void filter_speed_F_L();
void filter_speed_F_R();
void filter_speed_B_L();
void filter_speed_B_R();
void roll_wheels();
void judge_speed_max();//速度限幅
void break_down();


#endif