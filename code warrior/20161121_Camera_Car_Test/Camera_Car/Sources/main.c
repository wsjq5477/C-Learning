/***************************************************************************
    文件名称: main.c

    作者:     智能车项目摄像头组

    版本:     2016-11-04
    
    说明:     

    修改记录: 无
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "System_Init.h"
#include "Image.h"
#include "Route.h"

extern unsigned char black_line[40];


//=====================================================================================
void main(void) 
{     
    Sys_Init();
    delay(10000);            
    
    for(;;) 
    {
        PWMDTY67 = 1210;
        
        /***********
        中:1210
        左:1345(135)
        右:1075(135)
        ***********/
        //send_line();
        //send_image();
        /***********简单测试舵机*******************      
        
        binaryzation();          //二值化处理
        route_extract();         //黑线提取

        if(black_line[37]-black_line[7]>5) //左弯道
        {
            delay(5);            
            PWMDTY67=1530;           
        }      
        if(black_line[7]-black_line[37]>5) //右弯道
        {
            delay(5);
            PWMDTY67=1370;
        } 
        else
        {
            delay(5);     
            PWMDTY67=1100;           
        }
        */
  
    }
}
 

