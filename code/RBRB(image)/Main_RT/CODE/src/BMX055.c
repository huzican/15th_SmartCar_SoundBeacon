#include "BMX055.h"
#include "SEEKFREE_IIC.h"
#include "zf_systick.h"

bool BMX055_init(void)
{
  uint8 ErrCount;
  
/************************************/
/*���ٶ�����*/
/************************************/
  ErrCount = 0;
  while(simiic_read_reg(IIC_BMX055_ACC_ADR, BMX055_ACC_ID,SIMIIC) != 0xFA)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }
  simiic_write_reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMURANGE, 0x05);//8G      3 2G  5 4G  8 8G  
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMUBW, 0x0F);//1000HZ 
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMULPM, 0x00);//Normal MODE 
  
/************************************/
/*����������*/
/************************************/  
  ErrCount = 0;
  while(simiic_read_reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_ID,SIMIIC) != 0x0F)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }
  simiic_write_reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RANGE, 0x01);//+-1000   
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_BW, 0x02);//1000HZ  
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_LPM, 0x00);//Normal MODE  
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RATEHBW, 0x08);//��ͨ�˲� �����Բ�Ҫ   
  systick_delay_ms(10);
  
/************************************/
/*����������*/
/************************************/  
  ErrCount = 0;
  simiic_write_reg(IIC_BMX055_MAG_ADR, BMX055_MAG_POM, 0x81);
  systick_delay_ms(10);
  while(simiic_read_reg(IIC_BMX055_MAG_ADR, BMX055_MAG_ID,SIMIIC) != 0x32)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }  
  simiic_write_reg(IIC_BMX055_MAG_ADR, BMX055_MAG_DATARATE, 0x38);//�������30HZ
  systick_delay_ms(10);
  simiic_write_reg(IIC_BMX055_MAG_ADR, BMX055_MAG_INTEN, 0x00);//��ʹ���ж�
  systick_delay_ms(10);

  return true;
}


////�����ȡ������false
void BMX055_DataRead(BMX055_t *Q, uint8 type)//type    0����     1ֻ��������
{
  uint8 datatemp[6] = {0};

  simiic_read_regs(IIC_BMX055_GYRO_ADR, BMX055_GYRO_XDATALSB, datatemp, 6,SIMIIC);
  Q->GYROXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]));
  Q->GYROYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]));
  Q->GYROZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]));            //ֻ��Z��
  if(type)     return;//ֻ��������
  simiic_read_regs(IIC_BMX055_ACC_ADR, BMX055_ACC_XDATALSB, datatemp, 6,SIMIIC);
  Q->ACCXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 4);
  Q->ACCYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 4);
  Q->ACCZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 4);
}


void BMX055_read_mag_back(float *m)//���������ݶ�ȡ
{
  uint8 datatemp[6] = {0};
  simiic_read_regs(IIC_BMX055_MAG_ADR, BMX055_MAG_XDATALSB, datatemp, 6,SIMIIC);
  m[0] = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 3);       //X
  m[1] = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 3);       //Y
  m[2] = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 1);       //Z
}


void BMX055_read_back(float *g, float *a, char flag[])
{
  BMX055_t bmx055;
  
  if(!strcmp(flag,"SIX"))//����
  {
    BMX055_DataRead(&bmx055, 0);
  
    g[0] = bmx055.GYROXdata;
    g[1] = bmx055.GYROYdata;
    g[2] = bmx055.GYROZdata;
  
    a[0] = bmx055.ACCXdata;
    a[1] = bmx055.ACCYdata;
    a[2] = bmx055.ACCZdata;
  }
  else if(!strcmp(flag,"GZ"))//ֻ��Z��
  {
    BMX055_DataRead(&bmx055, 2);
    
    g[2] = bmx055.GYROZdata;
  }
  else if(!strcmp(flag,"OFFSET"))//��ƫ����  
  {
    BMX055_DataRead(&bmx055, 0);
    static float x=0,y=0,z=0;
    static uint8 i = 0;
    x += g[0];
    y += g[1];
    z += g[2];
    i++;
    if( i == 10)
    {
//      var[0] = x/10;
//      var[1] = y/10;
//      var[2] = z/10;
      x = 0;
      y = 0;
      z = 0;
      i = 0;
    }
  }
}