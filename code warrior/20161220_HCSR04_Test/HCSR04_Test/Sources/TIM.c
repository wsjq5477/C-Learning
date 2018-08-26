/***************************************************************************
    文件名称: TIM.c

    作者:     智能车项目摄像头组

    版本:     2016-12-20
    
    说明:     TIM模块的初始化与中断服务子程序

    修改记录: 
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "TIM.h"

/******************************函数声明************************************/
void TIM_Init(void);
void Measure(void);
void Data_split(unsigned int value);

/*************************全局变量定义**********************************/
unsigned char echo_flag = 0;             //测量完毕标志位
unsigned int temp_count = 0;
unsigned int range = 0;
unsigned char SM_SBUFF[4];

/*************************************************************
函数：void TIM_Init(void);
功能：TIM初始化
说明：
*************************************************************/ 
void TIM_Init(void)
{   
    TSCR1 = 0x90;
    TIOS  = 0x00;
    PACTL_PAEN = 0;     //关闭脉冲累加器使能
    PACNT = 0x0000;   
    PACTL_PAMOD = 1;    //门控时间累加
    PACTL_PEDGE = 0;    //高电平计数
    PACTL_CLK1 = 0;     //64分频,PACLK=1MHz
    PACTL_CLK0= 1;
    PACTL_PAI = 1;                                                                           
    PACTL_PAEN = 1;     //打开脉冲累加器使能
}   


/**************************************************************************************
函数：void Measure(void) 
功能：计算超声波模块测量的距离
说明：
***************************************************************************************/
void Measure(void) 
{                                                                           
    if(echo_flag==1)
    {           
        range = (temp_count/10)*34/10+40;                //单位为mm,20mm-4000mm
    }   	
    Data_split(range);
}

/****************************************
函数：void Data_split(void)
功能：数据拆分
说明：将4位数拆分为个4个数据并存放在数组中
****************************************/
void Data_split(unsigned int value)
{   
    unsigned int temp;
    temp = value;
                                
    SM_SBUFF[3] =  48+temp/1000;                  //将结果拆分并储存
    SM_SBUFF[2] =  48+temp/100%10;                                 
    SM_SBUFF[1] =  48+temp/10%10;
    SM_SBUFF[0] =  48+temp%10; 
}

/******************************中断服务子程序************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED  
interrupt VectorNumber_Vtimpaie void ECT_0_ISR(void)    //PT7中断
{    
    PAFLG_PAIF = 1;
    
    temp_count = PACNT;                                //一次1us
    echo_flag = 1;
    
    PACNT = 0;    
}                                     
#pragma CODE_SEG DEFAULT 