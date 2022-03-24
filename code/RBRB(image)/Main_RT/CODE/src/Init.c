#include "include.h"
#include "Init.h"
#include "headfile.h"

void Init()
{
  NVIC_SetPriority(PIT_IRQn, 1);
  
  scc8660_csi_init();	//初始化摄像头 使用CSI接口
  CreateTable();
  Show_init();
  oled_init();
  
  BM_init();
  beep_init();
  while(!BMX055_init());
  motor_init();
  Encoder_init();
  PIT_init();
  
  speed_mode();//速度选档
  
}

void BM_init()
{
  gpio_init(BM1, GPI, 0, GPIO_PIN_CONFIG);
  gpio_init(BM2, GPI, 0, GPIO_PIN_CONFIG);
}

void beep_init()
{
  gpio_init(beep_pin, GPO, 0, GPIO_PIN_CONFIG);
}

void PIT_init()
{
  pit_init();
  pit_interrupt_ms(PIT_CH0, PIT_TIME);

}

void speed_mode()
{
  if(BM1_get && BM2_get)            speed_level = 1;
  else if(BM1_get && !BM2_get)      speed_level = 2;
  else if(!BM1_get && BM2_get)      speed_level = 3;
  else if(!BM1_get && !BM2_get)     speed_level = 4;
}