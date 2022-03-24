#ifndef  _POSTURE_H_
#define  _POSTURE_H_

#include "common.h"

#define    SPIN_COE      3.8
#define    SAMPLE_RATE   0.005

typedef struct car_status
{
  float yaw[5];                 //Æ«º½½Ç
  float yaw_rate[5];            //Æ«º½½ÇËÙ¶È
  float ideal_yaw_rate;
}car_status;
extern car_status car;

/*****GYRO*****/
typedef struct{
  float X_data;
  float Y_data;
  float Z_data;
  uint8 GYRO_CAL_FLAG;
}Gyrotypedef;
extern Gyrotypedef Gyro;

void get_posture();
void Kalman_filter();

extern void get_posture();
extern void Kalman_filter();



#endif