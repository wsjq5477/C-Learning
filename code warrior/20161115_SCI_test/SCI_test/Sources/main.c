/***************************************************************************
    文件名称: main.c

    作者:     智能车项目摄像头组

    版本:     2016-11-04
    
    说明:     

    修改记录: 无
***************************************************************************/

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"
#include "Test.h"
#include "CRG.h"


//=====================================================================================
void main(void) 
{     
    SetBusCLK_64M();
    SCI_Init();
    
    send_test();
    
    for(;;) 
    {
       
    }
}
 

