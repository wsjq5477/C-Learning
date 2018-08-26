/********************************
SCI.H

MC9S08AW60 SCI模块的头文件
********************************/

#ifndef _SCI_H_
#define _SCI_H_

//***************宏定义*************//
#define   BAUDRATE   9600                                                 //设置波特率为9600

//***************函数声明*************//
extern void SCI0_Init(void);
extern void send_data_ch0(unsigned int data);
extern void send_data_ch1(unsigned int data);
extern void uart_putchar( unsigned char ch); 
#endif