/***************************************************************************
    �ļ�����: main.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-12-01
    
    ˵��:     

    �޸ļ�¼: 2016-11-29���Կ���
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CRG.h"
void main(void) 
{   
    
    SetBusCLK_64M();
    PWME = 0x00;	        
     DDRP_DDRP2 = 1;
     PTP_PTP2 = 0;                
     PWMPRCLK = 0x03;			  
                   
     PWMSCLA = 80;			    
     PWMCLK = 0x03;			    
     PWMPER0 = 200;			   
     PWMPER1 = 200;
       
     PWMDTY0 = 40;			      
     PWMDTY1 = 0;                        
     
     PWMPOL = 0xFF;			    
     PWMCAE = 0x00;			       
     PWMCTL = 0x00;			     
     PWME = 0x03; 	 
       
      for(;;) 
      {
          
      }
}