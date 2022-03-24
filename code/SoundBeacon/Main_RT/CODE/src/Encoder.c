#include "Encoder.h"
#include "zf_qtimer.h"
#include "Motor.h"
#include "zf_pit.h"

#define QTIME  5

int16 F_L_save[3];
int16 F_R_save[3];
int16 B_L_save[3];
int16 B_R_save[3];


void Encoder_init()
{
  qtimer_quad_init(F_L_QTIMER, F_L_ENCODER_PHASE_A, F_L_ENCODER_PHASE_B);
  qtimer_quad_init(F_R_QTIMER, F_R_ENCODER_PHASE_A, F_R_ENCODER_PHASE_B);
  qtimer_quad_init(B_L_QTIMER, B_L_ENCODER_PHASE_A, B_L_ENCODER_PHASE_B);
  qtimer_quad_init(B_R_QTIMER, B_R_ENCODER_PHASE_A, B_R_ENCODER_PHASE_B);
}

void speed_get()
{
  motor.speed_F_L = qtimer_quad_get(F_L_QTIMER, F_L_ENCODER_PHASE_A) * QTIME;
  motor.speed_F_R = -qtimer_quad_get(F_R_QTIMER, F_R_ENCODER_PHASE_A) * QTIME;
  motor.speed_B_L = qtimer_quad_get(B_L_QTIMER, B_L_ENCODER_PHASE_A) * QTIME;
  motor.speed_B_R = -qtimer_quad_get(B_R_QTIMER, B_R_ENCODER_PHASE_A) * QTIME;
  
  qtimer_quad_clear(F_L_QTIMER, F_L_ENCODER_PHASE_A);
  qtimer_quad_clear(F_R_QTIMER, F_R_ENCODER_PHASE_A);
  qtimer_quad_clear(B_L_QTIMER, B_L_ENCODER_PHASE_A);
  qtimer_quad_clear(B_R_QTIMER, B_R_ENCODER_PHASE_A);
  
//  filter_F_L();
//  filter_F_R();
//  filter_B_L();
//  filter_B_R();
}

void filter_F_L()
{
  static float a=0;
  float speed_error;
  speed_error = motor.speed_F_L - F_L_save[0];
  a = speed_error/30;   //分母如何选定 每个周期减下来的速度乘以2 再加上一些
  if(a < 0)
    a = -a;
  if(a > 0.85)
    a = 0.85;
  motor.speed_F_L = (int16)(motor.speed_F_L*(1-a) + (F_L_save[0]+F_L_save[1]+F_L_save[2])*a/3);
  F_L_save[2] = F_L_save[1];
  F_L_save[1] = F_L_save[0];
  F_L_save[0] = motor.speed_F_L;
}

void filter_F_R()
{
  static float a=0;
  float speed_error;
  speed_error = motor.speed_F_R - F_R_save[0];
  a = speed_error/30;   //分母如何选定 每个周期减下来的速度乘以2 再加上一些
  if(a < 0)
    a = -a;
  if(a > 0.85)
    a = 0.85;
  motor.speed_F_R = (int16)(motor.speed_F_R*(1-a) + (F_R_save[0]+F_R_save[1]+F_R_save[2])*a/3);
  F_R_save[2] = F_R_save[1];
  F_R_save[1] = F_R_save[0];
  F_R_save[0] = motor.speed_F_R;
}

void filter_B_L()
{
  static float a=0;
  float speed_error;
  speed_error = motor.speed_B_L - B_L_save[0];
  a = speed_error/30;   //分母如何选定 每个周期减下来的速度乘以2 再加上一些
  if(a < 0)
    a = -a;
  if(a > 0.85)
    a = 0.85;
  motor.speed_B_L = (int16)(motor.speed_B_L*(1-a) + (B_L_save[0]+B_L_save[1]+B_L_save[2])*a/3);
  B_L_save[2] = B_L_save[1];
  B_L_save[1] = B_L_save[0];
  B_L_save[0] = motor.speed_B_L;
}

void filter_B_R()
{
  static float a=0;
  float speed_error;
  speed_error = motor.speed_B_R - B_R_save[0];
  a = speed_error/30;   //分母如何选定 每个周期减下来的速度乘以2 再加上一些
  if(a < 0)
    a = -a;
  if(a > 0.85)
    a = 0.85;
  motor.speed_B_R = (int16)(motor.speed_B_R*(1-a) + (B_R_save[0]+B_R_save[1]+B_R_save[2])*a/3);
  B_R_save[2] = B_R_save[1];
  B_R_save[1] = B_R_save[0];
  B_R_save[0] = motor.speed_B_R;
}
void speed_filter()
{
  ;
}