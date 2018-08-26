/***********************************************************
    文件名称: sys_init.c
    说 明:   初始化程序
***********************************************************/


/***************************************************
** 函数名称: void sys_init(void)
** 功能描述: CRG\SCI\TIM初始化函数
** 说明:     对各功能模块进行初始化设置
****************************************************/
#include <hidef.h>     
#include "derivative.h" 
#include  "include.h"

/*****************************函数声明***********************************/
void sys_init(void) 
{
    TIM_init();         //TIM初始化
   
    SCI0_Init();        //SCI初始化
    
    DDRA = 0x00;        //对IO口设置输出输入
   
    DDRB = 0X00;
    DDRT = 0X00;

    init_PIT();    
}


void TIM_init(void)
{   

    TIOS = 0x00;                         //通道选择输入捕捉功能
    TSCR1 = 0x80;                         //1000 0000,TEN = 1，定时器正常运行
   
    TCTL4 = 0x18;                         //0001 1000,通道2为01上升沿捕捉，通道1为10下降沿捕捉
    TIE = 0x06;                           //0000 0110,允许Cn1和Cn2产生中断请求
    TFLG1 = 0xFF;                         //清零所有标志位
      
}


void PA_Init(void)
{
    PACTL=0x40;
    PACNT=0x00;
}

void init_PIT(void)
{
    PITINTE=0X01;
    PITCE=0X01;
    PITMUX= 0x00;
    PITFLT=0X01;
    PITLD0= 4000;

    PITMTLD0=200;
    PITCFLMT = 0x81; 
}

/****************
void zhongduanyouxianji(void)
{
  	INT_CFADDR  = 0x70;		  //PIT0中断优先级设为7 
  	INT_CFDATA5 = 0x05;
  	INT_CFADDR  = 0xE0;		  //TIM1（场中断）优先级设为6
  	INT_CFDATA6 = 0x07;
  	INT_CFADDR  = 0xE0;		  //TIM0（行中断）优先级设为5
  	INT_CFDATA7 = 0x06;	   
}
****************/
