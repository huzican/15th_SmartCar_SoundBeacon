#ifndef     __TFMINI_H__
#define     __TFMINI_H__

#include "common.h"
#include "zf_uart.h"

#define HEADER          0x59    //Êý¾ÝÖ¡Í·

#define TF1_UART            USART_5
#define TF1_BAUD            115200
#define TF1_UART_TX_PIN     UART5_TX_C28
#define TF1_UART_RX_PIN     UART5_RX_C29

#define TF2_UART            USART_1
#define TF2_BAUD            115200
#define TF2_UART_TX_PIN     UART1_TX_B12
#define TF2_UART_RX_PIN     UART1_RX_B13

#define TF3_UART            USART_6
#define TF3_BAUD            115200
#define TF3_UART_TX_PIN     UART6_TX_B2
#define TF3_UART_RX_PIN     UART6_RX_B3

#define TF4_UART            USART_8
#define TF4_BAUD            115200
#define TF4_UART_TX_PIN     UART8_TX_D16
#define TF4_UART_RX_PIN     UART8_RX_D17

extern int block_distance1;
extern int block_distance2;
extern int block_distance3;
extern int block_distance4;

void TFmini_init();
void parse_info(uint8 *data_buffer,uint8 num,int *tf_dis);
void tf1_IRQHandle();
void tf2_IRQHandle();
void tf3_IRQHandle();
void tf4_IRQHandle();

#endif