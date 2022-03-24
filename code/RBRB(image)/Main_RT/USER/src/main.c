#include "headfile.h"
#include "include.h"
#include "isr.h"

uint32 use_time;
int main(void)
{
    DisableGlobalIRQ();
    board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
//    adc_init(ADC_1,ADC1_CH0_B27,ADC_8BIT);
//    pit_init();
//    pit_interrupt_ms(PIT_CH1,100);  //初始化pit通道0 周期
//    NVIC_SetPriority(PIT_IRQn,15);
    
    
	//systick_delay_ms(100);	//延时100ms，等待主板其他外设上电成功
    
    rda5807_init(95.0);
    

    EnableGlobalIRQ(0);
    while(1)
    {
      
    }
    
}






