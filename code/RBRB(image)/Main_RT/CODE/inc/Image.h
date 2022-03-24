#ifndef   _IMAGE_H_
#define   _IMAGE_H_

#include "common.h"

#define VCAN_PORT               USART_2
#define VCAN_BAUD               115200

#define image_H 120
#define image_W 160

typedef enum
{
  NearBeacon,
  FarBeacon
}BeaconDis_e;
extern  BeaconDis_e   beacon_dis;

extern int16 BeaconWidth;

extern int LabTable[1024];
extern int GamaTable1[32];
extern int GamaTable2[64];

extern int L[image_H][image_W];
extern int A[image_H][image_W];
extern int B[image_H][image_W];
extern uint8 image_BW[image_H][image_W];


void RGB2LAB();
void CreateTable();
void LAB2BW();
void image_handle();
void vcan_sendware(void *wareaddr, uint32 waresize);

extern void RGB2LAB();
extern void CreateTable();
extern void LAB2BW();
extern void image_handle();
extern void vcan_sendware(void *wareaddr, uint32 waresize);

#endif
