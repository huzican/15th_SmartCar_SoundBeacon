#include "CH455.h"
#include "zf_iic.h"
#include "zf_systick.h"
#include "SEEKFREE_IIC.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"

#define SDAPin    C20
#define SCLPin    C18


#define high 	1
#define low 	0

#define		CH455_SCLPin_high		{gpio_set (SCLPin, 1);}
#define		CH455_SCLPin_low		{gpio_set (SCLPin, 0);}
#define		CH455_SCLPin_D_OUT	        {gpio_init (SCLPin, GPO,1,GPIO_PIN_CONFIG | OD_EN);}	 //  设置SCLPin为输出方向,对于双向I/O需切换为输出
#define		CH455_SDAPin_high		{gpio_set (SDAPin, 1);}
#define		CH455_SDAPin_low		{gpio_set (SDAPin, 0);}
#define		CH455_SDAPin_IN		         gpio_get(SDAPin)                    //  读取SDAPin输入电平
#define		CH455_SDAPin_D_OUT	        {gpio_init (SDAPin, GPO,1,GPIO_PIN_CONFIG | OD_EN);}   // 设置SDAPin为输出方向,对于双向I/O需切换为输出
#define		CH455_SDAPin_D_IN		{gpio_init (SDAPin, GPI,1,GPIO_PIN_CONFIG | OD_EN);}   // 设置SDAPin为输入方向,对于双向I/O需切换为输入


void CH455_I2c_Start()
{
    CH455_SDAPin_D_OUT;
    CH455_SCLPin_high;  
    DelayUs(I2C_DELAY);
    CH455_SDAPin_high;  
    DelayUs(I2C_DELAY);
    CH455_SDAPin_low;   
    DelayUs(I2C_DELAY);
    CH455_SCLPin_low;   
    DelayUs(I2C_DELAY);
}
void CH455_I2c_Stop()
{
    CH455_SDAPin_D_OUT;
    CH455_SCLPin_low;    
    DelayUs(I2C_DELAY);
    CH455_SDAPin_low;     
    DelayUs(I2C_DELAY);
    CH455_SCLPin_high;   
    DelayUs(I2C_DELAY);
    CH455_SDAPin_high;   
    DelayUs(I2C_DELAY);
}
void CH455_I2c_WrByte(unsigned char IIC_Byte) //IIC发送一个字节
{
    unsigned char i;
    CH455_SDAPin_D_OUT;
    for(i = 0; i < 8; i++)
    {
        if(IIC_Byte & 0x80) CH455_SDAPin_high   //SDAPin=high;
        else  CH455_SDAPin_low 
        DelayUs(I2C_DELAY);
        CH455_SCLPin_high;      
        DelayUs(I2C_DELAY);
        CH455_SCLPin_low;      
        DelayUs(I2C_DELAY);
        IIC_Byte<<=1;
    }
    DelayUs(I2C_DELAY);
    CH455_SDAPin_high;          
    DelayUs(I2C_DELAY);
    CH455_SCLPin_high;         
    DelayUs(I2C_DELAY);
    CH455_SCLPin_low;          
    DelayUs(I2C_DELAY);
}
 
unsigned char  CH455_I2c_RdByte( void )		//读一个字节数据
{    
    unsigned char i, bytedata;
    CH455_SDAPin_high;      //数据线拉高
    DelayUs(I2C_DELAY);
    CH455_SDAPin_D_IN;     //将数据设置为输入模式
    DelayUs(I2C_DELAY);
    CH455_SCLPin_low;
    DelayUs(I2C_DELAY);
    for(i = 0; i < 8; i++)      //读8位数据
    {
        CH455_SCLPin_high;
        DelayUs(I2C_DELAY);
        bytedata <<= 1;
        DelayUs(I2C_DELAY);
        bytedata |= (CH455_SDAPin_IN);
        DelayUs(I2C_DELAY);
        CH455_SCLPin_low;
        DelayUs(I2C_DELAY);
    }
    CH455_SDAPin_D_OUT;            //数据线设置回输出模式
    CH455_SDAPin_high;        
    DelayUs(I2C_DELAY);
    CH455_SCLPin_high;          
    DelayUs(I2C_DELAY);
    CH455_SCLPin_low;           
    DelayUs(I2C_DELAY);
    return(bytedata);//返回数据
}
 
void CH455_Write( uint16 cmd )	//写命令
{
	CH455_I2c_Start();               //启动总线
   	CH455_I2c_WrByte(((uint8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
   	CH455_I2c_WrByte((uint8)cmd);     //发送数据
  	CH455_I2c_Stop();                 //结束总线 
}
 
uint8 CH455_Read( void )		//读取按键
{
	uint8 keycode;
   	CH455_I2c_Start();                //启动总线
   	CH455_I2c_WrByte((uint8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);//获取按键
   	keycode=CH455_I2c_RdByte();      //读取数据
	CH455_I2c_Stop();                //结束总线
	return keycode;
}
void CH455_Init(void)
{
    CH455_SCLPin_D_OUT;
    CH455_SDAPin_D_OUT;
    CH455_Write( CH455_SYSOFF ); 
    CH455_Write( CH455_SYSON );  //重新启动信号
    CH455_SDAPin_high;
}