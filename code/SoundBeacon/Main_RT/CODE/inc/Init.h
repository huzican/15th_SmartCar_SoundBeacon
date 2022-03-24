#ifndef   _INIT_H_
#define   _INIT_H_

#define BM1     D4
#define BM2     D27
#define BM1_get     (gpio_get(BM1))
#define BM2_get     (gpio_get(BM2))

#define beep_pin    B11    

void vcan_sendware(void *wareaddr, uint32 waresize);
void bluetooth_init();
void Init();
void speed_mode();
void BM_init();
void beep_init();
void set_mode() ;

#endif