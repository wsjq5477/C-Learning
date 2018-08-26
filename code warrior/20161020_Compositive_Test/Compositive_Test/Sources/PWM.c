/***************************************************************************
    文件名称: PWM.c

    项目名称: Compositive_Test_V3.0.mpc 

    作者:     季齐

    版本:     V1.0
    
    说明:     完成PWM的初始化

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "PWM.h"

void PWM_init(void) 
{
    PWME = 0x00;              //禁止PWM
    
    PWMPRCLK = 0x06;          //CLOCKA预分频设置为3
    
    PWMSCLA = 125;            //SA分频因子设置为125
    
    PWMCLK = 0x02;			      //通道0,1选择ClockSA作为PWM时钟
    
    PWMPOL = 0xFF;            //周期脉冲开始时输出高电平    
    
    PWMCAE = 0x00;            //左对齐
    
    PWMCTL = 0x08;            //级联通道0和通道1级联
    
    PWMPER1 = 200;			      //通道1周期寄存器设置为200
                               
    PWMDTY1 = 0;			        //通道1占空比寄存器设置为0%
    
    PWME = 0x03;              //使能PWM                                                       
}