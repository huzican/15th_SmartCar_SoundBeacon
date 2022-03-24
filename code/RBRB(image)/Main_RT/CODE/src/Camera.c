#include "Camera.h"
#include "SEEKFREE_SCC8660_CSI.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_18TFT.h"
#include "Image.h"


ImgShow_e  img_show = tft;
ImgType_e img_type = BW;

void Show_init()
{
  if(img_show == ips200)
    ips200_init();
  else if(img_show == ips114)
    ips114_init();
  else if(img_show == tft)
  {
    lcd_init();
    lcd_clear(BLUE);
  }
}

void img_display()
{
  if(img_show == ips200)
  {
    if(img_type == RGB)
      ips200_displayimage8660_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 160, 120);
    else if(img_type == BW)
      ips200_displayimage032(image_BW[0], image_W, image_H);
  }
  else if(img_show == ips114)
  {
    if(img_type == RGB)
      ips114_displayimage8660_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 160, 120);
    else if(img_type == BW)
      ips114_displayimage032(image_BW[0], image_W, image_H);

  }
  else if(img_show == tft)
  {
    if(img_type == RGB)
      lcd_displayimage8660_zoom(scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H,160,120);
    else if(img_type == BW)
      lcd_displayimage7725(image_BW[0], image_W, image_H);
  }
}