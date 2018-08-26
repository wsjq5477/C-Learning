/***************************************************************************
    文件名称: main.c

    作者:     智能车项目摄像头组

    版本:     2016-12-01
    
    说明:     

    修改记录: 2016-11-29测试可跑
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
    unsigned int speed;                                                 //循环变量 
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
            SM_SBUFF[3] = speed%10;                         //拆分转换结果以便送显示
            SM_SBUFF[2] = speed/10%10;                           //拆分转换结果以便送显示
            SM_SBUFF[1] = speed/100%10;                             //拆分转换结果以便送显示
            SM_SBUFF[0] = speed/1000;                                 //拆分转换结果以便送显示
           //  delay(10000);      
            for(i=0;i<4;i++) 
            {
                   SCI0SR1_TDRE = 1;
    
                   uart_putchar(SM_SBUFF[i]+0x30);                                                           //发送HELLO
                  // uart_putchar(speed[4][i]+0x30)
                   while(!SCI0SR1_TC)                                                              // 等待发送缓冲区空
                   {                                           
                         ;
                   }
     
   	               delay(2000); 
            }
          SCI0DRL = '\n';              //换行
          while(!SCI0SR1_TDRE);   
              
            
    }
}




 

