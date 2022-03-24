#include "headfile.h"
#include "include.h"
#include "isr.h"
#include "Init.h"
#include "VoiceProcess.h"

float var[8];

void main()
{
    DisableGlobalIRQ();
    board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    systick_delay_ms(100);	//延时100ms，等待主板其他外设上电成功
    
    Init();     //外设等初始化函数
    
    EnableGlobalIRQ(0);
    
    while(1)
    {     
      if(sample_finish)
      {
        sample_finish=0;
        beacon_progress();
      }
      car_control();
      oled_printf_int32(0,0,maxacor_index[0],5);
      oled_printf_int32(63,0,maxacor_index[1],5);
      oled_printf_int32(63,1,maxacor_index[2],5);
      oled_printf_int32(0,1,maxacor_index[3],5);
      oled_printf_float(0,2,beacon_error,5,5);
      oled_printf_float(63,2,beacon_distance,5,5);
      oled_printf_int32(0,3,mpu_gyro_z,10);

      oled_printf_int32(0,4,block_distance1,5);
      oled_printf_int32(63,4,block_distance2,5);
      oled_printf_int32(0,5,block_distance3,5);
      oled_printf_int32(63,5,block_distance4,5);
    }  
}