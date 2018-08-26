#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"

/**********************函数声明********************/
//需外部调用

/**************************************************************************************
    函数：SCI0_Init(void)
    功能：SCI0初始化
    入口参数：无
    出口参数：无
    说明：
**************************************************************************************/

void SCI0_Init(void)	                                       //SCI0初始化
{
    SCI0BD = 32000000/16/BAUDRATE;    //设置波特率为9600，8000000/16/9600 = 52；
    
        
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

/***************************************************************************
    函数名称：   void send_data_ch0(unsigned char data)
    函数功能：   向发送AD0通道转换完的数据
    调用函数：   
                 
***************************************************************************/
void send_data_ch0(unsigned int data)
{
    
    
    SCI0DRL = 'C';             
    while(!SCI0SR1_TC)                                         // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = 'H'; 
    while(!SCI0SR1_TC)                                         // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = '0';
    while(!SCI0SR1_TC)                                         // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = ':';
    while(!SCI0SR1_TC)                                         // 等待发送完成
    {            
       ;
    } 
    
    SCI0DRL = 0x30 + (data / 1000);
    while(!SCI0SR1_TC)                                          // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data / 100 % 10);
    while(!SCI0SR1_TC)                                          // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data / 10 % 10);
    while(!SCI0SR1_TC)                                          // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data % 10);
    while(!SCI0SR1_TC)                                          // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x0D;                                              // 回车
    while(!SCI0SR1_TC)                                          // 等待发送完成
    {            
       ;
    }
}




/***************************************************************************
    函数名称：   void send_data_ch0(unsigned char data)
    函数功能：   向发送AD1通道转换完的数据
    调用函数：   
                 
***************************************************************************/
void send_data_ch1(unsigned int data)
{
    
    
    SCI0DRL = 'C';             
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = 'H'; 
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = '1';
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    } 
    SCI0DRL = ':';
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    } 
    
    SCI0DRL = 0x30 + (data / 1000);
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data / 100 % 10);
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data / 10 % 10);
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x30 + (data % 10);
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    }
    
    SCI0DRL = 0x0D;                                               // 回车
    while(!SCI0SR1_TC)                                           // 等待发送完成
    {            
       ;
    }
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
功能：接收子程序
说明：
****************************************/
void uart_putchar( unsigned char ch) 
{

    SCI0DRL = ch;	                         // 向缓冲区写入数据
    
    while(!SCI0SR1_TDRE);                  // 等待发送缓冲区空
        
	  
}