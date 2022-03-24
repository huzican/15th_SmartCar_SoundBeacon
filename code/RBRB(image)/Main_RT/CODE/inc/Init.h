#ifndef   _INIT_H_
#define   _INIT_H_

#define BM1     D4
#define BM2     D27
#define BM1_get     (gpio_get(BM1))
#define BM2_get     (gpio_get(BM2))

#define beep_pin    B11    

void Init();
void PIT_init();
void speed_mode();
void BM_init();
void beep_init();

extern void Init();
extern void PIT_init();
extern void speed_mode();
extern void BM_init();
extern void beep_init();

#endif