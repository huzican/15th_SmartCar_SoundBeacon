#ifndef _ENCODER_H_
#define _ENCODER_H_


#define PIT_TIME    5

#define F_L_QTIMER             QTIMER_1
#define F_L_ENCODER_PHASE_A     QTIMER1_TIMER0_C0
#define F_L_ENCODER_PHASE_B     QTIMER1_TIMER1_C1

#define F_R_QTIMER             QTIMER_2
#define F_R_ENCODER_PHASE_A     QTIMER2_TIMER0_C3
#define F_R_ENCODER_PHASE_B     QTIMER2_TIMER1_C4

#define B_L_QTIMER             QTIMER_3
#define B_L_ENCODER_PHASE_A     QTIMER3_TIMER0_C6
#define B_L_ENCODER_PHASE_B     QTIMER3_TIMER1_C7

#define B_R_QTIMER             QTIMER_4
#define B_R_ENCODER_PHASE_A     QTIMER4_TIMER0_C9
#define B_R_ENCODER_PHASE_B     QTIMER4_TIMER1_C10



void Encoder_init();
void speed_get();
void filter_F_L();
void filter_F_R();
void filter_B_L();
void filter_B_R();
void speed_filter();

extern void Encoder_init();
extern void speed_get();
extern void filter_F_L();
extern void filter_F_R();
extern void filter_B_L();
extern void filter_B_R();
extern void speed_filter();

#endif