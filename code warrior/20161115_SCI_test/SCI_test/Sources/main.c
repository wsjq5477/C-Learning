/***************************************************************************
    �ļ�����: main.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-04
    
    ˵��:     

    �޸ļ�¼: ��
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
 

