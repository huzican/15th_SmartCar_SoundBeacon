#include "Image.h"
#include "common.h"
#include "SEEKFREE_SCC8660_CSI.h"



int16 BeaconWidth = 0;
BeaconDis_e beacon_dis = NearBeacon;

int LabTable[1024];
int GamaTable1[32];
int GamaTable2[64];

int L[image_H][image_W];
int A[image_H][image_W];
int B[image_H][image_W];
uint8 image_BW[image_H][image_W];

const int NearThreshold[6]={80, 100, -22, 11, -10, 100};
const int FarThreshold[6] = {13, 60, 29, 80, -128, 127};

void CreateTable()
{
  for(int i = 0; i < 1024; i++)
  {
    if(i > 9)
      LabTable[i]=(int)(pow((float)i/1024,1.0F/3.0F)*1024);
    else
      LabTable[i]=(int)(7.787F * i + 141.2);
  }
  
  for(int j = 0; j <32; j++)
  {
    float x = j/32.0F;
    x = x>0.04045 ? pow((x+0.055f)/1.055f,2.4f):x/12.92;
    GamaTable1[j] = (int)(x*1024);
  }
  
  for(int k = 0; k < 64; k++)
  {
    float y = k/64.0F;
    y = y > 0.04045 ? pow((y+0.055F)/1.055F,2.4F):y/12.92F;
    GamaTable2[k] = (int)(y*1024);
  }
}

void RGB2LAB()
{
  int r=0,g=0,b=0,x=0,y=0,z=0;
  int i,j;
  for(i = 0; i < image_H; i++)
  {
    for(j = 0; j < image_W; j++)
    {
      r=(scc8660_csi_image[i][j]&0Xf800)>>11;
      g=(scc8660_csi_image[i][j]&0X07e0)>>5;
      b=(scc8660_csi_image[i][j]&0X001f);
      
      x=(455026*GamaTable1[r]+394489*GamaTable2[g]+GamaTable1[b])>>20;
      y=(223002*GamaTable1[r]+749900*GamaTable2[g]+75675*GamaTable1[b])>>20;
      z=(18619*GamaTable1[r]+114786*GamaTable2[g]+915097*GamaTable1[b])>>20;
      
      L[i][j]=y>9 ? (116*LabTable[y]-16384)>>10:(903*LabTable[y])>>10;
      A[i][j]=(500*(LabTable[x]-LabTable[y]))>>10;
      B[i][j]=(200*(LabTable[y]-LabTable[z]))>>10;
    }
  }
}

void LAB2BW()
{
  int i,j;
  if(beacon_dis == FarBeacon)
  {
    for(i = 0; i < image_H; i++)
    {
      for(j = 0; j < image_W; j++)
      {
        if(
           (L[i][j]>FarThreshold[0]&&L[i][j]<FarThreshold[1])
            &&(A[i][j]>FarThreshold[2]&&A[i][j]<FarThreshold[3])
            &&(B[i][j]>FarThreshold[4]&&B[i][j]<FarThreshold[5])
          )
        {
          image_BW[i][j]=255;
        }
        else
          image_BW[i][j]=0;
      }
    }
  }
  else if(beacon_dis == NearBeacon)
  {
    for(i = 0; i < image_H; i++)
    {
      for(j = 0; j < image_W; j++)
      {
        if(
           (L[i][j]>NearThreshold[0]&&L[i][j]<NearThreshold[1])
            &&(A[i][j]>NearThreshold[2]&&A[i][j]<NearThreshold[3])
            &&(B[i][j]>NearThreshold[4]&&B[i][j]<NearThreshold[5])
          )
        {
          image_BW[i][j]=255;
        }
        else
          image_BW[i][j]=0;
      }
    }
  }
}

void image_handle()
{
  RGB2LAB();
  LAB2BW();
}


void vcan_sendware(void *wareaddr, uint32 waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(VCAN_PORT, (uint8 *)wareaddr, waresize);    //发送数据
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //发送后命令
}