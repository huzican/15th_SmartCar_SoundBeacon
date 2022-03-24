#include "Avoid.h"
#include "zf_pit.h"
#include "motor.h"
#include "VoiceProcess.h"
#include "TFmini.h"


#define SAFE_TH     100       //��ȫ���Ͼ���
#define DANGER_TH   35        //�������Ͼ���

#define DRIFT_SPEED_S 800      //��ȫ�������ٶ�
#define DRIFT_SPEED_D 2000      //Σ���������ٶ�

uint8 avoid_flag=0;             //�Ƿ���ϱ�־
int8 avoid_dir=LEFT;            //���Ϸ���
uint8 merge_flag = 0;           //�ƺ��ϰ�һ������Ϊ1
AvoidMode_e avoid_mode;          //����ģʽ


/*
 * �����״����
 *
 *     TF1     TF2
 *
 *          
 *
 *     TF3     TF4
 */

void avoid_check()
{
  if(motor_dir == 1)
  {
    if((block_distance1 <= DANGER_TH && block_distance1 > 0) || (block_distance2 <= DANGER_TH && block_distance2 > 0))//�״��б�����ʾ
    {
      avoid_flag = 1;
      avoid_mode = TRANSLATE_D;
      if(block_distance1 >= block_distance2 &&(block_distance1!=0)&&(block_distance2!=0))
        avoid_dir = LEFT;
      else if(block_distance1 <= block_distance2 &&(block_distance1!=0)&&(block_distance2!=0))
        avoid_dir = RIGHT;
    }
    else if((block_distance1 <= SAFE_TH && block_distance1 > DANGER_TH) || (block_distance2 <= SAFE_TH && block_distance2 > DANGER_TH))
    {
      if(beacon_distance > TANGENT_TH)
      {
        avoid_flag = 1;
        avoid_mode = TRANSLATE_S;
        if(beacon_error >= 0)//������
        {
          avoid_dir = LEFT;
        }
        else
        {
          avoid_dir = RIGHT;
        }
      }
    }
  }
  else if(motor_dir == -1)
  {
    if((block_distance4 <= DANGER_TH && block_distance4 > 0) || (block_distance3 <= DANGER_TH && block_distance3 > 0))//�״��б�����ʾ
    {
      avoid_flag = 1;
      avoid_mode = TRANSLATE_D;
      if(block_distance4 >= block_distance3 &&(block_distance4!=0)&&(block_distance3!=0))
        avoid_dir = LEFT;
      else if(block_distance4 <= block_distance3 &&(block_distance4!=0)&&(block_distance3!=0))
        avoid_dir = RIGHT;
    }
    else if((block_distance4 <= SAFE_TH && block_distance4 > DANGER_TH) || (block_distance3 <= SAFE_TH && block_distance3 > DANGER_TH))
    {
      if(beacon_distance > TANGENT_TH)
      {
        avoid_flag = 1;
        avoid_mode = TRANSLATE_S;
        if(beacon_error >= 0)//������
        {
          avoid_dir = LEFT;
        }
        else
        {
          avoid_dir = RIGHT;
        }
      }
    }
  }
}

//void avoid_check()
//{
//  if(beacon_distance <= TANGENT_TH)//��ֹ�����״��ޱ��Ϸ�Ӧ���ӵ�ģʽ
//  {
//    avoid_flag = 1;
//    avoid_mode = TANGENT;   //�е�
//    merge_flag = 1;
//    
//    tangent_kp = 30;    //�����ٶȵ���
//    tangent_speed = tangent_kp *(TANGENT_TH - (int16)beacon_distance);
//    if(tangent_speed > TANGENT_MAX) tangent_speed = TANGENT_MAX;
//      
//    if(beacon_error >= 0)//�������
//    {
//      avoid_dir = RIGHT;//���ұ��е�
//    }
//    else
//    {
//      avoid_dir = LEFT;//������е�
//    }
//  }
//  else
//  {
//    merge_flag = 0;
//  }
//  
//  if(motor_dir  == 1) //�����״����
//  {
//    if((block_distance1 <= DANGER_TH && block_distance1 > 0) || (block_distance2 <= DANGER_TH && block_distance2 > 0))//�״��б�����ʾ
//    {
//      avoid_flag = 1;
//      avoid_mode = TRANSLATE_D;
//      if(block_distance1 >= block_distance2 &&(block_distance1!=0)&&(block_distance2!=0))
//        avoid_dir = LEFT;
//      else if(block_distance1 <= block_distance2 &&(block_distance1!=0)&&(block_distance2!=0))
//        avoid_dir = RIGHT;
////      if(merge_flag)//�ƺ��ϰ���ͬһ������
////      {
////        if(beacon_error >= 0)//������
////        {
////          avoid_dir = RIGHT;
////        }
////        else
////        {
////          avoid_dir = LEFT;
////        }
////      }
////      else
////      {
////        if(beacon_error >= 0)//������
////        {
////          avoid_dir = LEFT;
////        }
////        else
////        {
////          avoid_dir = RIGHT;
////        }
////      }
//    }
//    else if((block_distance1 <= SAFE_TH && block_distance1 > DANGER_TH) || (block_distance2 <= SAFE_TH && block_distance2 > DANGER_TH))
//    {
//      avoid_flag = 1;
//      if(!merge_flag)
//      {
//        avoid_mode = TRANSLATE_S;
//        if(beacon_error >= 0)//������
//        {
//          avoid_dir = LEFT;
//        }
//        else
//        {
//          avoid_dir = RIGHT;
//        }
//      }
//    }
//  }
//  else if(motor_dir == -1)
//  {
//    if((block_distance4 <= DANGER_TH && block_distance4 > 0) || (block_distance3 <= DANGER_TH && block_distance3 > 0))//�״��б�����ʾ
//    {
//      avoid_flag = 1;
//      avoid_mode = TRANSLATE_D;
//      if(block_distance4 >= block_distance3 &&(block_distance4!=0)&&(block_distance3!=0))
//        avoid_dir = LEFT;
//      else if(block_distance4 <= block_distance3 &&(block_distance4!=0)&&(block_distance3!=0))
//        avoid_dir = RIGHT;
////      if(merge_flag)//�ƺ��ϰ���ͬһ������
////      {
////        if(beacon_error >= 0)//������
////        {
////          avoid_dir = RIGHT;
////        }
////        else
////        {
////          avoid_dir = LEFT;
////        }
////      }
////      else
////      {
////        if(beacon_error >= 0)//������
////        {
////          avoid_dir = LEFT;
////        }
////        else
////        {
////          avoid_dir = RIGHT;
////        }
////      }
//    }
//    else if((block_distance4 <= SAFE_TH && block_distance4 > DANGER_TH) || (block_distance3 <= SAFE_TH && block_distance3 > DANGER_TH))
//    {
//      avoid_flag = 1;
//      if(!merge_flag)
//      {
//        avoid_mode = TRANSLATE_S;
//        if(beacon_error >= 0)//������
//        {
//          avoid_dir = LEFT;
//        }
//        else
//        {
//          avoid_dir = RIGHT;
//        }
//      }
//    }
//  }
//}
//void avoid_check()
//{
//  if(motor_dir == 1)
//  {
//    if((block_distance1 <= SAFE_TH && block_distance1 > 0) || (block_distance2 <= SAFE_TH && block_distance2 > 0))
//    {
//      avoid_flag = 1;
//      if(fabs(block_distance1-beacon_distance)<20 || fabs(block_distance2-beacon_distance)<20)
////      if(beacon_distance < 80)
//       {
//         merge_flag = 1;        //�ƺ��ϰ���ͬһ������
//         if(beacon_error >= 0)  //�������
//           avoid_dir = RIGHT;   //�������ұ�
//         else
//           avoid_dir = LEFT;
//       }
//      else
//       {
//         merge_flag = 0;        //�ƺ��ϰ�����������
//         if(beacon_error >= 0)  //�������
//           avoid_dir = LEFT;    //���������
//         else
//           avoid_dir = RIGHT;
//       }
//      
//      if((block_distance1 <= DANGER_TH && block_distance1 > 0) || (block_distance2 <= DANGER_TH && block_distance2 > 0))//��������
//      {
//        avoid_mode = TRANSLATE_D;       //���ٺ���
//      }
//      else if((block_distance1 <= SAFE_TH && block_distance1 > DANGER_TH) || (block_distance2 <= SAFE_TH && block_distance2 > DANGER_TH))
//      {
//        avoid_mode = TRANSLATE_S;
//      }
//    }
//    else
//    {
//      avoid_flag = 0;
//    }
//  }
//  else if(motor_dir == -1)
//  {
//    if((block_distance4 <= SAFE_TH && block_distance4 > 0) || (block_distance3 <= SAFE_TH && block_distance3 > 0))
//    {
//      avoid_flag = 1;
//      if(fabs(block_distance4-beacon_distance)<20 || fabs(block_distance3-beacon_distance)<20)
//       {
//         merge_flag = 1;        //�ƺ��ϰ���ͬһ������
//         if(beacon_error >= 0)  //�������
//           avoid_dir = RIGHT;   //�������ұ�
//         else
//           avoid_dir = LEFT;
//       }
//      else
//       {
//         merge_flag = 0;        //�ƺ��ϰ�����������
//         if(beacon_error >= 0)  //�������
//           avoid_dir = LEFT;    //���������
//         else
//           avoid_dir = RIGHT;
//       }
//      
//      if((block_distance4 <= DANGER_TH && block_distance4 > 0) || (block_distance3 <= DANGER_TH && block_distance3 > 0))//��������
//      {
//        avoid_mode = TRANSLATE_D;       //���ٺ���
//      }
//      else if((block_distance4 <= SAFE_TH && block_distance4 > DANGER_TH) || (block_distance3 <= SAFE_TH && block_distance3 > DANGER_TH))
//      {
//        avoid_mode = TRANSLATE_S;
//      }
//    }
//    else
//    {
//      avoid_flag = 0;
//    }
//  }
//
//}



void avoiding()
{
  switch(avoid_mode)
  {
    case TRANSLATE_S:
    {
      if(avoid_dir == LEFT)
      {
        motor.speed_set_F_L -= DRIFT_SPEED_S * motor_dir;
        motor.speed_set_F_R += DRIFT_SPEED_S * motor_dir;
        motor.speed_set_B_L += DRIFT_SPEED_S * motor_dir;
        motor.speed_set_B_R -= DRIFT_SPEED_S * motor_dir;
      }
      else if(avoid_dir == RIGHT)
      {
        motor.speed_set_F_L += DRIFT_SPEED_S * motor_dir;
        motor.speed_set_F_R -= DRIFT_SPEED_S * motor_dir;
        motor.speed_set_B_L -= DRIFT_SPEED_S * motor_dir;
        motor.speed_set_B_R += DRIFT_SPEED_S * motor_dir;
      }
    };break;
    case TRANSLATE_D:
    {
      if(avoid_dir == LEFT)
      {
        motor.speed_set_F_L = -DRIFT_SPEED_D * motor_dir;
        motor.speed_set_F_R = DRIFT_SPEED_D * motor_dir;
        motor.speed_set_B_L = DRIFT_SPEED_D * motor_dir;
        motor.speed_set_B_R = -DRIFT_SPEED_D * motor_dir;
      }
      else if(avoid_dir == RIGHT)
      {
        motor.speed_set_F_L = DRIFT_SPEED_D * motor_dir;
        motor.speed_set_F_R = -DRIFT_SPEED_D * motor_dir;
        motor.speed_set_B_L = -DRIFT_SPEED_D * motor_dir;
        motor.speed_set_B_R = DRIFT_SPEED_D * motor_dir;
      }
    };break;
//    case TANGENT:
//    {
//      if(avoid_dir == LEFT)
//      {
//        motor.speed_set_F_L -= tangent_speed * motor_dir;
//        motor.speed_set_F_R += tangent_speed * motor_dir;
//        motor.speed_set_B_L += tangent_speed * motor_dir;
//        motor.speed_set_B_R -= tangent_speed * motor_dir;
//      }
//      else if(avoid_dir == RIGHT)
//      {
//        motor.speed_set_F_L += tangent_speed * motor_dir;
//        motor.speed_set_F_R -= tangent_speed * motor_dir;
//        motor.speed_set_B_L -= tangent_speed * motor_dir;
//        motor.speed_set_B_R += tangent_speed * motor_dir;
//      }
//    };break;
    
  }
}

void avoid_obstacle()
{
  avoid_check();
  if(avoid_flag)
  {
    avoiding();
    avoid_flag = 0; 
  }
}