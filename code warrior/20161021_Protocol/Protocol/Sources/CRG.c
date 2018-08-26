/********************************************************************************************
    文件名称：CRG.c
    
    功能描述: 通过设置时钟产生器（CRG）模块，改变总线频率，实现超频
              
    作者：    合肥工业大学飞思卡尔嵌入式系统实验室
    
    编译环境：Freescale CodeWarrier V5.0 For S12
    
    版本：    V1.0
    
    说明：
    
    修改记录：
    
    创建日期：2011-3-20
********************************************************************************************/

#include  <hidef.h>            /*  common defines and macros   */
#include  "derivative.h"       /*  derivative-specific definitions   */

#include  "CRG.h"              /*  some deal about PLL  */



/**********************************公共函数声明*******************************************/

extern void SetBusCLK_32M(void);                         //设置总线频率为32M
extern void SetBusCLK_40M(void);                         //设置总线频率为40M
extern void SetBusCLK_48M(void);                         //设置总线频率为48M
extern void SetBusCLK_64M(void);                         //设置总线频率为64M
extern void SetBusCLK_80M(void);                         //设置总线频率为80M

/*************************************************************************************************************
  函数名称  : void SetBusCLK_32M(void)
  
  功能简介  : 设置总线频率为32M
  
  入口参数  : 无
  
  出口参数  : 无 
***************************************************************************************************************/

void SetBusCLK_32M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //不使能锁相环时钟
   
    PLLCTL_PLLON=1;                        //锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    SYNR =0x40 | 0x03;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
                           /*  
                            *  0b01000011
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______SYNR[5:0]用于改变锁相环时钟倍频 
                            *    ||________VCOFRQ[1:0]=0:1,代表VCO_clock在48~80M之间
                            *    |_________VCOFRQ[1:0]用于控制VCO_clock的增益
                            */
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]用于改变锁相环时钟分频 
                            *    ||________REFFRQ[1:0]=1:0,因为参考时钟在6~12M之间
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M
                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}
/*************************************************************************************************************
  函数名称  : void SetBusCLK_40M(void)
  
  功能简介  : 设置总线频率为40M
  
  入口参数  : 无
  
  出口参数  : 无 
***************************************************************************************************************/

void SetBusCLK_40M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //不使能锁相环时钟
   
    PLLCTL_PLLON=1;                        //锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    SYNR =0xc0 | 0x04;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
                           /*  
                            *  0b11000100
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______SYNR[5:0]用于改变锁相环时钟倍频 
                            *    ||________VCOFRQ[1:0]=1:1,代表VCO_clock在80~160M之间
                            *    |_________VCOFRQ[1:0]用于控制VCO_clock的增益
                            */
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]用于改变锁相环时钟分频 
                            *    ||________REFFRQ[1:0]=1:0,因为参考时钟在6~12M之间
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M
                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}
/*************************************************************************************************************
  函数名称  : void SetBusCLK_48M(void)
  
  功能简介  : 设置总线频率为48M
  
  入口参数  : 无
  
  出口参数  : 无 
***************************************************************************************************************/

void SetBusCLK_48M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //不使能锁相环时钟
   
    PLLCTL_PLLON=1;                        //锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    SYNR =0xc0 | 0x05;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
                           /*  
                            *  0b11000101
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______SYNR[5:0]用于改变锁相环时钟倍频 
                            *    ||________VCOFRQ[1:0]=1:1,代表VCO_clock在80~160M之间
                            *    |_________VCOFRQ[1:0]用于控制VCO_clock的增益
                            */
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]用于改变锁相环时钟分频 
                            *    ||________REFFRQ[1:0]=1:0,因为参考时钟在6~12M之间
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            
                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}
/*************************************************************************************************************
  函数名称  : void SetBusCLK_64M(void)
  
  功能简介  : 设置总线频率为64M
  
  入口参数  : 无
  
  出口参数  : 无 
***************************************************************************************************************/

void SetBusCLK_64M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //不使能锁相环时钟
   
    PLLCTL_PLLON=1;                        //锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    SYNR =0xc0 | 0x07;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
                           /*  
                            *  0b11000111
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______SYNR[5:0]用于改变锁相环时钟倍频 
                            *    ||________VCOFRQ[1:0]=1:1,代表VCO_clock在80~160M之间
                            *    |_________VCOFRQ[1:0]用于控制VCO_clock的增益
                            */
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]用于改变锁相环时钟分频 
                            *    ||________REFFRQ[1:0]=1:0,因为参考时钟在6~12M之间
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            
                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}
/*************************************************************************************************************
  函数名称  : void SetBusCLK_80M(void)
  
  功能简介  : 设置总线频率为80M
  
  入口参数  : 无
  
  出口参数  : 无 
***************************************************************************************************************/


void SetBusCLK_80M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //不使能锁相环时钟
   
    PLLCTL_PLLON=1;                        //锁相环电路允许
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    SYNR =0xc0 | 0x09;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
                           /*  
                            *  0b11001001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______SYNR[5:0]用于改变锁相环时钟倍频 
                            *    ||________VCOFRQ[1:0]=1:1,代表VCO_clock在80~160M之间
                            *    |_________VCOFRQ[1:0]用于控制VCO_clock的增益
                            */
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]用于改变锁相环时钟分频 
                            *    ||________REFFRQ[1:0]=1:0,因为参考时钟在6~12M之间
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            
                            */
                                               
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}
