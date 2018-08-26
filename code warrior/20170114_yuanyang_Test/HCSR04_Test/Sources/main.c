/***************************************************************************
    �ļ�����: HCSR04_Test.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-12-20
    
    ˵��:     ����HCSR04������ģ��,���Ͳ����������������

    �޸ļ�¼: 
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
   
   DDRB = 0xff;     //PB��Ϊ�����
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
������void delay(unsigned int num);
���ܣ���ʱ����
˵��������64M���ߵ���ʱ,����1us
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
������void long_delay(unsigned int num);
���ܣ���ʱ����
˵��������64M���ߵ���ʱ,����1ms
*************************************************************/  
void long_delay(unsigned int num)   
{ 
    unsigned int i,j; 
    for(i=0;i<num;i++) 
    {
       for(j=0;j<64000;j++); 
    }
} 