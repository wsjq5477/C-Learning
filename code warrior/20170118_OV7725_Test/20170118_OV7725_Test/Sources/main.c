/***************************************************************************
    �ļ�����: main.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-04
    
    ˵��:     

    �޸ļ�¼: ��
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "System_Init.h"
#include "Image.h"

//=====================================================================================
void main(void) 
{     
    Sys_Init();
    
    for(;;) 
    {
        send_image();                      
    }
}
 

