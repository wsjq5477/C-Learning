/***************************************************************************
    文件名称: HCSR04_Test.c

    作者:     智能车项目摄像头组

    版本:     2016-12-20
    
    说明:     测试HCSR04超声波模块,发送测量后的数据至串口

    修改记录: 
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CRG.h"
#include "SCI.h"
#include "TIM.h"              

//=====================================================================================
void delay(unsigned int num);
void long_delay(unsigned int num); 

void main(void) 
{   
   SetBusCLK_64M();
   SCI_Init();
   TIM_Init();
   
   DDRB = 0xff;     //PB作为输出脚
   PORTB = 0x00;
   long_delay(200);
   
	 EnableInterrupts;
    
   for(;;)
   {
       
       PORTB = 0xff;   
       delay(20); 
       PORTB = 0x00;
       long_delay(200);
       Measure();
       send_data();   
       
   }
}


/*************************************************************
函数：void delay(unsigned int num);
功能：延时函数
说明：基于64M总线的延时,单次1us
*************************************************************/                       
void delay(unsigned int num)   
{ 
    unsigned int i,j; 
    for(i=0;i<num;i++) 
    {
       for(j=0;j<64;j++); 
    }
} 

/*************************************************************
函数：void long_delay(unsigned int num);
功能：延时函数
说明：基于64M总线的延时,单次1ms
*************************************************************/  
void long_delay(unsigned int num)   
{ 
    unsigned int i,j; 
    for(i=0;i<num;i++) 
    {
       for(j=0;j<64000;j++); 
    }
} 