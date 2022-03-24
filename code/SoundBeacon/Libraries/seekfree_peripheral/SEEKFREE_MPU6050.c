/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MPU6050
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					接线定义：
					------------------------------------ 
						软件IIC
                        SCL                 查看SEEKFREE_IIC文件内的SEEKFREE_SCL宏定义
						SDA                 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
                            
                        硬件IIC
                        SCL                 查看init_mpu6050_hardware函数内IIC初始化时所带参数
						SDA                 查看init_mpu6050_hardware函数内IIC初始化时所带参数    
					------------------------------------ 
 ********************************************************************************************************************/

#include "zf_iic.h"
#include "zf_systick.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"

int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;

/***************MPU模拟IIC***************/
#define MPU6050_SDA             gpio_get (MPU6050_SDA_PIN)
#define MPU6050_SDA0()          gpio_set (MPU6050_SDA_PIN, 0)		//IO口输出低电平
#define MPU6050_SDA1()          gpio_set (MPU6050_SDA_PIN, 1)		//IO口输出高电平  
#define MPU6050_SCL0()          gpio_set (MPU6050_SCL_PIN, 0)		//IO口输出低电平
#define MPU6050_SCL1()          gpio_set (MPU6050_SCL_PIN, 1)		//IO口输出高电平
#define MPU6050_DIR_OUT()       gpio_dir (MPU6050_SDA_PIN, GPO)    //输出方向
#define MPU6050_DIR_IN()        gpio_dir (MPU6050_SDA_PIN, GPI)    //输入方向

#define MPU6050_DELAY_TIME 100

void mpu6050_iic_init()
{
  gpio_init (MPU6050_SCL_PIN, GPO,1,GPIO_PIN_CONFIG | OD_EN);
  gpio_init (MPU6050_SDA_PIN, GPO,1,GPIO_PIN_CONFIG | OD_EN);
}

void mpu6050_iic_delay(void)
{
	uint16 delay_time;
    delay_time = MPU6050_DELAY_TIME;
	while(delay_time--);
}
void mpu6050_iic_start(void)
{
	MPU6050_SDA1();
	MPU6050_SCL1();
	mpu6050_iic_delay();
	MPU6050_SDA0();
	mpu6050_iic_delay();
	MPU6050_SCL0();
}

void mpu6050_iic_stop(void)
{
	MPU6050_SDA0();
	MPU6050_SCL0();
	mpu6050_iic_delay();
	MPU6050_SCL1();
	mpu6050_iic_delay();
	MPU6050_SDA1();
	mpu6050_iic_delay();
	
}
uint8 mpu6050_iic_read_reg(uint8 dev_add, uint8 reg, IIC_type type)
{
	uint8 dat;
	mpu6050_iic_start();
    mpu6050_send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	mpu6050_send_ch( reg );   				//发送从机寄存器地址
	if(type == SCCB)mpu6050_iic_stop();
	
	mpu6050_iic_start();
	mpu6050_send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
	dat = mpu6050_read_ch(no_ack);   				//读取数据
	mpu6050_iic_stop();
	
	return dat;
}
void mpu6050_iic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, IIC_type type)
{
	mpu6050_iic_start();
    mpu6050_send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	mpu6050_send_ch( reg );   				//发送从机寄存器地址
	if(type == SCCB)mpu6050_iic_stop();
	
	mpu6050_iic_start();
	mpu6050_send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
    while(--num)
    {
        *dat_add = mpu6050_read_ch(ack); //读取数据
        dat_add++;
    }
    *dat_add = mpu6050_read_ch(no_ack); //读取数据
	mpu6050_iic_stop();
}
void mpu6050_iic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	mpu6050_iic_start();
    mpu6050_send_ch( (dev_add<<1) | 0x00);   //发送器件地址加写位
	mpu6050_send_ch( reg );   				 //发送从机寄存器地址
	mpu6050_send_ch( dat );   				 //发送需要写入的数据
	mpu6050_iic_stop();
}

void mpu6050_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	MPU6050_SDA1();//SDA 输出数据
        else			MPU6050_SDA0();
        c <<= 1;
        mpu6050_iic_delay();
        MPU6050_SCL1();                //SCL 拉高，采集信号
        mpu6050_iic_delay();
        MPU6050_SCL0();                //SCL 时钟线拉低
    }
	mpu6050_sccb_waitack();
}
void mpu6050_iic_sendack(unsigned char ack_dat)
{
    MPU6050_SCL0();
	mpu6050_iic_delay();
	if(ack_dat) MPU6050_SDA0();
    else    	MPU6050_SDA1();

    MPU6050_SCL1();
    mpu6050_iic_delay();
    MPU6050_SCL0();
    mpu6050_iic_delay();
}
uint8 mpu6050_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    MPU6050_SCL0();
    mpu6050_iic_delay();
    MPU6050_SDA1();             
    MPU6050_DIR_IN();           //置数据线为输入方式
    for(i=0;i<8;i++)
    {
        mpu6050_iic_delay();
        MPU6050_SCL0();         //置时钟线为低，准备接收数据位
        mpu6050_iic_delay();
        MPU6050_SCL1();         //置时钟线为高，使数据线上数据有效
        mpu6050_iic_delay();
        c<<=1;
        if(MPU6050_SDA) 
        {
            c+=1;   //读数据位，将接收的数据存c
        }
    }
    MPU6050_DIR_OUT();
	MPU6050_SCL0();
	mpu6050_iic_delay();
	mpu6050_iic_sendack(ack_x);
    return c;
}
static int mpu6050_sccb_waitack(void)
{
    MPU6050_SCL0();
	MPU6050_DIR_IN();
	mpu6050_iic_delay();
	
	MPU6050_SCL1();
    mpu6050_iic_delay();
	
    if(MPU6050_SDA)           //应答为高电平，异常，通信失败
    {
        MPU6050_DIR_OUT();
        MPU6050_SCL0();
        return 0;
    }
    MPU6050_DIR_OUT();
    MPU6050_SCL0();
	mpu6050_iic_delay();
    return 1;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MPU6050自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self1_check(void)
{
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    while(0x07 != mpu6050_iic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV,SIMIIC))
    {
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
		//4 可能没有调用模拟IIC的初始化函数
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init(void)
{
    mpu6050_iic_init();
    systick_delay_ms(100);                                   //上电延时

    mpu6050_self1_check();
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ采样率
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);       //
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
	mpu6050_iic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_accdata(void)
{
    uint8 dat[6];

    mpu6050_iic_read_regs(MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6, SIMIIC);  
    mpu_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_gyro(void)
{
    uint8 dat[6];

    mpu6050_iic_read_regs(MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6, SIMIIC);  
//    mpu_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
//    mpu_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
    mpu_gyro_z += 16;
}

//-------------------------------------------------------------------------------------------------------------------
//  以上函数是使用软件IIC通信，相比较硬件IIC，软件IIC引脚更加灵活，可以使用任意普通IO
//-------------------------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------------------------
//  以下函数是使用硬件IIC通信，相比较软件IIC，硬件IIC速度可以做到更快。
//-------------------------------------------------------------------------------------------------------------------

#define IIC_NUM         IIC_1
#define IIC_SDA_PIN     IIC1_SDA_B17
#define IIC_SCL_PIN     IIC1_SCL_B16
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MPU6050自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self2_check(void)
{
    uint8 dat=0;
    
    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07); //125HZ采样率
    while(0x07 != dat)
    {
        iic_read_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, &dat);
        systick_delay_ms(10);
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init_hardware(void)
{
    systick_delay_ms(100);                                      //上电延时
    iic_init(IIC_NUM, IIC_SDA_PIN, IIC_SCL_PIN,400*1000);       //硬件IIC初始化     波特率400K
    
    mpu6050_self2_check();
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//解除休眠状态
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);    //125HZ采样率
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);        //
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);   //2000
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);  //8g
	iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, User_Control, 0x00);
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_accdata_hardware(void)
{
    uint8 dat[6];
    
    iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6);
    mpu_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_gyro_hardware(void)
{
    uint8 dat[6];

    iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6);  
    mpu_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}








