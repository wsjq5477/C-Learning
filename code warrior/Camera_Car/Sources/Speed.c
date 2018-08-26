              /*******************************************************************************************
    文件名称：Speed.c
    
    功能描述: 
              
    作者：   
    
    编译环境：Freescale CodeWarrier V5.0 For S12
    
    版本：    V1.0
    
    说明：
    
    修改记录：
    
    创建日期：2016-11-18
********************************************************************************************/

#include  <hidef.h>            /*  common defines and macros   */
#include  "derivative.h"       /*  derivative-specific definitions   */
#include  "System_Init.h"
#include  "Speed.h" 

/**********************************公共函数声明*******************************************/


 /**********************************全局变量声明**************************************/            
  unsigned int   M_PACNT; 
  unsigned int g_i_CurrentVelocity;



int get_speed(void)
{
	return g_i_CurrentVelocity;
}

  /***************************************************************************************
    函数：       interrupt VectorNumber_Vpit0 void PIT_ISR(void)
    功能：       PIT通道0中断服务程序
    说明：
   ***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{
    
  	PITTF_PTF0 = 1;                               //清中断标志
    M_PACNT = PACNT;                               //读取脉冲累加器的当前值
    PACNT = 0;
    g_i_CurrentVelocity =  M_PACNT;			//速度值送到数组保存                                   //重新开始计数
  	                       
}
#pragma CODE_SEG DEFAULT 
