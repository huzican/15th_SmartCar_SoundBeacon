#include "arm_const_structs.h"
#include "arm_math.h"
#include "arm_common_tables.h"

#include "VoiceProcess.h"
#include "SEEKFREE_RDA5807.h"
#include "zf_adc.h"
#include "zf_pit.h"
#include "math.h"
#include "zf_systick.h"
#include "Motor.h"

#define PEAK_TH     10000                //峰值阈值
#define MAXACOR_TH  230                  //峰值最大下标，场地最大8m，对于最大下标230

#define C0 331.6                  //空气在0摄氏度速度
#define DILATATION 0.6    //系数
#define TEMPERATURE 28          //空气温度

#define SOUND_VELOCITY 346.6      //声速为331.6+0.6*T    T取25°C

int sample_finish =0;                 //数据采集完成标志
uint8  mic_flag=0;				    //adc采集完成标志位

/*
 * ADC数据采集麦克风信号
 *
 *     MIC1     MIC2
 *
 *          FM
 *
 *     MIC4     MIC3
 */
float  mic1_data[MIC_NUM]={0};		//保存麦克风1数据的缓存
float  mic2_data[MIC_NUM]={0};		//保存麦克风2数据的缓存
float  mic3_data[MIC_NUM]={0};		//保存麦克风3数据的缓存
float  mic4_data[MIC_NUM]={0};		//保存麦克风4数据的缓存
float  fm_data[MIC_NUM]={0};        //保存FM数据的缓存

int16 maxacor_index[4]={0};//相关峰值下标


float beacon_distance=0;//距离
float beacon_angle = 0;//角度
float beacon_error = 0;//偏差
//uint8 quadrant=0;

void voice_init()               //ADC采集初始化，FM初始化
{
    //麦克风采集
  adc_init(MIC1_ADC,MIC1_CH,ADC_12BIT);
  adc_init(MIC2_ADC,MIC2_CH,ADC_12BIT);
  adc_init(MIC3_ADC,MIC3_CH,ADC_12BIT);
  adc_init(MIC4_ADC,MIC4_CH,ADC_12BIT);
  adc_init(FM_ADC,FM_CH,ADC_12BIT);
  
  //FM初始化
  rda5807_init(FM_FREQUENCE);
  
//  pit_interrupt_us(PIT_CH1,SAMPLE_FRE);//麦克风信号采集
}

void voice_sample()             //声音信号采集
{ 
  static int16 mic_sample_num = 0;  

  if(mic_sample_num < MIC_NUM)
  {
  
    //麦克风数据采集
    //在FFT运算中需要减去直流偏量
    //模块直流偏量1.25V   1.25/3.3 *2^12=1551    2^12=4096
    mic1_data[mic_sample_num]=0.05*(adc_convert(MIC1_ADC,MIC1_CH)-1551);
    mic2_data[mic_sample_num]=0.05*(adc_convert(MIC2_ADC,MIC2_CH)-1551);
    mic3_data[mic_sample_num]=0.05*(adc_convert(MIC3_ADC,MIC3_CH)-1551);
    mic4_data[mic_sample_num]=0.05*(adc_convert(MIC4_ADC,MIC4_CH)-1551);

    fm_data[mic_sample_num]=0.05*(adc_convert(FM_ADC,FM_CH)-1100);
     
    mic_sample_num++;//采集点数加一
  }
  else
  {
    mic_sample_num = 0;
    sample_finish=1;
  }
}

void normal(float *x, uint16 len)       //去掉均值
{
  float sum = 0;
  uint16 i = 0;
  
  for(i = 0; i < len; i++)
  {
    sum += x[i];
  }
  
  sum = sum / len;
  
  for(i = 0; i < len; i++)
  {
    x[i] = (x[i] - sum);
  }
  
}


/*
  *     acor1：    MIC1和FM 互相关结果
  *     acor2：    MIC2和FM 互相关结果
  *     acor3：    MIC3和FM 互相关结果
  *     acor4：    MIC4和FM 互相关结果
  *     len  ： 数据长度
  *     index： 存放相关结果最大值下标
 */
void seek_max(float *acor1,float *acor2,float *acor3,float *acor4,int16 len,int16 *index)                 //寻找幅值最大下标
{
  int16 i = 0;
  
  index[0]=0;
  index[1]=0;
  index[2]=0;
  index[3]=0;
  
  for(i=1;i<len;i++)
  {
    if(acor1[i] > acor1[index[0]])
	{
		index[0] = i;
	}
	if(acor2[i] > acor2[index[1]])
	{
		index[1] = i;
	}
	if(acor3[i] > acor3[index[2]])
	{
		index[2] = i;
	}
	if(acor4[i] > acor4[index[3]])
	{
		index[3] = i;
	}
  }
  
}


//声音处理获取峰值下标
void voice_progress()
{ 
  arm_rfft_fast_instance_f32 s;//傅里叶变换结构体参数
  
  float mic1_fft_in[MIC_NUM*2]={0};    //补零结果
  float mic2_fft_in[MIC_NUM*2]={0};
  float mic3_fft_in[MIC_NUM*2]={0};
  float mic4_fft_in[MIC_NUM*2]={0};
  float fm_fft_in[MIC_NUM*2]={0};

  float mic1_fft_out[MIC_NUM*2]={0};    //fft结果
  float mic2_fft_out[MIC_NUM*2]={0};
  float mic3_fft_out[MIC_NUM*2]={0};
  float mic4_fft_out[MIC_NUM*2]={0};
  float fm_fft_out[MIC_NUM*2]={0};

                          
  float acor1[MIC_NUM*2]={0};          //卷积结果
  float acor2[MIC_NUM*2]={0};         //信号1乘以信号2共轭
  float acor3[MIC_NUM*2]={0};
  float acor4[MIC_NUM*2]={0};

  float acor1_ifft[MIC_NUM*2]={0};      //ifft结果
  float acor2_ifft[MIC_NUM*2]={0};
  float acor3_ifft[MIC_NUM*2]={0};
  float acor4_ifft[MIC_NUM*2]={0};
  //去均值
  normal(mic1_data,MIC_NUM);
  normal(mic2_data,MIC_NUM);
  normal(mic3_data,MIC_NUM);
  normal(mic4_data,MIC_NUM);
  normal(fm_data,MIC_NUM);
  
  //补零
  for(int16 i=0;i<MIC_NUM;i++)
  {
    mic1_fft_in[i] = mic1_data[i];
    mic1_fft_in[i+MIC_NUM] = 0;
    
    mic2_fft_in[i] = mic2_data[i];
    mic2_fft_in[i+MIC_NUM] = 0;
    
    mic3_fft_in[i] = mic3_data[i];
    mic3_fft_in[i+MIC_NUM] = 0;
    
    mic4_fft_in[i] = mic4_data[i];
    mic4_fft_in[i+MIC_NUM] = 0;
    
    fm_fft_in[i] = fm_data[i];
    fm_fft_in[i+MIC_NUM] = 0;
  }
  
  //信号傅里叶变换
  arm_rfft_fast_init_f32(&s,MIC_NUM*2);
  arm_rfft_fast_f32(&s, mic1_fft_in, mic1_fft_out, 0);
  arm_rfft_fast_f32(&s, mic2_fft_in, mic2_fft_out, 0);
  arm_rfft_fast_f32(&s, mic3_fft_in, mic3_fft_out, 0);
  arm_rfft_fast_f32(&s, mic4_fft_in, mic4_fft_out, 0);
  arm_rfft_fast_f32(&s, fm_fft_in, fm_fft_out, 0); 
  
//  //对信号带通滤波
//  for(int16 i = 0;i < 500;i++)
//  {
//    mic1_fft_out[i] = 0;
//    mic2_fft_out[i] = 0;
//    mic3_fft_out[i] = 0;
//    mic4_fft_out[i] = 0;
//    fm_fft_out[i] = 0;
//  }
//  for(int16 i = 4000; i < 4096;i++)
//  {
//    mic1_fft_out[i] = 0;
//    mic2_fft_out[i] = 0;
//    mic3_fft_out[i] = 0;
//    mic4_fft_out[i] = 0;
//    fm_fft_out[i] = 0;
//  }
  
    //(信号1)*(信号2的共轭)
  //(a+bi)(c-di)=(ac+bd)+(bc-ad)i
  for(int16 i=0;i<MIC_NUM*2;i+=2)
  {
    acor1[i]=mic1_fft_out[i]*fm_fft_out[i]+mic1_fft_out[i+1]*fm_fft_out[i+1];
    acor1[i+1]=mic1_fft_out[i+1]*fm_fft_out[i]-mic1_fft_out[i]*fm_fft_out[i+1];
        
    acor2[i]=mic2_fft_out[i]*fm_fft_out[i]+mic2_fft_out[i+1]*fm_fft_out[i+1];
    acor2[i+1]=mic2_fft_out[i+1]*fm_fft_out[i]-mic2_fft_out[i]*fm_fft_out[i+1];
   
    acor3[i]=mic3_fft_out[i]*fm_fft_out[i]+mic3_fft_out[i+1]*fm_fft_out[i+1];
    acor3[i+1]=mic3_fft_out[i+1]*fm_fft_out[i]-mic3_fft_out[i]*fm_fft_out[i+1];

    acor4[i]=mic4_fft_out[i]*fm_fft_out[i]+mic4_fft_out[i+1]*fm_fft_out[i+1];
    acor4[i+1]=mic4_fft_out[i+1]*fm_fft_out[i]-mic4_fft_out[i]*fm_fft_out[i+1];
    
  }
  
    //傅里叶反变换
    arm_rfft_fast_f32(&s, acor1, acor1_ifft, 1);
    arm_rfft_fast_f32(&s, acor2, acor2_ifft, 1);
    arm_rfft_fast_f32(&s, acor3, acor3_ifft, 1);
    arm_rfft_fast_f32(&s, acor4, acor4_ifft, 1);
    
    //寻找ifft变换结果最大幅值下标
    seek_max(acor1_ifft,acor2_ifft,acor3_ifft,acor4_ifft,MAXACOR_TH,maxacor_index);
    
    if(acor1_ifft[maxacor_index[0]] < PEAK_TH)
      maxacor_index[0] = 0;
    if(acor2_ifft[maxacor_index[1]] < PEAK_TH)
      maxacor_index[1] = 0;
    if(acor3_ifft[maxacor_index[2]] < PEAK_TH)
      maxacor_index[2] = 0;
    if(acor4_ifft[maxacor_index[3]] < PEAK_TH)
      maxacor_index[3] = 0;
}

//计算距离和角度
//d1_in:左麦克到灯距离，d2_in:右麦克到灯距离，d_out ，error_out两麦克中心到灯距离与偏差
//偏差左+右-
void dis_error_cal(float d1_in, float d2_in, float *d_out, float *error_out)
{
  *error_out = (d2_in - d1_in);
  d1_in*=3.4;
  d2_in*=3.4;
  *d_out = sqrtf((2*d1_in*d1_in + 2*d2_in*d2_in - MIC2MIC*MIC2MIC)/4);
}

//angle是beacon到mic中点的连线与x轴正向的夹角余弦值
//左- 右+
void dis_angle_cal(float d1_in, float d2_in, float *d_out, float *angle_out)
{
  d1_in*=3.4;
  d2_in*=3.4;
  *d_out = sqrtf((2*d1_in*d1_in + 2*d2_in*d2_in - MIC2MIC*MIC2MIC)/4);
  *angle_out = ((*d_out)*(*d_out)+0.25*MIC2MIC*MIC2MIC-d2_in*d2_in)/(2*(*d_out)*MIC2MIC);
}

//计算距离和角度信息
void cal_beacon()
{
  if(!(maxacor_index[0]||maxacor_index[1]||maxacor_index[2]||maxacor_index[3]))
  {
    stop_flag=1;
  }
  else
  {
    stop_flag=0;
    if((maxacor_index[0]+maxacor_index[1]) <= (maxacor_index[2]+maxacor_index[3]))
    {
      motor_dir = 1;  //灯在前面
      dis_error_cal(maxacor_index[0],maxacor_index[1],&beacon_distance,&beacon_error);  
      
      if(beacon_error > ERROR_MAX)      beacon_error = ERROR_MAX;   //偏差限幅
      else if(beacon_error < -ERROR_MAX)  beacon_error = -ERROR_MAX;
    }
    else if((maxacor_index[0]+maxacor_index[1]) > (maxacor_index[2]+maxacor_index[3]))
    {
      motor_dir = -1;
      dis_error_cal(maxacor_index[2],maxacor_index[3],&beacon_distance,&beacon_error);

      if(beacon_error > ERROR_MAX)      beacon_error = ERROR_MAX;
      else if(beacon_error < -ERROR_MAX)  beacon_error = -ERROR_MAX; 
    }
    
    if(beacon_distance < TANGENT_TH)//切灯处理
    {
      if(motor_dir == 1)
        beacon_error =  (maxacor_index[1]+maxacor_index[2])-(maxacor_index[0]+maxacor_index[3]);
      else if(motor_dir == -1)
        beacon_error =  (maxacor_index[0]+maxacor_index[3])-(maxacor_index[1]+maxacor_index[2]);
      
      if(beacon_error >= 0)//灯在左边
      {
        if(beacon_distance >=92)
          beacon_error -= 2;
        else if(beacon_distance >= 86)
          beacon_error -= 3;
        else if(beacon_distance >= 74)
          beacon_error -= 4;
        else if(beacon_distance >= 59)
          beacon_error -= 5;
        else if(beacon_distance >= 23)
          beacon_error -= 6;
        else if(beacon_distance >= 6)
          beacon_error -= 7;
        else
          beacon_error -= 8;
      }
      else
      {
        if(beacon_distance >=92)
          beacon_error += 2;
        else if(beacon_distance >= 86)
          beacon_error += 3;
        else if(beacon_distance >= 74)
          beacon_error += 4;
        else if(beacon_distance >= 59)
          beacon_error += 5;
        else if(beacon_distance >= 23)
          beacon_error += 6;
        else if(beacon_distance >= 6)
          beacon_error += 7;
        else
          beacon_error += 8;
      }
    }
  }
}

void beacon_progress()
{
  voice_progress();
  cal_beacon();
}
////计算距离和角度信息
//void cal_beacon()
//{
//  float mic2beacon[4];
//  float sound_velocity;           //C0+sqrt(1+DILATATION*TEMPERATURE)
//  
//  sound_velocity = C0+DILATATION*TEMPERATURE;   //计算声速
//  
//  //距离计算 index*100*10^-6*Vs*100 cm
//  mic2beacon[0] = 0.01 * sound_velocity * maxacor_index[0];
//  mic2beacon[1] = 0.01 * sound_velocity * maxacor_index[1];
//  mic2beacon[2] = 0.01 * sound_velocity * maxacor_index[2];
//  mic2beacon[3] = 0.01 * sound_velocity * maxacor_index[3];
//  
//  if(maxcor_index[0])
//}

