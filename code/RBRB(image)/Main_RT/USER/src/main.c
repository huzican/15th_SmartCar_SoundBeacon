#include "headfile.h"
#include "include.h"
#include "isr.h"

uint32 use_time;
int main(void)
{
    DisableGlobalIRQ();
    board_init();   	//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    
//    adc_init(ADC_1,ADC1_CH0_B27,ADC_8BIT);
//    pit_init();
//    pit_interrupt_ms(PIT_CH1,100);  //��ʼ��pitͨ��0 ����
//    NVIC_SetPriority(PIT_IRQn,15);
    
    
	//systick_delay_ms(100);	//��ʱ100ms���ȴ��������������ϵ�ɹ�
    
    rda5807_init(95.0);
    

    EnableGlobalIRQ(0);
    while(1)
    {
      
    }
    
}






