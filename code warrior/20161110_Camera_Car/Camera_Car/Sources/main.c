/***************************************************************************
    �ļ�����: main.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-04
    
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

extern unsigned char black_line[40];

//=====================================================================================
void main(void) 
{   
    
    Sys_Init();
    delay(10000);            
     
    for(;;) 
    {
        
        send_image();
        
    }
}
 

