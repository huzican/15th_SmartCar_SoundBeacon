/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MPU6050
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
					------------------------------------ 
						���IIC
                        SCL                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
						SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
                            
                        Ӳ��IIC
                        SCL                 �鿴init_mpu6050_hardware������IIC��ʼ��ʱ��������
						SDA                 �鿴init_mpu6050_hardware������IIC��ʼ��ʱ��������    
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

/***************MPUģ��IIC***************/
#define MPU6050_SDA             gpio_get (MPU6050_SDA_PIN)
#define MPU6050_SDA0()          gpio_set (MPU6050_SDA_PIN, 0)		//IO������͵�ƽ
#define MPU6050_SDA1()          gpio_set (MPU6050_SDA_PIN, 1)		//IO������ߵ�ƽ  
#define MPU6050_SCL0()          gpio_set (MPU6050_SCL_PIN, 0)		//IO������͵�ƽ
#define MPU6050_SCL1()          gpio_set (MPU6050_SCL_PIN, 1)		//IO������ߵ�ƽ
#define MPU6050_DIR_OUT()       gpio_dir (MPU6050_SDA_PIN, GPO)    //�������
#define MPU6050_DIR_IN()        gpio_dir (MPU6050_SDA_PIN, GPI)    //���뷽��

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
    mpu6050_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	mpu6050_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)mpu6050_iic_stop();
	
	mpu6050_iic_start();
	mpu6050_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = mpu6050_read_ch(no_ack);   				//��ȡ����
	mpu6050_iic_stop();
	
	return dat;
}
void mpu6050_iic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, IIC_type type)
{
	mpu6050_iic_start();
    mpu6050_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	mpu6050_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)mpu6050_iic_stop();
	
	mpu6050_iic_start();
	mpu6050_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = mpu6050_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = mpu6050_read_ch(no_ack); //��ȡ����
	mpu6050_iic_stop();
}
void mpu6050_iic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	mpu6050_iic_start();
    mpu6050_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	mpu6050_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	mpu6050_send_ch( dat );   				 //������Ҫд�������
	mpu6050_iic_stop();
}

void mpu6050_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	MPU6050_SDA1();//SDA �������
        else			MPU6050_SDA0();
        c <<= 1;
        mpu6050_iic_delay();
        MPU6050_SCL1();                //SCL ���ߣ��ɼ��ź�
        mpu6050_iic_delay();
        MPU6050_SCL0();                //SCL ʱ��������
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
    MPU6050_DIR_IN();           //��������Ϊ���뷽ʽ
    for(i=0;i<8;i++)
    {
        mpu6050_iic_delay();
        MPU6050_SCL0();         //��ʱ����Ϊ�ͣ�׼����������λ
        mpu6050_iic_delay();
        MPU6050_SCL1();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        mpu6050_iic_delay();
        c<<=1;
        if(MPU6050_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
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
	
    if(MPU6050_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
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
//  @brief      MPU6050�Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self1_check(void)
{
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    while(0x07 != mpu6050_iic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV,SIMIIC))
    {
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
		//4 ����û�е���ģ��IIC�ĳ�ʼ������
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init(void)
{
    mpu6050_iic_init();
    systick_delay_ms(100);                                   //�ϵ���ʱ

    mpu6050_self1_check();
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);       //
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
	mpu6050_iic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    mpu6050_iic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ȡMPU6050����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  ���Ϻ�����ʹ�����IICͨ�ţ���Ƚ�Ӳ��IIC�����IIC���Ÿ���������ʹ��������ͨIO
//-------------------------------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------------------------------
//  ���º�����ʹ��Ӳ��IICͨ�ţ���Ƚ����IIC��Ӳ��IIC�ٶȿ����������졣
//-------------------------------------------------------------------------------------------------------------------

#define IIC_NUM         IIC_1
#define IIC_SDA_PIN     IIC1_SDA_B17
#define IIC_SCL_PIN     IIC1_SCL_B16
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MPU6050�Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self2_check(void)
{
    uint8 dat=0;
    
    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07); //125HZ������
    while(0x07 != dat)
    {
        iic_read_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, &dat);
        systick_delay_ms(10);
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init_hardware(void)
{
    systick_delay_ms(100);                                      //�ϵ���ʱ
    iic_init(IIC_NUM, IIC_SDA_PIN, IIC_SCL_PIN,400*1000);       //Ӳ��IIC��ʼ��     ������400K
    
    mpu6050_self2_check();
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);    //125HZ������
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);        //
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);   //2000
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);  //8g
	iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, User_Control, 0x00);
    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ȡMPU6050����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_gyro_hardware(void)
{
    uint8 dat[6];

    iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6);  
    mpu_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}








