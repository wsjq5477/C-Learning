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
        ��:1210
        ��:1345(135)
        ��:1075(135)
        ***********/
        //send_line();
        //send_image();
        /***********�򵥲��Զ��*******************      
        
        binaryzation();          //��ֵ������
        route_extract();         //������ȡ

        if(black_line[37]-black_line[7]>5) //�����
        {
            delay(5);            
            PWMDTY67=1530;           
        }      
        if(black_line[7]-black_line[37]>5) //�����
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
 

