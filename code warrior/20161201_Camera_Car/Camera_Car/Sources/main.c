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
 

