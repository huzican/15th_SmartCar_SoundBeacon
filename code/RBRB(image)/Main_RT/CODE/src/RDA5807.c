//#include "SEEKFREE_IIC.h"
//#include "zf_systick.h"
//#include "SEEKFREE_RDA5807.h"
//
//
//#define ack 1      //主应答
//#define no_ack 0   //从应答
//
//
//#define FM_ADDRESS	0x20>>1
//
//uint8 rda5807_config_reg[] = {
//    0xc1, 0x03,                             // Register 0x2
//    0x00, 0x00,                             // Register 0x3
//    0x0a, 0x00,        	                    // Register 0x4
//    0x88, 0x0f,                             // Register 0x5
//    0x00, 0x00,                             // Register 0x6
//    0x42, 0x02,                             // Register 0x7
//};
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807模拟IIC读取多字节数据
////  @param      dev_add			设备地址(低七位地址)
////  @param      data[]			数据保存的地址指针
////  @param      num				读取字节数量
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rad5807m_simiic_write(uint8 dev_add, uint8 data[], uint8 num)
//{
//   	uint8 i;
//	simiic_start();
//    send_ch( (dev_add<<1) | 0x00);   		//发送器件地址加写位
//
//	for(i=0;i<num;i++)
//		send_ch(data[i]);   				 //发送需要写入的数据
//
//	simiic_stop();
//}
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      模拟IIC读取多字节数据
////  @param      dev_add			设备地址(低七位地址)
////  @param      dat_add			数据保存的地址指针
////  @param      num				读取字节数量
////  @return     uint8			返回寄存器的数据
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807m_simiic_read(uint8 dev_add,uint8 *dat_add,uint8 num)
//{
//    simiic_start();
//
//	send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
//    while(--num)
//    {
//        *dat_add = read_ch(ack); //读取数据
//        dat_add++;
//
//    }
//    *dat_add = read_ch(no_ack); //读取数据
//
//	simiic_stop();
//}
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      频率寄存器配置（内部使用，用户无需关心）
////  @param      channel         	频率寄存器值
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_channel_config(uint16 reg_chn)
//{
//	rda5807_config_reg[0] = 0xc0;
//	rda5807_config_reg[1] = 0x01;
//	rda5807_config_reg[2] = (uint8)(reg_chn >> 2);
//	rda5807_config_reg[3] = (uint8)(((reg_chn & 0x3) << 6) | 0x18);
//}
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807设置频率	（内部使用，用户无需关心）
////  @param      freq         频率值
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_set_channel(float freq)
//{
//	uint16 reg_chn=0;
//	reg_chn = (int)((freq - 76.0) * 10.0 + 0.5);
//    rda5807_channel_config(reg_chn);
//    rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
//}
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807设置为空闲模式
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_set_idle(void)
//{
//	rda5807_config_reg[1] &= ~(1<<0);//Power Up Disabled.
//	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
//}
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807设置为工作模式
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_set_transimt(void)
//{
//	rda5807_config_reg[1] |= 1<<0;//Power Up Enable.
//	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
//}
//
//
//
//
//
//
//uint8  rda5807_read_reg[10];
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807读取ID（内部使用，用户无需关心）
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_read_id(void)
//{
//	rda5807m_simiic_read(FM_ADDRESS,rda5807_read_reg,10);
//}
//
//
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      rda5807初始化
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void rda5807_init(float freq)
//{
//	uint8 dat[2] = {0x00,0x02};
//
//
//	//iic初始化
//	simiic_init();
//	simiic_delay_set(100);//设置模拟IIC延时，因为默认速率较高
//	//复位
//	rad5807m_simiic_write(FM_ADDRESS,dat,2);
//
//	systick_delay_ms(600);
//
//	while(rda5807_read_reg[8] != 0x58)
//	{
//		//读取寄存器，自检。
//		rda5807m_simiic_read(FM_ADDRESS,rda5807_read_reg,10);
//		systick_delay_ms(50);
//		//如果一直卡在这里，查看SCL和SDA是否连接好。
//		//需要设置SEEKFREE_IIC.h文件中的SIMIIC_DELAY_TIME值大于60
//	}
//
//	//启动
//	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg,sizeof(rda5807_config_reg));
//	//频率设置
//	rda5807_set_channel(freq);
//
//	simiic_delay_set(SIMIIC_DELAY_TIME);//还原模拟IIC默认速率
//}
//
//
//
//
