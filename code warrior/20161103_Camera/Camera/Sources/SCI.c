/*******************************************************************************************
    文件名称：SCI.c
    
    功能描述: 完成SCI的初始化及数据的发送
              
    作者：    合肥工业大学飞思卡尔嵌入式系统实验室
    
    编译环境：Freescale CodeWarrier V5.1 For xs128
    
    版本：    V1.3
    
    说明：
    
    修改记录：
    
****************************************************************************************/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include"include.h"

/**********************函数声明********************/
extern void SCI0_Init(void);
extern unsigned char SCI0_rec_dat(void);
extern void uart_putchar( unsigned char ch);
extern void putstr(char ch[]);

/**************************************************************************************
    函数：SCI0_Init(void)
    功能：SCI0初始化
    入口参数：无
    出口参数：无
    说明：
**************************************************************************************/

void SCI0_Init(void) 
{
    SCI0BD = 80000000 / 16 / BAUDRATE;
    /* SCI1C1 = (LOOPS | SCISWAI | RSRC |  M  | //
                 WAKE  |    ILT   |  PE | PT  | //
                )   */
    SCI0CR1 = 0x00;            /*  
                                    *  0b00000000
                                    *    ||||||||__ 仅在PE=1时有效
                                    *    |||||||___ 奇偶校验禁止
                                    *    ||||||____ 在开始后立即对空闲特征位计数
                                    *    |||||_____ 空闲线唤醒
                                    *    ||||______ 8位正常模式
                                    *    |||_______ 此位仅在LOOPS=1时有效
                                    *    ||________ SCI在等待模式下继续运行
                                    *    |_________ 正常工作，RxD和TxD使用独立引脚
                                    */
    /* SCI1C2 = (TIE | TCIE | RIE | ILIE  | //
                 TE  | RE |  RWU  | SBK | //
                )   */   
    SCI0CR2 = 0x0C;            /*  
                                    *  0b00101100
                                    *    ||||||||__ 正常发送器操作
                                    *    |||||||___ 正常SCI接收器操作
                                    *    ||||||____ 接收器使能
                                    *    |||||_____ 发送器使能
                                    *    ||||______ IDLE中断请求禁止
                                    *    |||_______ RDRF中断请求开启
                                    *    ||________ TC中断请求禁止
                                    *    |_________ 中断请求禁止
                                    */ 
}

/**************************************************************************************
    函数：SCI0_rec_dat(void)
    功能：通过SCI0接收数据
    入口参数：无
    出口参数：unsigned char dat，返回接收到的无符号字符型数据
    说明：
**************************************************************************************/
unsigned char SCI0_rec_dat(void) 
{
    return(SCI0DRL);
}

/****************************************
函数：void uart_putchar( unsigned char ch) 
功能：发送字符子程序
说明：
****************************************/
/*void uart_putchar(unsigned char ch) 
{
    if(ch == '\n')            //输出空格
    {
        while(!(SCI0SR1&0x80)) 
        {
            ;
        }
        SCI0DRL = 0x20;
        while(!SCI0SR1_TC);
        return;
    }//end of if(ch == '\n')
    
    if(ch == '\r')            //输出换行
    {
        while(!(SCI0SR1&0x80)) 
        {
            ;
        }
        SCI0DRL = 0x0d;
        while(!SCI0SR1_TC);
        return;
    }//end of if(ch == '\r')
    
    while(!(SCI0SR1&0x80))     //等待接收完成
    {
        ;
    }
    SCI0DRL = ch;
    while(!SCI0SR1_TC);             //输出字符窜的字符      
}
  */
/****************************************
函数：void uart_putchar( unsigned char ch) 
功能：发送字符串子程序
说明：
****************************************/
/*void putstr(char ch[]) 
{
    unsigned char ptr = 0;
    while(ch[ptr]) 
    {
        uart_putchar((unsigned char)ch[ptr++]);   
    }      
}   */