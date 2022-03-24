#ifndef   _CAMERA_H_
#define   _CAMERA_H_

#include "common.h"

typedef enum
{
  ips200,
  ips114,
  tft
}ImgShow_e;
extern ImgShow_e  img_show; 

typedef enum
{
  RGB,
  BW
}ImgType_e;
extern ImgType_e img_type;

void Show_init();
void img_display();

extern void Show_init();
extern void img_display();



#endif