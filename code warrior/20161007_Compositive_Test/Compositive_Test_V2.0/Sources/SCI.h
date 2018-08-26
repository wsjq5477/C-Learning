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
extern void SCI_senddata0(unsigned int data0);
extern void SCI_senddata1(unsigned int data1);
extern void SCI_putchar( char ch);
extern void SCI_putstr( signed char *p); 
#endif