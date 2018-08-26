/***************************************************************************
    �ļ�����: main.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-12-01
    
    ˵��:     

    �޸ļ�¼: 2016-11-29���Կ���
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "System_Init.h"
#include "Image.h"
#include "Route.h"
#include "SCI.h"
#include "PID.h"
#include "Speed.h"		   

extern unsigned char black_line[40];
//extern unsigned int g_i_CurrentVelocity = 0;
//unsigned char PIT_Int_Flag1;
//float speed[4];
//=====================================================================================
void main(void) 
{   
   
    unsigned char i; 
    unsigned int speed;                                                 //ѭ������ 
    unsigned char SM_SBUFF[4] ={0,0,0,0};         
    PERT_PERT7 = 1;
    PPST_PPST7 = 0;
    //TIM_Init();  
    //SCI_Init();
    //PWM_Init(); 
    Sys_Init(); 
    delay(10000);

    EnableInterrupts;
    for(;;) 
    {
        
        //send_image();
        //send_line();
        //get_line();               
        //route_judge();
        
          steer_PD();
        	speed_PID();
            speed = get_speed();
            SM_SBUFF[3] = speed%10;                         //���ת������Ա�����ʾ
            SM_SBUFF[2] = speed/10%10;                           //���ת������Ա�����ʾ
            SM_SBUFF[1] = speed/100%10;                             //���ת������Ա�����ʾ
            SM_SBUFF[0] = speed/1000;                                 //���ת������Ա�����ʾ
           //  delay(10000);      
            for(i=0;i<4;i++) 
            {
                   SCI0SR1_TDRE = 1;
    
                   uart_putchar(SM_SBUFF[i]+0x30);                                                           //����HELLO
                  // uart_putchar(speed[4][i]+0x30)
                   while(!SCI0SR1_TC)                                                              // �ȴ����ͻ�������
                   {                                           
                         ;
                   }
     
   	               delay(2000); 
            }
          SCI0DRL = '\n';              //����
          while(!SCI0SR1_TDRE);   
              
            
    }
}




 

