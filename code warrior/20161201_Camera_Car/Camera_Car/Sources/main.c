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
#include "CRG.h"

extern unsigned char black_line[40];

//=====================================================================================
void main(void) 
{   
    
    SetBusCLK_64M();
    
    delay(10000);
    delay(10000);
    delay(10000); 
     
    for(;;) 
    {
        
        send_image();
        //send_line();
        //get_line();               
        //route_judge();
        //steer_PD();
            
    }
}
 

