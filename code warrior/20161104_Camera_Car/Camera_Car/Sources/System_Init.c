/***************************************************************************
    文件名称: System_Init.c

    作者:     智能车项目摄像头组

    版本:     2016-11-04
    
    说明:     完成系统的初始化

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "System_Init.h"
#include "SCI.h"
#include "CRG.h"

void Sys_Init(void) 
{
    DisableInterrupts; 
    
    SetBusCLK_64M();
    TIM_Init();  
    SCI_Init();
    PWM_Init();  
                  
    EnableInterrupts; 
    
    DDRT = 0x00; 
    DDRA = 0X00;     
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //通道选择输入捕捉功能
    TSCR1 = 0x80;                         //1000 0000,TEN = 1，定时器正常运行
    TCTL4 = 0x18;                         //0001 1000,通道2为01上升沿捕捉，通道1为10下降沿捕捉
    TIE = 0x06;                           //0000 0110,允许Cn1和Cn2产生中断请求
    TFLG1 = 0xFF;                         //清零所有标志位     
}    

void PWM_Init(void)
{
    PWME=0x00;        //中断使能关闭
      
    PWMPRCLK=0x30;    //CLOCKB预分频8=64/8=8M
    PWMSCLB=0x04;     //对时钟B比例分频,CLOCKSB=CLOCKB/(2*4)=8/8=1M=1000000   
    PWMCLK=0xC0;      //通道6,7选择CLOCKSB
    PWMCTL=0x80;      //通道6,7级联
    
    PWMPER67=20000;    //周期20000,理论上为50Hz(20ms周期)
    PWMDTY67=1520;     //高电平1520us,中间位置
    
    /****************占空比设置*********************        
    高电平为1520us时：一定为中间位置；
    高电平为920us时：一定为最左边位置；
    高电平为2120us时：一定为最右边位置
                        
    PWMDTY7=920;      //高电平920us,最左边
    PWMDTY7=2500;     //高电平2120us,最右边   
    **********************************************/
    
    PWMPOL=0xFF;      //极性选择开始为高电平
    PWMCAE=0x00;      //通道左对齐
                                                          
    PWME_PWME7=1;

}

           