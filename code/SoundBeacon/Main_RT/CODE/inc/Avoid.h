#ifndef   _AVOID_H_
#define   _AVOID_H_

#include "common.h"

#define AVOID_TIME 5

#define LEFT        1
#define RIGHT       -1

typedef enum
{
  TRANSLATE_S = 1,
  TRANSLATE_D = 2,
  TANGENT = 3,
  DRIFT = 4,  
  SPIN = 5  
} AvoidMode_e;

extern int8 avoid_dir; 
extern uint8 avoid_flag;
extern uint8 merge_flag;  
extern AvoidMode_e avoid_mode;

void avoid_check();
void avoiding();
void avoid_obstacle();



#endif