#include "TFmini.h"
//#include "common.h"
//#include "zf_uart.h"

uint8 TF_flag1 = 1;     //激光雷达有获取信息的标志位
uint8 TF_flag2 = 1;
uint8 TF_flag3 = 1;
uint8 TF_flag4 = 1;

void parse_info(uint8 *data_buffer,uint8 num, UARTN_enum uartn)
{
  int16 dist;
  uint16 strength;
//  float temprature;
  int16 check;
  check = (uint8)(data_buffer[0] + data_buffer[1] + data_buffer[2] + data_buffer[3] + data_buffer[4] + data_buffer[5] + data_buffer[6] + data_buffer[7]);
  if (data_buffer[8] == (check & (uint8)0xff))
  {
    dist = (int16)(*(data_buffer+2)<<0) | (int16)(*(data_buffer+3)<<8);
    strength = (int16)(*(data_buffer+4)<<0) | (int16)(*(data_buffer+5)<<8);
//    temprature = (int)(*(data_buffer+6)<<0) | (int)(*(data_buffer+7)<<8);
//    temprature = temprature/8 - 256;
    if(strength>100 && strength!=65535 && dist<RELIABLE_DIS)
    {                   //检测到障碍
      if(uartn == TF1_UART)
      {
        TF_flag1 = 1;
        sensor_data.TFminiFlag |= ((uint8)(0x01<<0));
        if(dist<DANGER_DIS)
        {
          sensor_data.TFminiFlag |= ((uint8)(0x01<<3));
        }
        else
        {
          sensor_data.TFminiFlag &= (uint8)(~(0x01<<3));
        }
      }
      else if(uartn == TF2_UART)
      {
        TF_flag2 = 1;
        sensor_data.TFminiFlag |= ((uint8)(0x01<<1));
        if(dist<DANGER_DIS)
        {
          sensor_data.TFminiFlag |= ((uint8)(0x01<<4));
        }
        else
        {
          sensor_data.TFminiFlag &= (uint8)(~(0x01<<4));
        }
      }
      else if(uartn == TF3_UART)
      {
        TF_flag3 = 1;
        sensor_data.TFminiFlag |= ((uint8)(0x01<<2));
        if(dist<DANGER_DIS)
        {
          sensor_data.TFminiFlag |= ((uint8)(0x01<<5));
        }
        else
        {
          sensor_data.TFminiFlag &= (uint8)(~(0x01<<5));
        }
      }
      else if(uartn == TF4_UART)
      {
        TF_flag4 = 1;
        sensor_data.TFminiFlag |= ((uint8)(0x01<<3));
        if(dist<DANGER_DIS)
        {
          sensor_data.TFminiFlag |= ((uint8)(0x01<<6));
        }
        else
        {
          sensor_data.TFminiFlag &= (uint8)(~(0x01<<6));
        }
      }
      
    }
    else
    {                   //没有检测到障碍
      if(uartn == TF1_UART)
      {
        TF_flag1 = 1;
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<0));
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<3));
      }
      else if(uartn == TF2_UART)
      {
        TF_flag2 = 1;
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<1));
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<4));
      }
      else if(uartn == TF3_UART)
      {
        TF_flag3 = 1;
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<2));
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<5));
      }
      else if(uartn == TF4_UART)
      {
        TF_flag4 = 1;
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<3));
        sensor_data.TFminiFlag &= (uint8)(~(0x01<<6));
      }
    }
  }
}

void TF_ISR(UARTN_enum uartn)
{
  uint8 TF_data;
  uart_getchar(uartn, &TF_data);
  static uint8 data_buffer[9];
  static uint8 data_cnt = 0;
  static uint8 TF_state = 0;
  if(TF_state==0&&TF_data==HEADER)
  {
    TF_state=1;
    data_buffer[0] = HEADER;
  }
  else if(TF_state==1&&TF_data==HEADER)
  {
    TF_state=2;
    data_buffer[1] = HEADER;
    data_cnt = 1;
  }
  else if(TF_state==2)
  {
    data_buffer[++data_cnt]=TF_data;
    if(data_cnt>=8)
    {
      TF_state = 0;
      parse_info(data_buffer, data_cnt, uartn);
    }
  }
  else
  {
//    if (TF_state != 0)
//      printf("get info error, state: %d/n", state);
    TF_state = 0;
  }
}
