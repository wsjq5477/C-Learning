 /***********************************************************
    文件名称:CRG.c
    项目名称:run.mcp
    作 者:   飞思卡尔智能车工作室
    版 本:   2014.10.26
    说 明:   总线频率选择
    修改记录:
***********************************************************/
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
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0x40 | 0x0F;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
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
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0x40 | 0x07;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz;
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0x40 | 0x03;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
    #endif    
                            
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
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
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
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0x40 | 0x13;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
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
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0x40 | 0x09;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0x40 | 0x04;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
    #endif    
                            
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
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
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
                
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x17;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
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
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x0B;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x05;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
    #endif    
                            
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
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
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
                
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x1F;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
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
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x0F;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x07;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
    #endif 
                            
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
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
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
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x27;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
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
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x13;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x09;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
    #endif 
                            
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
                            *    |_________REFFRQ[1:0]用于设置参考时钟REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //短暂延时，等待时钟频率稳定
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //时钟频率已稳定，锁相环频率锁定
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //使能锁相环时钟
}

