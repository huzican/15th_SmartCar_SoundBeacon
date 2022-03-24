#ifndef  __GLOBAL_VARIABLE_H__
#define  __GLOBAL_VARIABLE_H__

typedef struct
{
  uint16 InfoHead;              //帧头0xAEAA，先发低位后发高位
  
  int16 BeaconCordinate_F;      //信标灯坐标偏差
  int16 BeaconWidth_F;           //信标灯大小
  int16 BeaconCordinate_N;      //信标灯坐标偏差
  int16 BeaconWidth_N;           //信标灯大小
  
  uint8 TFminiFlag;             //雷达检测标志位
  /*
        TFminiFlag = 0b 0       0       0       0       0       0       0       0        从高位至低位依次为
                        None    None    Danger3 Danger2 Danger1 TF3     TF2     TF1
        这里的Danger表示有障碍在危险区，TF表示有障碍在避障范围内
  */
  uint8 BackFlag;               //用摄像头避障时的倒车标志位
  
  uint16 InfoTail;              //帧尾0xFFFE，先发低位后发高位
  
}SensorData;
extern SensorData sensor_data;

#define  SENSOR_DATA_LENGTH     sizeof(SensorData)/sizeof(uint8_t)


#endif