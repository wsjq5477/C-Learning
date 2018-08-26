#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

extern void ADC_init();

/*************************************
函数void ADC_Init()
功能：ATD0,ATD1初始化
说明：
****************************************/
void ADC_init()         //初始化
{
    ATD0DIEN = 0;           //禁止数字输入
    /* APCTL1 = (ETRIGSEL | SRES1 | SRES0 | SMP_DIS | //
                 ETRIGCH3 | ETRIGCH2 | ETRIGCH1 | ETRIGCH0 | //
                )   */
    ATD0CTL1 = 0x40;       //AD分辨率选择12位，且采样前不放电
                           /*  
                            *  0b01000000
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____[ETRIGCH3~ETRIGCH0]外部触发通道选择位 
                            *    |||||_____ 
                            *    ||||______采样前是否放电控制位 
                            *    |||_______ 
                            *    ||________ATD转换精度选择位SRES1:SRES0]=[1:0]为12位
                            *    |_________外部触发源选择位，S12XS系列此位无效 
                            */  
    /* APCTL2 = (0 | AFFC | ICLKSTP | ETRIGLE | //
                 ETRIGP | ETRIGE | ASCIE | ACMPIE | //
                )   */
    ATD0CTL2 = 0x40;       //AD模块快速清零, 无等待模式, 禁止外部触发, 中断禁止
                           /*  
                            *  0b01000000
                            *    ||||||||__ATD比较中断使能位
                            *    |||||||___ATD转换序列结束中断使能位 
                            *    ||||||____外部触发信号使能位
                            *    |||||_____ 
                            *    ||||______[ETRIGLE:ETRIGP]外部触发信号触发条件选择位
                            *    |||_______停止模式内部时钟使能位
                            *    ||________AD模块快速清零位
                            *    |_________只读位 
                            */
    /* APCTL3 = (DJM | S8C | S4C| S2C | //
                 S1C| FIFO | FRZ1 | FRZ0 | //
                )   */
    ATD0CTL3 = 0x90;       //AD转换结果右对齐，每个序列2次转换, No FIFO, Freeze模式下继续转换
                           /*  
                            *  0b10010000
                            *    ||||||||__[FRZ1:FRZ0]后台调试冻结使能位
                            *    |||||||___
                            *    ||||||____结果寄存器先进先出模式位
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______
                            *    ||________[S8C~S1C]一个转换序列的长度
                            *    |_________结果寄存器数据对齐位 
                            */
    /* APCTL4 = (SMP2 | SMP1 | SMP0 | PRS4 | //
                 PRS3 | PRS2 | PRS1 | PRS0 | //
                )   */
    ATD0CTL4 = 0x03;       //采样时间4个周期，PRS=3
                           /*  
                            *  0b00000011
                            *    ||||||||__[PRS4~PRS0]ATD时钟预分频因子
                            *    |||||||___
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______[SMP2~SMP0]采样时间选择位
                            *    ||________
                            *    |_________ 
                            */
}