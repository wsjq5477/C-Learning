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
    PAC_Init(); 
    SCI0_Init();
    PWM_Init();
    PIT_Init();  
    
    DDRT = 0x00; 
    DDRA = 0x00;   
                 
    EnableInterrupts;         
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //通道选择输入捕捉功能
    TSCR1 = 0x80;                         //1000 0000,TEN = 1，定时器正常运行
    TCTL4 = 0xA9;                         //1010 1001,通道2和3位上升沿捕捉，通道0为01上升沿捕捉，通道1为10下降沿捕捉
    TIE = 0x0F;                           //0000 1111,允许Cn3、Cn2、Cn0和Cn1产生中断请求
    TFLG1 = 0xFF;                         //清零所有标志位    
}    

void PAC_Init(void)
{}
                                                                                                     
void PWM_Init(void)
{
    PWME=0x00;        //中断使能关闭
    
    DDRP_DDRP2 = 1;
    PTP_PTP2 = 0; 
        
    PWMPRCLK=0x33;    //CLOCKA,B预分频8=64/8=8M
    
    PWMSCLA=0x04;
    PWMSCLB=0x04;     //对时钟B比例分频,CLOCKSB=CLOCKB/(2*4)=8/8=1M=1000000
    PWMCTL_CON67 = 1;
       
    PWMCLK_PCLK7 = 1;
    PWMCLK_PCLK0 = 1;
    PWMCLK_PCLK1 = 1; //通道0,1选择CLOCKSA;通道6,7选择CLOCKSB
    
    PWMPER67=20000;   //周期20000,理论上为50Hz(20ms周期)
    PWMPER0 = 200;    //电机反转
    PWMPER1 = 200;    //电机正转
    
    PWMDTY67=1210;    //高电平1210us,中间位置
    PWMDTY0 = 0;
    PWMDTY1 = 50;      //占空比25%    
    
    PWMPOL=0xFF;      //极性选择开始为高电平
    PWMCAE=0x00;      //通道左对齐
                                                          
    PWME_PWME7= 1;
    PWME_PWME0= 1;
    PWME_PWME1= 1;     
}
/****************************************
函数：void PIT_init(void)
功能：初始化PIT，设置精确定时时间为10ms
说明：
****************************************/
void PIT_Init(void)
{

    PITCFLMT = 0x00;       //禁止PIT模块
 
    PITCE_PCE0 = 1;        //使能定时器通道0

    PITMUX = 0x00;        //定时器通道0使用Base Timer0 (微计数器0) 
                          
    /* PITMTLD0 = (PMTLD7~PMTLD0)   */
    PITMTLD0 = 0x31;      
    /* PITLD0 = (PLD15~PLD0)   */                        
    PITLD0 = 0x31FF;       //设置16位计数器的加载寄存器
                           /*  PITMTLD0与PITLD0共同决定定时器通道0的定时周期
                            *  定时周期为($PITMTLD0+1)*($PITLD0+1)个内部总线时钟周期
                            *  因此($PITMTLD0+1)*($PITLD0+1) = 0.64M,即定时周期为4M个内部总线时钟周期
                            *  内部总线时钟周期为1/64M，所以定时周期为0.64M*(1/64M) = 0.01s
                            */
    PITINTE = 0x01;         //使能PIT通道0的中断
                         
  
    PITCFLMT = 0x80;        //使能PIT模块
}
           