#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/****************************************
函数：void PIT_init()
功能：初始化PIT
说明：
****************************************/
void PIT_init()
{
    /* PITCFLMT = (PITE | PITSWAI | PITFRZ | 0 | //
                 0 | 0 | PFLMT1 | PFLMT0 | //
                )   */
    PITCFLMT = 0x00;       //禁止PIT模块
                           /*  
                            *  0b00000000
                            *    ||||||||__微计数器0的强制加载位 
                            *    |||||||___微计数器1的强制加载位 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______冻结模式下PIT计数器冻结位
                            *    ||________等待模式下PIT停止位
                            *    |_________PIT模块使能位
                            */  
    PITCE_PCE0 = 1;        //使能定时器通道0

    /* PITMUX = (0 | 0 | 0 | 0 | //
                 PMUX3 | PMUX2 | PMUX1 | PMUX0 | //
                )   */
    PITMUX = 0x00;        //定时器通道0使用Base Timer0 (微计数器0)
                           /*  
                            *  当PMUX3~PMUX0：定时器通道3~0的复用位。当PITMUXn被置1时，第n路定时器
                            *  使用微计数器1５盤ITMUXn被置0时，第n路定时器使用微计数器0   
                            */ 
    /* PITMTLD0 = (PMTLD7~PMTLD0)   */
    PITMTLD0 = 0xFF;      
    /* PITLD0 = (PLD15~PLD0)   */                        
    PITLD0 = 0x30D4;       //设置16位计数器的加载寄存器
                           /*  PITMTLD0与PITLD0共同决定定时器通道0的定时周期
                            *  定时周期为($PITMTLD0+1)*($PITLD0+1)个内部总线时钟周期
                            *  因此($PITMTLD0+1)*($PITLD0+1) = 3.2M,即定时周期为4M个内部总线时钟周期
                            *  内部总线时钟周期为1/32M，所以定时周期为3.2M*(1/32M) = 0.1s
                            */


    /* PITMUX = (0 | 0 | 0 | 0 | //
                 PINTE3 | PINTE2 | PINTE1 | PINTE0 | //
                 )   */
    PITINTE = 0x01;         //使能PIT通道0的中断
                           /*  
                            *  0b00000001
                            *    ||||||||__使能PIT通道0
                            *    |||||||___使能PIT通道1
                            *    ||||||____使能PIT通道2
                            *    |||||_____使能PIT通道3 
                            *    ||||______
                            *    |||_______
                            *    ||________
                            *    |_________
                            */  

    PITCFLMT = 0x80;        //使能PIT模块
}
