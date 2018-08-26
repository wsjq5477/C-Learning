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
    
    EnableInterrupts; 
    
    DDRT = 0x00; 
    DDRA = 0x00;     
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //通道选择输入捕捉功能
    TSCR1 = 0x80;                         //1000 0000,TEN = 1，定时器正常运行
    TCTL4 = 0x0A;                         //0000 1001,通道0为01上升沿捕捉，通道1为10下降沿捕捉
    TIE = 0x03;                           //0000 0011,允许Cn0和Cn1产生中断请求
    TFLG1 = 0xFF;                         //清零所有标志位     
}    


           