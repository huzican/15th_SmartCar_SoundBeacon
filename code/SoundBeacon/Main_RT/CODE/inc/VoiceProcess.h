#ifndef     __VOICEPROCESS_H__
#define     __VOICEPROCESS_H__

#include "common.h"

/*
 * ADC���ݲɼ���˷��ź�
 *
 *     MIC1     MIC2
 *
 *          
 *
 *     MIC4     MIC3
 */
#define MIC1_ADC    ADC_1
#define MIC1_CH     ADC1_CH10_B21   //��ǰ FL
#define MIC2_ADC    ADC_1
#define MIC2_CH     ADC1_CH12_B23   //��ǰ FR
#define MIC3_ADC    ADC_1
#define MIC3_CH     ADC1_CH6_B17    //�Һ� BR
#define MIC4_ADC    ADC_1
#define MIC4_CH     ADC1_CH5_B16    //��� BL
#define FM_ADC      ADC_1
#define FM_CH       ADC1_CH15_B26    //������ 

#define FM_FREQUENCE    85.0

#define SAMPLE_FRE 100          //����Ƶ��

//��˷�ɼ������Ͳɼ�Ƶ����أ���Ҫ��֤�ɼ�������Ӧ��ʱ��պ����ź�ʱ����ͬ
//�����źŵĳ�����0.2048s�������õò���Ƶ��Ϊ10KHZ����������Ϊ0.1ms,����2048��������0.2048s
#define MIC_NUM	2048		//����ɼ���˷����

#define MIC2MIC 20              //������˷�֮����룬��λcm
#define ERROR_MAX   10             //ƫ����ֵ�������֮����������MIC1TO2/3.4
#define TANGENT_TH  100        //�еƾ���
   
extern int sample_finish;   

extern float  mic1_data[MIC_NUM];		//������˷�1���ݵĻ���
extern float  mic2_data[MIC_NUM];		//������˷�2���ݵĻ���
extern float  mic3_data[MIC_NUM];		//������˷�3���ݵĻ���
extern float  mic4_data[MIC_NUM];		//������˷�4���ݵĻ���
extern float  fm_data[MIC_NUM];        //����FM���ݵĻ���


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