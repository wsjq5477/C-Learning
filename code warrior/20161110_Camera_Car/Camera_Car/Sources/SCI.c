/***************************************************************************
    文件名称: SCI.c

    作者:     智能车项目摄像头组

    版本:     2016-11-04
    
    说明:     完成SCI的初始化定义并打包发送图像

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"

/*************************************************************
函数：void SCI_Init(void);
功能：SCI初始化
说明：
*************************************************************/ 
void SCI_Init(void)
{
    SCI0BD = 416;     //64000000/16/9600  
    SCI0CR1 = 0x00;      
    SCI0CR2 = 0x0C;   //禁止中断             
}

/*************************************************************
函数：void send_data(unsigned char text);
功能：数据发送函数
说明：一次发送一个字符
*************************************************************/ 
void send_data(unsigned char text)
{
      
    while (!(SCI0SR1&0x80));   
    SCI0DRH = 0;
    SCI0DRL = text; 
}





