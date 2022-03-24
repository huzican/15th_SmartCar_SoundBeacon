/********************************��̬��������********************************
*           ������������            ���ٶȼ�������
*           +-250��/s��  131         +-2g  �� 16384    
*           +-500��/s��  65.5        +-4g  �� 8192
*           +-1000��/s�� 32.8        +-8g  �� 4096
*           +-2000��/s�� 16.4        +-16g  :  2048
*
****************************************************************************/
#include "Posture.h"
#include "BMX055.h"


// ��ƫ����                              //+-4g           //+-8g
//#define    GYRO_X_DATA0          0          //-320
//#define    GYRO_Y_DATA0          0          //95
#define    GYRO_Z_DATA0          0          //-265

/* ������ */
#define    GYRO_SENS             32.8    //32.8//65.5
#define    ACCE_SENS             512     //512//8192


//������
Gyrotypedef          Gyro;


car_status car;
BMX055_t bmx055;


void get_posture()
{
  BMX055_DataRead(&bmx055, 1);
  Gyro.Z_data = (float)((bmx055.GYROZdata - GYRO_Z_DATA0) / GYRO_SENS);
  car.yaw_rate[0] = Gyro.Z_data;
  car.yaw[0] += Gyro.Z_data*0.005;              //��ʱ��ȡ���ݣ���ʱ����5ms
  if(car.yaw[0] > 180)
    car.yaw[0] -= 360;
  else if(car.yaw[0] < -180)
    car.yaw[0] += 360;
}




/******�������˲�*****/

//״̬�任��
float A1_1 = 1;
float A1_2 = -1 * SAMPLE_RATE;
float A2_1 = 0;
float A2_2 = 1;
float B1_1 = SAMPLE_RATE;
float B2_1 = 0;
float H1_1 = 1;
float H1_2 = 0;
//ϵ����  
float Pest1_1;          //�������Э������ 
float Pest1_2;
float Pest2_1;
float Pest2_2;    
float Psta1_1 = 1;      //�������Э������
float Psta1_2 = 1;  
float Psta2_1 = 1;  
float Psta2_2 = 1;   
float Q1_1 = 0.00001;   //���̼�������Э���0.00001����
float Q1_2 = 0;  
float Q2_1 = 0;  
float Q2_2 = 0.00001;   //0.00001����
float K1_1;             //����������
float K2_1;    
float R = 0.0045;          //�۲�����Э�����Ҫ���˲���
float I1_1 = 1;         //��λ��
float I1_2 = 0;  
float I2_1 = 0;  
float I2_2 = 1;   
//״̬��
float Xest1_1;          //����״̬����
float Xest2_1;          
float Xsta1_1 = 0;      //����״̬����
float Xsta2_1 = 0;    
float angle;
float angular;

void Kalman_filter()
{
    float th_acc,w_gyro;
    th_acc = car.yaw[0];		//6050���ٶȼƲ����ĽǶ�
    w_gyro = car.yaw_rate[0];		//6050�����ǲ����Ľ��ٶ�
    
    Xest1_1 = Xsta1_1 + A1_2*Xsta2_1 + B1_1*w_gyro;//
    Xest2_1 = Xsta2_1;
    
    Pest1_1 = (Psta1_1 + A1_2*Psta2_1) + (Psta1_2 + A1_2*Psta2_2)*A1_2 + Q1_1;//��˹��������K-1ʱ������ƫ���Ŀǰ��ȷ����ƫ��ƽ���͵Ŀ���
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

    car.yaw[0] = Xsta1_1;		//�˲���ĽǶ�ֵ
    car.yaw_rate[0] = w_gyro - Xsta2_1;	//�˲���Ľ��ٶ�ֵ ǰ��Ϊ��������Ϊ��
}
