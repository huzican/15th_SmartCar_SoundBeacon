#ifndef   _MOTOR_H_
#define   _MOTOR_H_

#include "common.h"

//�������Ƶ��
#define         MOTOR_FRE       10000

//��ʼ��Ƶ��
#define         INIT_DUTY       0

//PWM_DUTY_MAX��fsl_pwm.h�ļ��� ��Ϊ10000
#define         PWM_PRECISON    PWM_DUTY_MAX    //PWM����

//�ٶ��޷�
#define         SPEED_MAX   5000
#define         SPEED_MIN  -5000

//��ǰ���PWMͨ��
#define         F_L_GO_PWMCH      PWM1_MODULE0_CHA_D12
#define         F_L_BACK_PWMCH    PWM1_MODULE0_CHB_D13

//��ǰ���PWMͨ��
#define         F_R_GO_PWMCH      PWM1_MODULE1_CHA_D14
#define         F_R_BACK_PWMCH    PWM1_MODULE1_CHB_D15

//�����PWMͨ��
#define         B_L_GO_PWMCH      PWM1_MODULE3_CHA_D0
#define         B_L_BACK_PWMCH    PWM1_MODULE3_CHB_D1

//�Һ���PWMͨ��
#define         B_R_GO_PWMCH      PWM2_MODULE3_CHA_D2
#define         B_R_BACK_PWMCH    PWM2_MODULE3_CHB_D3

typedef struct
{
  int16 speed_F_L;              //��ǰ
  int16 speed_set_F_L;
  int16 speed_F_L_save[3];
  
  int16 speed_F_R;              //��ǰ
  int16 speed_set_F_R;
  int16 speed_F_R_save[3];
  
  int16 speed_B_L;              //���
  int16 speed_set_B_L;
  int16 speed_B_L_save[3];
  
  int16 speed_B_R;              //���
  int16 speed_set_B_R;
  int16 speed_B_R_save[3];
  
  int16 speed_set;
  int16 dir_err[5];             //����ƫ��
}motor_status;

extern motor_status motor;
extern int8 motor_dir;       //����˶�����
extern int8 stop_flag;
extern int8 speed_level;

extern int16 X_SPEED_S;     //����Ư���ٶ�
extern int16 X_SPEED_D;

void motor_init();//��ʼ�����
void speed_control();//�ٶȿ���
void filter_speed();//�ٶ��˲�
void filter_speed_F_L();
void filter_speed_F_R();
void filter_speed_B_L();
void filter_speed_B_R();
void roll_wheels();
void judge_speed_max();//�ٶ��޷�
void break_down();
void roll_wheels();
void mecanum_control();
void car_control();
void pull_up();

#endif