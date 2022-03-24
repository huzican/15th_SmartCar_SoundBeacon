#include "include.h"
#include "Init.h"
#include "headfile.h"
#include "VoiceProcess.h"

#define BULETOOTH_UART  USART_4
#define BULETOOTH_BAUD  115200
#define BULETOOTH_TX    UART4_TX_C16
#define BLUETOOTH_RX    UART4_RX_C17          

void bluetooth_init()
{
  uart_init (BULETOOTH_UART, BULETOOTH_BAUD,BULETOOTH_TX,BLUETOOTH_RX);
}
void vcan_sendware(void *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(BULETOOTH_UART, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(BULETOOTH_UART, (uint8 *)wareaddr, waresize);    //发送数据
    uart_putbuff(BULETOOTH_UART, cmdr, sizeof(cmdr));    //发送后命令
}

void Init()
{
    motor_init();
    mpu6050_init();
//    bluetooth_init();
    voice_init();
    NVIC_SetPriority(PIT_IRQn, 0);
    pit_init();
    pit_interrupt_us(PIT_CH1,SAMPLE_FRE);//麦克风信号采集
    Encoder_init();
    pit_interrupt_ms(PIT_CH2, PIT_TIME);//mpu不放中断，影响声音采集
    oled_init();
    pid_init();
    TFmini_init();
  
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


void speed_mode()
{
  if(BM1_get && BM2_get)            speed_level = 1;
  else if(BM1_get && !BM2_get)      speed_level = 2;
  else if(!BM1_get && BM2_get)      speed_level = 3;
  else if(!BM1_get && !BM2_get)     speed_level = 4;
}

void set_mode()         //速度选档
{
  if(speed_level == 1)
  {
  }
  else if(speed_level==2)
  {
  }
  else if(speed_level==3)
  {
  }
  else if(speed_level==4)
  {
  }
}
