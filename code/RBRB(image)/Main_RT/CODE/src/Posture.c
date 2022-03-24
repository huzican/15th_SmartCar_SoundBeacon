/********************************姿态解算引擎********************************
*           陀螺仪灵敏度            加速度计灵敏度
*           +-250°/s：  131         +-2g  ： 16384    
*           +-500°/s：  65.5        +-4g  ： 8192
*           +-1000°/s： 32.8        +-8g  ： 4096
*           +-2000°/s： 16.4        +-16g  :  2048
*
****************************************************************************/
#include "Posture.h"
#include "BMX055.h"


// 零偏矫正                              //+-4g           //+-8g
//#define    GYRO_X_DATA0          0          //-320
//#define    GYRO_Y_DATA0          0          //95
#define    GYRO_Z_DATA0          0          //-265

/* 灵敏度 */
#define    GYRO_SENS             32.8    //32.8//65.5
#define    ACCE_SENS             512     //512//8192


//陀螺仪
Gyrotypedef          Gyro;


car_status car;
BMX055_t bmx055;


void get_posture()
{
  BMX055_DataRead(&bmx055, 1);
  Gyro.Z_data = (float)((bmx055.GYROZdata - GYRO_Z_DATA0) / GYRO_SENS);
  car.yaw_rate[0] = Gyro.Z_data;
  car.yaw[0] += Gyro.Z_data*0.005;              //定时读取数据，定时周期5ms
  if(car.yaw[0] > 180)
    car.yaw[0] -= 360;
  else if(car.yaw[0] < -180)
    car.yaw[0] += 360;
}




/******卡尔曼滤波*****/

//状态变换阵
float A1_1 = 1;
float A1_2 = -1 * SAMPLE_RATE;
float A2_1 = 0;
float A2_2 = 1;
float B1_1 = SAMPLE_RATE;
float B2_1 = 0;
float H1_1 = 1;
float H1_2 = 0;
//系数阵  
float Pest1_1;          //先验估计协方差阵 
float Pest1_2;
float Pest2_1;
float Pest2_2;    
float Psta1_1 = 1;      //后验估计协方差阵
float Psta1_2 = 1;  
float Psta2_1 = 1;  
float Psta2_2 = 1;   
float Q1_1 = 0.00001;   //过程激励噪声协方差，0.00001即可
float Q1_2 = 0;  
float Q2_1 = 0;  
float Q2_2 = 0.00001;   //0.00001即可
float K1_1;             //卡尔曼增益
float K2_1;    
float R = 0.0045;          //观测噪声协方差，主要调此参数
float I1_1 = 1;         //单位阵
float I1_2 = 0;  
float I2_1 = 0;  
float I2_2 = 1;   
//状态阵
float Xest1_1;          //先验状态估计
float Xest2_1;          
float Xsta1_1 = 0;      //后验状态估计
float Xsta2_1 = 0;    
float angle;
float angular;

void Kalman_filter()
{
    float th_acc,w_gyro;
    th_acc = car.yaw[0];		//6050加速度计测量的角度
    w_gyro = car.yaw_rate[0];		//6050陀螺仪测量的角速度
    
    Xest1_1 = Xsta1_1 + A1_2*Xsta2_1 + B1_1*w_gyro;//
    Xest2_1 = Xsta2_1;
    
    Pest1_1 = (Psta1_1 + A1_2*Psta2_1) + (Psta1_2 + A1_2*Psta2_2)*A1_2 + Q1_1;//高斯白噪声，K-1时刻最优偏差和目前不确定度偏差平方和的开方
    Pest1_2 =  Psta1_2 + A1_2*Psta2_2;
    Pest2_1 = Psta2_1 + Psta2_2*A1_2;
    Pest2_2 = Psta2_2 + Q2_2;

    K1_1 = Pest1_1/(Pest1_1 + R);//Kg
    K2_1 = Pest2_1/(Pest1_1 + R);

    Xsta1_1 = Xest1_1+K1_1*(th_acc - Xest1_1);        
    Xsta2_1 = Xest2_1+K2_1*(th_acc - Xest1_1);        

    Psta1_1 = (I1_1-K1_1) * Pest1_1;   
    Psta1_2 = (I1_1-K1_1) * Pest1_2;
    Psta2_1 =  -K2_1*Pest1_1 + Pest2_1; 
    Psta2_2 =  -K2_1*Pest1_2+  Pest2_2; 

    car.yaw[0] = Xsta1_1;		//滤波后的角度值
    car.yaw_rate[0] = w_gyro - Xsta2_1;	//滤波后的角速度值 前倾为负，后倾为正
}
