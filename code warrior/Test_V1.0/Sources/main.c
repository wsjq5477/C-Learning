/******************************************************************************************
    文件名称：Test_V1.0.c
    
    项目名称：Test_V1.0.mcp
    
    作者：    季齐
    
    版本：    V1.0
    
    说明：    精确定时一秒,AD双通道连续采样后在数码管上显示
    
    创建日期：2016-9-26
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "SCI.h"
#include "PIT.h"
#include "CRG.h"

/**********************************宏定义*************************************************/
#define  SEG_DD    DDRA                                               //段码口方向
#define  DIG_DD    DDRT                                               //位码口方向
#define  SEG_NUM   PORTA                                              //段码数据
#define  DIG_NUM   PTT                                                //位码数据

/***********************************函数声明**********************************************/
void Delay(unsigned int loop_times);                                  //延时子程序

/**********************************全局变量声明*******************************************/
const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //共阴数码管字形表定义                                                
unsigned char g_pitflag = 0;                    
unsigned int convert_value0;
unsigned int convert_value1;
unsigned int SM_SBUFF[8];    

                                        
/**********************************主函数*************************************************/
void main(void)
{       
     unsigned char led_dig;                                          //数码管位码
     unsigned char i;                                                //循环变量
  
     EnableInterrupts;
                                                                         
     SEG_DD = 0xFF;                                                  //A端口设置为输出口
     DIG_DD = 0xFF;                                                  //H端口设置为输出口
     SEG_NUM = 0x00;                                                 //A端口输出全0
     DIG_NUM = 0x00;                                                 //H端口输出全0

     SetBusCLK_32M();                                                //总线频率32M
     AD_Init();                                                      //AD模块初始化
     PIT_init();                                                     //PIT模块初始化
     SCI0_Init();                                                    //SCI初始化

     Delay(20);                                                          
        
     for(;;)
     {  
         led_dig = 0x01; 
         for(i=0;i<8;i++) 
         {
             SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]];
             DIG_NUM = led_dig;
             led_dig = led_dig<<1;
             Delay(1);
         }   
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
                     
  	PITTF_PTF0 = 1;                                                 //清零标志位
  	
  	g_pitflag++;                                                    //g_second_flag加1
  	
  	if(g_pitflag == 10)                                             //每次0.1秒，进入10次中断说明1s时间到
  	{   
  	    g_pitflag = 0;                                              //每到1秒，将标志位清零  	                                                          //g_second_flag   
        
        while(!ATD0STAT2L_CCF0)                                     //等待ATD0转换完成
        {
          ; 
        }                                                                         
        convert_value0 = ATD0DR0;                                   //读取ATD0转换结果
        SM_SBUFF[3] =  convert_value0/1000;                         //将结果拆分并储存
        SM_SBUFF[2] =  convert_value0/100%10;                                 
        SM_SBUFF[1] =  convert_value0/10%10;
        SM_SBUFF[0] =  convert_value0%10; 
         
        while(!ATD0STAT2L_CCF1)                                     //等待ATD1转换完成
        {
          ;
        }              
        convert_value1 = ATD0DR1;                                   //读取转换结果
        SM_SBUFF[7] =  convert_value1/1000;                         //将结果拆分并储存
        SM_SBUFF[6] =  convert_value1/100%10;                                 
        SM_SBUFF[5] =  convert_value1/10%10;
        SM_SBUFF[4] =  convert_value1%10;               
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
    unsigned char j;
    unsigned char Rev_data;    
    unsigned char str[]={ 'E','R','R','O','R' };      
	  
	  if (SCI0SR1_RDRF) 
     {
         Rev_data = SCI0DRL;                                        //接收数据                             
         if(Rev_data == 'A')                                        //若接收A，则发送通道0和通道1的数据
         { 
             send_data_ch0(convert_value0);   
                     
             send_data_ch1(convert_value1);                      	 
         } 
         else 
         {
             for(j=0;j<5;j++) 
             {          
                uart_putchar(str[j]);                               //若接收的不是A,则发送ERROR             
                while(!SCI0SR1_TC)                                  //等待发送完成
                {                                           
                  ;
                }                                                                                 
             }                 
             SCI0DRL = 0x0D;                                        //回车
             while(!SCI0SR1_TC)                                     //等待发送完成
             {            
                ;
             }            
         }
     } 	
}
#pragma CODE_SEG DEFAULT

/****************************************
函数：void delay(void)
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
