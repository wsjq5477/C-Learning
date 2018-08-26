              /*******************************************************************************************
    �ļ����ƣ�Speed.c
    
    ��������: 
              
    ���ߣ�   
    
    ���뻷����Freescale CodeWarrier V5.0 For S12
    
    �汾��    V1.0
    
    ˵����
    
    �޸ļ�¼��
    
    �������ڣ�2016-11-18
********************************************************************************************/

#include  <hidef.h>            /*  common defines and macros   */
#include  "derivative.h"       /*  derivative-specific definitions   */
#include  "System_Init.h"
#include  "Speed.h" 

/**********************************������������*******************************************/


 /**********************************ȫ�ֱ�������**************************************/            
  unsigned int   M_PACNT; 
  unsigned int g_i_CurrentVelocity;



int get_speed(void)
{
	return g_i_CurrentVelocity;
}

  /***************************************************************************************
    ������       interrupt VectorNumber_Vpit0 void PIT_ISR(void)
    ���ܣ�       PITͨ��0�жϷ������
    ˵����
   ***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{
    
  	PITTF_PTF0 = 1;                               //���жϱ�־
    M_PACNT = PACNT;                               //��ȡ�����ۼ����ĵ�ǰֵ
    PACNT = 0;
    g_i_CurrentVelocity =  M_PACNT;			//�ٶ�ֵ�͵����鱣��                                   //���¿�ʼ����
  	                       
}
#pragma CODE_SEG DEFAULT 
