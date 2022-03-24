#ifndef     __TFMINI_H__
#define     __TFMINI_H__

#include "common.h"
#include "zf_uart.h"
#include "global_variable.h"

#define HEADER          0x59
#define DIST_LIMIT      180

#define DANGER_DIS      50       //DANGER AREA 0-50
#define RELIABLE_DIS    140      //限制有效距离为140cm


#define TF1_UART            USART_2
#define TF1_BAUD            115200
#define TF1_UART_TX_PIN     UART2_TX_B18
#define TF1_UART_RX_PIN     UART2_RX_B19

#define TF2_UART            USART_3
#define TF2_BAUD            115200
#define TF2_UART_TX_PIN     UART3_TX_B22
#define TF2_UART_RX_PIN     UART3_RX_B23

#define TF3_UART            USART_4
#define TF3_BAUD            115200
#define TF3_UART_TX_PIN     UART4_TX_C16
#define TF3_UART_RX_PIN     UART4_RX_C17

#define TF4_UART            USART_5
#define TF4_BAUD            115200
#define TF4_UART_TX_PIN     UART5_TX_C28
#define TF4_UART_RX_PIN     UART5_RX_C29


extern uint8 TF_flag1;
extern uint8 TF_flag2;
extern uint8 TF_flag3;
extern uint8 TF_flag4;

extern void TF_ISR(UARTN_enum uartn);

#endif