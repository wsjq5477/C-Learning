/***************************************************************************
    文件名称: SCI.c

    项目名称: Compositive_Test_V2.0.mpc 

    作者:     季齐

    版本:     V1.0
    
    说明:     完成SCI的初始化并定义发送数据函数

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"

void SCI0_Init(void)	                                      
{
    SCI0BD = 64000000/16/BAUDRATE;    //设置波特率为9600，06400000/16/9600 ；
          
	 	/* SCI1C1 = (LOOPS | SCISWAI | RSRC |  M  | //
                 WAKE  |    ILT   |  PE | PT  | //
                )   */
	  SCI0CR1 = 0x00;                /*  
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
	  SCI0CR2 = 0x2C;                /*  
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

/****************************************************************************
    函数名称：void SCI_putchar( unsigned char ch) 
    函数功能：字符发送子程序
    调用函数：无

****************************************************************************/
void SCI_putchar( unsigned char ch) 
{
    SCI0DRL = ch;	                                               // 向缓冲区写入数据
    
    while(!SCI0SR1_TC);                                          // 等待发送完成       	  
}

/****************************************************************************
    函数名称：void SCI_putstr( unsigned char ch) 
    函数功能：字符串发送子程序
    调用函数：SCI_putchar(); 

****************************************************************************/
void SCI_putstr(char *p) 
{
    while(*p)
    {
        SCI_putchar(*p);
        p++;        
    }
}

/****************************************************************************
    函数名称：void SCI_senddata0( unsigned char ch) 
    函数功能：数据发送子程序
    调用函数：SCI_putstr();

****************************************************************************/
void SCI_senddata0( unsigned int data0)
{
    unsigned int temp_data0;
    signed char ch0[4];
    signed char (*p)[4];
    
    temp_data0 = data0*4600/4096;                      //将数字量转化为电压值
     
    ch0[3] =  temp_data0/1000;                         //将结果拆分并储存
    ch0[2] =  temp_data0/100%10;                                 
    ch0[1] =  temp_data0/10%10;
    ch0[0] =  temp_data0%10;  
    
    p = &ch0;
    
    SCI_putstr("ch0 = ");                              //发送"ch0 = "
    
    SCI_putstr(*p);                                    //发送ch0数据
}

/****************************************************************************
    函数名称：void SCI_senddata0( unsigned char ch) 
    函数功能：数据发送子程序
    调用函数：无

****************************************************************************/
void SCI_senddata1( unsigned int data1)
{
    unsigned int temp_data1;
    signed char ch1[4] ;
    signed char (*p)[4] ;
    
    temp_data1 = data1*4600/4096;                 //将数字量转化为电压值
    
    ch1[3] =  temp_data1/1000;                         //将结果拆分并储存
    ch1[2] =  temp_data1/100%10;                                 
    ch1[1] =  temp_data1/10%10;
    ch1[0] =  temp_data1%10;  
    
    p = &ch1;
    
    SCI_putstr("ch1 = ");
    
    SCI_putstr(*p);
}




