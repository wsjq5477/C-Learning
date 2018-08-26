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

extern unsigned char g_keyflag;
extern unsigned char ch0_A[4];
extern unsigned char ch1_A[4];

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





