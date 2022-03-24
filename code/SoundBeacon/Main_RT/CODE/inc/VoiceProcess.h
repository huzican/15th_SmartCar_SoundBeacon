#ifndef     __VOICEPROCESS_H__
#define     __VOICEPROCESS_H__

#include "common.h"

/*
 * ADC数据采集麦克风信号
 *
 *     MIC1     MIC2
 *
 *          
 *
 *     MIC4     MIC3
 */
#define MIC1_ADC    ADC_1
#define MIC1_CH     ADC1_CH10_B21   //左前 FL
#define MIC2_ADC    ADC_1
#define MIC2_CH     ADC1_CH12_B23   //右前 FR
#define MIC3_ADC    ADC_1
#define MIC3_CH     ADC1_CH6_B17    //右后 BR
#define MIC4_ADC    ADC_1
#define MIC4_CH     ADC1_CH5_B16    //左后 BL
#define FM_ADC      ADC_1
#define FM_CH       ADC1_CH15_B26    //右声道 

#define FM_FREQUENCE    85.0

#define SAMPLE_FRE 100          //采样频率

//麦克风采集点数和采集频率相关，需要保证采集点所对应的时间刚好与信号时长相同
//由于信号的长度是0.2048s，若设置得采样频率为10KHZ，采样周期为0.1ms,乘以2048个点正好0.2048s
#define MIC_NUM	2048		//定义采集麦克风点数

#define MIC2MIC 20              //两个麦克风之间距离，单位cm
#define ERROR_MAX   10             //偏差最值，由麦克之间距离决定，MIC1TO2/3.4
#define TANGENT_TH  100        //切灯距离
   
extern int sample_finish;   

extern float  mic1_data[MIC_NUM];		//保存麦克风1数据的缓存
extern float  mic2_data[MIC_NUM];		//保存麦克风2数据的缓存
extern float  mic3_data[MIC_NUM];		//保存麦克风3数据的缓存
extern float  mic4_data[MIC_NUM];		//保存麦克风4数据的缓存
extern float  fm_data[MIC_NUM];        //保存FM数据的缓存


extern int16 maxacor_index[4];

extern float beacon_angle;
extern float beacon_error;
extern float beacon_distance;
extern uint8 quadrant;

void voice_init();
void voice_sample();
void normal(float *x, uint16 len);
void xccor(float *acor1, float *acor2, float *acor3, float *acor4, 
           int16 *mic1, int16 *mic2, int16 *mic3, int16 *mic4, int16 *fm, uint16 len);
void seek_max(float *acor1,float *acor2,float *acor3,float *acor4,int16 len,int16 *index);
void voice_progress();
void dis_error_cal(float d1_in, float d2_in, float *d_out, float *error_out);
void dis_angle_cal(float d1_in, float d2_in, float *d_out, float *angle_out);
void cal_beacon();
void beacon_progress();

#endif