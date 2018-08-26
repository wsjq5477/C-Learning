/********************************
SCI.H

MC9S08AW60 SCI模块的头文件
********************************/

#ifndef _SCI_H_
#define _SCI_H_

//***************宏定义*************//
//#define   BAUDRATE   9600                                                 //设置波特率为9600

//***************函数声明*************//
extern void SCI_Init(void);
extern void send_data(unsigned char text);
extern void simulation(void);
extern void send_test(void);

#endif