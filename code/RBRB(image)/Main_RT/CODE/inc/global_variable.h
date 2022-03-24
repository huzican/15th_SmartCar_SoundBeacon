#ifndef  __GLOBAL_VARIABLE_H__
#define  __GLOBAL_VARIABLE_H__

typedef struct
{
  uint16 InfoHead;              //֡ͷ0xAEAA���ȷ���λ�󷢸�λ
  
  int16 BeaconCordinate_F;      //�ű������ƫ��
  int16 BeaconWidth_F;           //�ű�ƴ�С
  int16 BeaconCordinate_N;      //�ű������ƫ��
  int16 BeaconWidth_N;           //�ű�ƴ�С
  
  uint8 TFminiFlag;             //�״����־λ
  /*
        TFminiFlag = 0b 0       0       0       0       0       0       0       0        �Ӹ�λ����λ����Ϊ
                        None    None    Danger3 Danger2 Danger1 TF3     TF2     TF1
        �����Danger��ʾ���ϰ���Σ������TF��ʾ���ϰ��ڱ��Ϸ�Χ��
  */
  uint8 BackFlag;               //������ͷ����ʱ�ĵ�����־λ
  
  uint16 InfoTail;              //֡β0xFFFE���ȷ���λ�󷢸�λ
  
}SensorData;
extern SensorData sensor_data;

#define  SENSOR_DATA_LENGTH     sizeof(SensorData)/sizeof(uint8_t)


#endif