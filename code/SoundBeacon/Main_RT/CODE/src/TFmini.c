#include "TFmini.h"

int block_distance1;    //障碍距离
int block_distance2;
int block_distance3;
int block_distance4;

lpuart_handle_t     tf1_lpuartHandle;   //串口中断结构体变量
lpuart_transfer_t   tf1_receivexfer;    //串口中断接收数据

lpuart_handle_t     tf2_lpuartHandle;
lpuart_transfer_t   tf2_receivexfer;

lpuart_handle_t     tf3_lpuartHandle;
lpuart_transfer_t   tf3_receivexfer;

lpuart_handle_t     tf4_lpuartHandle;
lpuart_transfer_t   tf4_receivexfer;

void TFmini_init()      //激光雷达初始化，务必记得配置isr.c中uart中断
{
  uart_init (TF1_UART, TF1_BAUD,TF1_UART_TX_PIN,TF1_UART_RX_PIN);
  uart_init (TF2_UART, TF2_BAUD,TF2_UART_TX_PIN,TF2_UART_RX_PIN);
  uart_init (TF3_UART, TF3_BAUD,TF3_UART_TX_PIN,TF3_UART_RX_PIN);
  uart_init (TF4_UART, TF4_BAUD,TF4_UART_TX_PIN,TF4_UART_RX_PIN);
  
  NVIC_SetPriority(LPUART5_IRQn, 3);
  NVIC_SetPriority(LPUART1_IRQn, 3);
  NVIC_SetPriority(LPUART6_IRQn, 3);
  NVIC_SetPriority(LPUART8_IRQn, 3);
  
  uart_set_handle(TF1_UART,&tf1_lpuartHandle,tf1_IRQHandle,NULL,0,tf1_receivexfer.data,1);
  uart_set_handle(TF2_UART,&tf2_lpuartHandle,tf2_IRQHandle,NULL,0,tf2_receivexfer.data,1);
  uart_set_handle(TF3_UART,&tf3_lpuartHandle,tf3_IRQHandle,NULL,0,tf3_receivexfer.data,1);
  uart_set_handle(TF4_UART,&tf4_lpuartHandle,tf4_IRQHandle,NULL,0,tf4_receivexfer.data,1);
  
  uart_rx_irq(TF1_UART,1);
  uart_rx_irq(TF2_UART,1);
  uart_rx_irq(TF3_UART,1);
  uart_rx_irq(TF4_UART,1); 
}

void parse_info(uint8 *data_buffer,uint8 num,int *tf_dis)   //获取激光雷达距离信息
{
  int dist;
  int strength;
  float temprature;
  int check;
  check = data_buffer[0] + data_buffer[1] + data_buffer[2] + data_buffer[3] + data_buffer[4] + data_buffer[5] + data_buffer[6] + data_buffer[7];
  if (data_buffer[8] == (check & 0xff))
  {
    dist = (int)(*(data_buffer+2)<<0) | (int)(*(data_buffer+3)<<8);
    strength = (int)(*(data_buffer+4)<<0) | (int)(*(data_buffer+5)<<8);
    temprature = (int)(*(data_buffer+6)<<0) | (int)(*(data_buffer+7)<<8);
    temprature = temprature/8 - 256;
    if(strength>100 && strength!=65535)
    {
      //数据可靠
      *tf_dis = dist;
    }
  }
}

//uart中断服务函数
void tf1_IRQHandle()
{
  uint8 TF_data;
 // UARTN_enum uartn = TF1_UART;
  uart_getchar(TF1_UART, &TF_data);
  static uint8 data_buffer1[9];
  static uint8 data_cnt1 = 0;
  static uint8 TF_state1 = 0;
  if(TF_state1==0&&TF_data==HEADER)
  {
    TF_state1=1;
    data_buffer1[0] = HEADER;
  }
  else if(TF_state1==1&&TF_data==HEADER)
  {
    TF_state1=2;
    data_buffer1[1] = HEADER;
    data_cnt1 = 1;
  }
  else if(TF_state1==2)
  {
    data_buffer1[++data_cnt1]=TF_data;
    if(data_cnt1>=8)
    {
      TF_state1 = 0;
      parse_info(data_buffer1, data_cnt1,&block_distance1);
    }
  }
  else
    TF_state1 = 0;
}

void tf2_IRQHandle()
{
  uint8 TF_data;
  //UARTN_enum uartn = TF2_UART;
  uart_getchar(TF2_UART, &TF_data);
  static uint8 data_buffer2[9];
  static uint8 data_cnt2 = 0;
  static uint8 TF_state2 = 0;
  if(TF_state2==0&&TF_data==HEADER)
  {
    TF_state2=1;
    data_buffer2[0] = HEADER;
  }
  else if(TF_state2==1&&TF_data==HEADER)
  {
    TF_state2=2;
    data_buffer2[1] = HEADER;
    data_cnt2 = 1;
  }
  else if(TF_state2==2)
  {
    data_buffer2[++data_cnt2]=TF_data;
    if(data_cnt2>=8)
    {
      TF_state2 = 0;
      parse_info(data_buffer2, data_cnt2,&block_distance2);
    }
  }
  else
    TF_state2 = 0;
}

void tf3_IRQHandle()
{
  uint8 TF_data;
  //UARTN_enum uartn = TF3_UART;
  uart_getchar(TF3_UART, &TF_data);
  static uint8 data_buffer3[9];
  static uint8 data_cnt3 = 0;
  static uint8 TF_state3 = 0;
  if(TF_state3==0&&TF_data==HEADER)
  {
    TF_state3=1;
    data_buffer3[0] = HEADER;
  }
  else if(TF_state3==1&&TF_data==HEADER)
  {
    TF_state3=2;
    data_buffer3[1] = HEADER;
    data_cnt3 = 1;
  }
  else if(TF_state3==2)
  {
    data_buffer3[++data_cnt3]=TF_data;
    if(data_cnt3>=8)
    {
      TF_state3 = 0;
      parse_info(data_buffer3, data_cnt3,&block_distance3);
    }
  }
  else
    TF_state3 = 0;
}

void tf4_IRQHandle()
{
  uint8 TF_data;
  //UARTN_enum uartn = TF4_UART;
  uart_getchar(TF4_UART, &TF_data);
  static uint8 data_buffer4[9];
  static uint8 data_cnt4 = 0;
  static uint8 TF_state4 = 0;
  if(TF_state4==0&&TF_data==HEADER)
  {
    TF_state4=1;
    data_buffer4[0] = HEADER;
  }
  else if(TF_state4==1&&TF_data==HEADER)
  {
    TF_state4=2;
    data_buffer4[1] = HEADER;
    data_cnt4 = 1;
  }
  else if(TF_state4==2)
  {
    data_buffer4[++data_cnt4]=TF_data;
    if(data_cnt4>=8)
    {
      TF_state4 = 0;
      parse_info(data_buffer4, data_cnt4,&block_distance4);
    }
  }
  else
    TF_state4 = 0;
}