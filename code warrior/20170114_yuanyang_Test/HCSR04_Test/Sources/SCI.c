/***************************************************************************
    文件名称: SCI.c

    作者:     智能车项目摄像头组

    版本:     2016-12-20
    
    说明:     SCI的初始化与数据发送子程序

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"

/***********************引用声明**************************/
extern unsigned char SM_SBUFF[4];

void send_str(unsigned char *putchar);
/*************************************************************
函数：void SCI_Init(void);
功能：SCI初始化
说明：
*************************************************************/ 
void SCI_Init(void)
{
    SCI0BD = 416;     //64000000/16/9600,设置波特率为9600  
    SCI0CR1 = 0x00;      
    SCI0CR2 = 0x0C;   //禁止中断             
}

/*************************************************************
函数：void send_char(unsigned char text);
功能：数据发送函数
说明：一次发送一个字符
*************************************************************/ 
void send_char(unsigned char text)
{
      
    while (!(SCI0SR1&0x80));   
    SCI0DRH = 0;
    SCI0DRL = text; 
}

/*************************************************************
函数：void send_str(unsigned char *putchar);
功能：数据发送函数
说明：一次发送一个字符
*************************************************************/ 
void send_str(unsigned char *putchar)
{
    while(*putchar!=0x00)
    {
        send_char(*putchar++);
    }
}

/****************************************************************************
函数名称：void send_data( unsigned char ch) 
函数功能：数据发送子程序
调用函数：send_char();
****************************************************************************/
void send_data(void)
{
    signed char i;
    
    send_char('r');
    send_char('a');
    send_char('n');
    send_char('g');
    send_char('e');
    send_char('=');
    
    for(i=3;i>-1;i--)
    {      
        SCI0DRL = SM_SBUFF[i];
        while(!SCI0SR1_TC)                                          // 等待发送完成
        {            
           ;
        }                            
    }
    
    SCI0DRL = 0x0D;                                        //回车
    while(!SCI0SR1_TC)                                     //等待发送完成
    {            
       ;
    }            
}