#ifndef _ENCODER_H_
#define _ENCODER_H_


#define PIT_TIME    5

#define F_L_QTIMER              QTIMER_2
#define F_L_ENCODER_PHASE_A     QTIMER2_TIMER3_C25
#define F_L_ENCODER_PHASE_B     QTIMER2_TIMER0_C3

#define F_R_QTIMER              QTIMER_3
#define F_R_ENCODER_PHASE_A     QTIMER3_TIMER3_B19
#define F_R_ENCODER_PHASE_B     QTIMER3_TIMER2_B18

#define B_L_QTIMER              QTIMER_1
#define B_L_ENCODER_PHASE_A     QTIMER1_TIMER1_C1
#define B_L_ENCODER_PHASE_B     QTIMER1_TIMER0_C0

#define B_R_QTIMER              QTIMER_1
#define B_R_ENCODER_PHASE_A     QTIMER1_TIMER3_C24  
#define B_R_ENCODER_PHASE_B     QTIMER1_TIMER2_C2



void Encoder_init();
void speed_get();
void filter_F_L();
void filter_F_R();
void filter_B_L();
void filter_B_R();
void speed_filter();


#endif