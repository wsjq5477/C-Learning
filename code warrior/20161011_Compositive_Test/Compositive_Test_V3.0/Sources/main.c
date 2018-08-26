/******************************************************************************************
    文件名称：main.c
    
    项目名称：Compositive_Test_V3.0.mpc
    
    作者：    季齐
    
    版本：    V1.0
    
    说明：    精确定时一秒,AD双通道连续采样后在数码管上显示,当接收到上位机的字符时发送AD数据
    
    创建日期：2016-10-21
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "SCI.h"
#include "PIT.h"
#include "PWM.h"
#include "CRG.h"

/****************************************宏定义*******************************************/
#define  SEG_DD    DDRA                                               //段码口方向
#define  DIG_DD    DDRT                                               //位码口方向
#define  SEG_NUM   PORTA                                              //段码数据
#define  DIG_NUM   PTT                                                //位码数据
#define  SW_DD     DDRB_DDRB4                                         //拨码开关数据方向寄存器定义
#define  SW        PORTB_PB4                                          //拨码开关控制位I/O口定义

/***************************************函数声明******************************************/
void system_init(void);
void Num_test(void);
void Num_refresh(void);
void AD_acquisition(void);
void Data_split(unsigned int value0,unsigned int value1);
void PWM_output(unsigned int value0,unsigned int value1);
void Filtering(void);
void SCI_send(void);
void Delay(unsigned int loop_times);                                  //延时子程序

/*************************************全局变量声明****************************************/

unsigned char const g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //共阴数码管字形表定义           
extern unsigned char ch0_A[4] = 0;
extern unsigned char ch1_A[4] = 0;

unsigned int convert_value0;
unsigned int convert_value1;
unsigned int filtering_D_value0 = 0;
unsigned int filtering_D_value1 = 0;
unsigned int filtering_A_value0 = 0;
unsigned int filtering_A_value1 = 0;
    
unsigned int value0_buf[10];
unsigned int value1_buf[10];

unsigned char ch0_D[4] = 0;  
unsigned char ch1_D[4] = 0; 
                                                                                          
unsigned char g_pitflag = 0;
unsigned char g_sciflag = 0;
unsigned char g_keyflag = 0;
unsigned char Rev_data; 

/****************************************主函数*******************************************/

void main(void) 
{       
    unsigned char sw;
    
    system_init();
             
    for(;;)
    {  
        sw = SW;                       
        if(sw==0)                       //判断拨码开关状态
        {
            g_keyflag = 0;
        }
        else
        {
            g_keyflag = 1;
        }
        
        if(g_pitflag==1)                //PIT标志位为1则进行AD采样
        {
            AD_acquisition();
            g_pitflag = 0;
        }
        
        if(g_sciflag==1)                //SCI标志位为1则进行数据发送
        {
            SCI_send();
            g_sciflag = 0; 
        }
   
        Num_refresh(); 
                                               
        PWM_output(convert_value0,convert_value1);  
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
    PITTF_PTF0 = 1;
    
    g_pitflag = 1;                             
}
#pragma CODE_SEG DEFAULT

/***************************************************************************************
    函数名称：   interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
    函数功能：   接收中断使能
    调用函数：   无
    入口参数：   无
    出口参数：   无
    返回值：     无
    其他：  	
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
    if (SCI0SR1_RDRF) 
    {
       g_sciflag = 1;
       Rev_data = SCI0DRL; 
    }   	
}
#pragma CODE_SEG DEFAULT


/*************************************************************
函数：void system_init(void)
功能：系统初始化
说明：配置GPIO并将模块初始化
*************************************************************/
void system_init(void)
{                                                                        
    SEG_DD = 0xFF;                                                  //A端口设置为输出口
    DIG_DD = 0xFF;                                                  //H端口设置为输出口
    SEG_NUM = 0x00;                                                 //A端口输出全0
    DIG_NUM = 0x00;                                                 //H端口输出全0
    SW_DD = 0;                                                      //PB4为输入口     

    SetBusCLK_64M();                                                //总线频率64M
    AD_Init();                                                      //AD模块初始化
    PIT_init();                                                     //PIT模块初始化
    PWM_init();                                                     //PWM初始化
    SCI0_Init();                                                    //SCI初始化
   
    Num_test();                                                     //数码管显示测试
    
    EnableInterrupts;                                               //中断使能
}

/*************************************************************
函数：void delay(unsigned int loop_times)
功能：延时
说明：延时500*loop_times个机器周期
*************************************************************/
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

/*************************************************************
函数：void Num_test(void)
功能：测试数码管
说明：令数码管先显示0-7
*************************************************************/
void Num_test(void)
{  
    unsigned char led_dig;                                          //数码管位码
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

/*************************************************************
函数：void Num_refresh(void)
功能：刷新数码管
说明：判断拨码开关的状态并选择通道刷新8位数码管
*************************************************************/
void Num_refresh(void)
{  
    unsigned char led_dig;                                          //数码管位码
    unsigned char i;
    unsigned char ch[8];
    
    led_dig = 0x01; 
    
    if(g_keyflag==0)                                                //若keyflag==0，则显示通道0的值
    {
        for(i=0;i<4;i++)                                            //高四位为电压转换值，低四位为数字量
        {
            ch[i] = ch0_D[i];
        }
        for(i=0;i<4;i++)          
        {
            ch[i+4] = ch0_A[i];
        }      
    }
    else                                                            //若keyflag==1,则显示通道1的值                                                      
    {
        for(i=0;i<4;i++)          
        {
            ch[i] = ch1_D[i];
        }
        for(i=0;i<4;i++)          
        {
            ch[i+4] = ch1_A[i];
        }
    }
                                                                    
    for(i=0;i<8;i++)                                                //不断刷新数码管
    {
        if(i==7)
        { 
            SEG_NUM = g_LED_Seg_Table[ch[i]]|0x80;                  //在第七位(即电压转化值第三位)显示小数点    
        }
        else
        {
            SEG_NUM = g_LED_Seg_Table[ch[i]];
        }
        SEG_NUM = g_LED_Seg_Table[ch[i]];
        DIG_NUM = led_dig;
        led_dig = led_dig<<1;
        Delay(1);       
    }  
}

/*************************************************************
函数：void AD_acquisition(void)
功能：AD采集
说明：采集两路AD数据
*************************************************************/
void AD_acquisition(void)
{     
    PITCFLMT = 0x00;                                            //禁用PIT模块
  
    ATD0CTL5 = 0x10;                                            //多通道单次采样，通道0，1  
    
    while(!ATD0STAT2L_CCF0)                                     //等待ATD0转换完成
    {
      ; 
    }                                                                         
    convert_value0 = ATD0DR0;                                   //读取ATD0转换结果
    
    while(!ATD0STAT2L_CCF1)                                     //等待ATD1转换完成
    {
      ;
    }              
    convert_value1 = ATD0DR1;                                   //读取ATD0转换结果 
   
    Filtering();                                                //数字滤波
        
    Data_split(filtering_D_value0,filtering_D_value1);          //拆分数据
    
    PITCFLMT = 0x80;                                            //使能PIT模块
}

/************************************************************
函数：void Filtering(unsigned int value0,unsigned int value1);
功能：数字滤波
说明：将两个通道的数字量进行数字滤波
************************************************************/
void Filtering(void)
{
    static unsigned char count = 0;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned int temp;
  
    value0_buf[count] = convert_value0;
    value1_buf[count] = convert_value1;
      
    count++;   
     
    if(count==10)
    {
        count = 0;
        for(j=0;j<9;j++)                                   //对通道0滤波
        {
            for(i=0;i<9-j;i++)
            {
                if(value0_buf[i]>value0_buf[i+1])          //冒泡法排序
                {
                    temp = value0_buf[i];
                    value0_buf[i] = value0_buf[i+1];
                    value0_buf[i+1] = temp;
                }
            }
        }
        filtering_D_value0 = (value0_buf[4]+value0_buf[5])/2;
        
        for(j=0;j<9;j++)                                   //对通道1滤波
        {            
            for(i=0;i<9-j;i++)
            {             
                if(value1_buf[i]>value1_buf[i+1])
                {
                    temp = value1_buf[i];
                    value1_buf[i] = value1_buf[i+1];
                    value1_buf[i+1] = temp;
                }
            }
        } 
        filtering_D_value1 = (value1_buf[4]+value1_buf[5])/2;
    }
}
/************************************************************
函数：Data_split(convert_value0,convert_value1); 
功能：数据拆分
说明：将两个通道的数字量及其电压转化值拆分后存放在4个数组中
************************************************************/
void Data_split(unsigned int value0,unsigned int value1)
{
    ch0_D[3] = value0/1000;                                           //拆分通道0的数字量并存至数组
    ch0_D[2] = value0/100%10;
    ch0_D[1] = value0/10%10;
    ch0_D[0] = value0%10;         
                                      
    filtering_A_value0 = (unsigned int)(((long)value0)*4600/4096);    //拆分通道0的电压转化值并存至数组
    ch0_A[3] = (unsigned char)(filtering_A_value0/1000);                     
    ch0_A[2] = (unsigned char)(filtering_A_value0/100%10);                                 
    ch0_A[1] = (unsigned char)(filtering_A_value0/10%10);
    ch0_A[0] = (unsigned char)(filtering_A_value0%10); 
    
    ch1_D[3] = value1/1000;                                           //拆分通道1的数字量并存至数组
    ch1_D[2] = value1/100%10;
    ch1_D[1] = value1/10%10;
    ch1_D[0] = value1%10;   
    
    filtering_A_value1 = (unsigned int)(((long)value1)*4600/4096);
    ch1_A[3] = (unsigned char)(filtering_A_value1/1000);              //拆分通道0的电压转化值并存至数组
    ch1_A[2] = (unsigned char)(filtering_A_value1/100%10);                                 
    ch1_A[1] = (unsigned char)(filtering_A_value1/10%10);
    ch1_A[0] = (unsigned char)(filtering_A_value1%10);   
}

/************************************************************
函数：void SCI_send(void)
功能：发送数据
说明：若接收到数据则将AD数据发送至上位机
************************************************************/
void SCI_send(void)
{  
    if(Rev_data=='A')                                    //若接收A，发送通道0与通道1的数据
    {    
        SCI_senddata0(filtering_D_value0);
        
        SCI_senddata0(filtering_D_value0);
        
        SCI_senddata1(filtering_D_value1);
        
        SCI_senddata1(filtering_D_value1);                    	 
    } 
}

/************************************************************
函数：void PWM_output(unsigned int value0,unsigned int value1)
功能：发送数据
说明：若接收到数据则将AD数据发送至上位机
************************************************************/
void PWM_output(unsigned int value0,unsigned int value1)
{
    unsigned long temp_dty;
    
    temp_dty = (value0+value1)*100/4096;               //将电压反算为占空比
    
    PWMDTY1 = (unsigned char)(temp_dty);    
}
