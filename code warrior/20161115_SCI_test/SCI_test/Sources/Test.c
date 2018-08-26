/***************************************************************************
    文件名称: TEST.c

    作者:     智能车项目摄像头组

    版本:     2016-11-20
    
    说明:     测试不同C文件调用同一个数组

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"
#include "test.h"

extern unsigned char image[10][10];
unsigned char image_two[10][10];

void test(void)
{   
    unsigned char i1,j1;
    
    for(i1=0;i1<10;i1++)
    {
        for(j1=0;j1<10;j1++)
        {
            if(image[i1][j1]==10)
            {
                image_two[i1][j1]=0;
            }
            else 
            {
                image_two[i1][j1]=1;
            }
        }
    }
}