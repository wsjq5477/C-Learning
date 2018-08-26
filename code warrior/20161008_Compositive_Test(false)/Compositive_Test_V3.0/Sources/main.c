/******************************************************************************************
    文件名称：main.c
    
    项目名称：Compositive_Test_V2.0.mpc
    
    作者：    季齐
    
    版本：    V1.0
    
    说明：    精确定时一秒,AD双通道连续采样后在数码管上显示,当接收到上位机的字符时发送AD数据
    
    创建日期：2016-10-6
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "SCI.h"
#include "PIT.h"
#include "CRG.h"

/****************************************宏定义*******************************************/
#define  SEG_DD    DDRA                                               //段码口方向
#define  DIG_DD    DDRT                                               //位码口方向
#define  SEG_NUM   PORTA                                              //段码数据
#define  DIG_NUM   PTT                                                //位码数据

/***************************************函数声明******************************************/
void system_init(void);
void Num_test(void);
void Num_refresh(void);
void AD_acquisition(void);
void SCI_send(void);
void Delay(unsigned int loop_times);                                  //延时子程序

/*************************************全局变量声明****************************************/

const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //共阴数码管字形表定义           
unsigned int SM_SBUFF[8];
float convert_value0;
float convert_value1;                                                                                              
unsigned char g_pitflag = 0;
unsigned char g_sciflag = 0; 
    
                                        
/****************************************主函数*******************************************/

void main(void) 
{       
    system_init();
    
         
    for(;;)
    {  
        if(g_pitflag==1)
        {
            AD_acquisition();
            g_pitflag = 0;
        }
        
        if(g_sciflag==1)
        {
            SCI_send();
            g_sciflag = 0; 
        }
        
        Num_refresh();   
    }
}

/***************************************************************************************
    函数名称：   interrupt VectorNumber_Vsci0 void PIT_ISR(void)
    函数功能：   PIT通道0中断服务程序
    调用函数：   无
    入口参数：   无
    出口参数：   无
    返回值：     无
    其他：  	
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{          
    static unsigned char i = 1;
    
    PITTF_PTF0 = 1;
             
  	i++;                                                     
  	
  	if(i == 10)                                             //每次0.1秒，进入10次中断说明1s时间到
  	{   
  	    i = 0;                                              //每到1秒，将标志位清零  	                                                                             
        g_pitflag = 1;
    }                         
}
#pragma CODE_SEG DEFAULT

/*******************************************************************************************
    函数名称：   interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
    函数功能：   接收中断使能
    调用函数：   无
    入口参数：   无
    出口参数：   无
    返回值：     无
    其他：  	
*******************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
    if(SCI0SR1_RDRF)
	  {
		    g_sciflag = 1;  
   	}
      	
}
#pragma CODE_SEG DEFAULT


/****************************************
函数：void system_init(void)
功能：系统初始化
说明：配置GPIO并将模块初始化
****************************************/
void system_init(void)
{                                                                        
    SEG_DD = 0xFF;                                                  //A端口设置为输出口
    DIG_DD = 0xFF;                                                  //H端口设置为输出口
    SEG_NUM = 0x00;                                                 //A端口输出全0
    DIG_NUM = 0x00;                                                 //H端口输出全0

    SetBusCLK_64M();                                                //总线频率64M
    AD_Init();                                                      //AD模块初始化
    PIT_init();                                                     //PIT模块初始化
    SCI0_Init();                                                    //SCI初始化
    
    Num_test();                                                     //数码管显示测试
    
    EnableInterrupts;                                               //中断使能
}

/****************************************
函数：void delay(unsigned int loop_times)
功能：延时
说明：延时500*loop_times个机器周期
****************************************/
void Delay(unsigned int loop_times)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i<loop_times; loop_i++) 
    {
        for (loop_j=0; loop_j<4000;loop_j++) 
        {
            ;
        }
    }
}

/****************************************
函数：void Num_test(void)
功能：测试数码管
说明：令数码管先显示0-7
****************************************/
void Num_test(void)
{  
    unsigned char led_dig;                                         //数码管位码
    unsigned char j;                                                //循环变量     
    
    led_dig = 0x01; 
    
    for(j=0;j<8;j++) 
    {
       SEG_NUM = g_LED_Seg_Table[j];
       DIG_NUM = led_dig;
       led_dig = led_dig<<1;
       Delay(300);
    }  
}

/****************************************
函数：void Num_refresh(int SM_SBUFF2)
功能：刷新数码管
说明：不断刷新8位数码管
****************************************/
void Num_refresh(void)
{  
    unsigned char led_dig;                                          //数码管位码
    unsigned char i;                                                //循环变量     
    
    led_dig = 0x01; 
    for(i=0;i<8;i++) 
    {
        if(i==3||i==7)
        { 
            SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]]|0x80;               \
            
        }
        else
        {
            SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]];
        }
        DIG_NUM = led_dig;
        led_dig = led_dig<<1;
        Delay(1);
    }  
}

/****************************************
函数：void AD_acquisition(void)
功能：AD采集
说明：采集两路AD数据并拆分
****************************************/
void AD_acquisition(void)
{
    float temp0;
    float temp1;   
    
    ATD0CTL5 = 0x10;                                            //多通道单次采样，通道0，1  
    
    while(!ATD0STAT2L_CCF0)                                     //等待ATD0转换完成
    {
      ; 
    }                                                                         
    convert_value0 = ATD0DR0;                                   //读取ATD0转换结果
    temp0 = convert_value0*4600/4096;
    
    SM_SBUFF[3] =  (unsigned int)(temp0/1000);                         //将结果拆分并储存
    SM_SBUFF[2] =  (unsigned int)(temp0/100%10);                                 
    SM_SBUFF[1] =  (unsigned int)(temp0/10%10);
    SM_SBUFF[0] =  (unsigned int)(temp0%10); 
         
    while(!ATD0STAT2L_CCF1)                                     //等待ATD1转换完成
    {
      ;
    }              
    convert_value1 = ATD0DR1;                                   //读取ATD0转换结果
    temp1 = convert_value1*4600/4096;
    
    SM_SBUFF[7] =  (unsigned int)(temp1/1000);                         //将结果拆分并储存
    SM_SBUFF[6] =  (unsigned int)(temp1/100%10);                                 
    SM_SBUFF[5] =  (unsigned int)(temp1/10%10);
    SM_SBUFF[4] =  (unsigned int)(temp1%10);    
}

/****************************************
函数：void SCI_send(void)
功能：发送数据
说明：若接收到数据则将AD数据发送至上位机
****************************************/
void SCI_send(void)
{
    unsigned char Rev_data;      
	  
	  if (SCI0SR1_RDRF) 
     {
         Rev_data = SCI0DRL;                                        //接收数据      
                                
         if(Rev_data == 'A')                                        //若接收A，则发送通道0和通道1的数据
         { 
             SCI_senddata0(convert_value0);   
                     
             SCI_senddata1(convert_value1);                      	 
         } 
     }
}
