/********************************
SCI.H

MC9S08AW60 SCIģ���ͷ�ļ�
********************************/

#ifndef _SCI_H_
#define _SCI_H_

//***************�궨��*************//
#define   BAUDRATE   9600                                                 //���ò�����Ϊ9600

//***************��������*************//
extern void SCI0_Init(void);
extern void SCI_senddata0(unsigned int data0);
extern void SCI_senddata1(unsigned int data1);
extern void SCI_putchar( char ch);
extern void SCI_putstr( signed char *p); 
#endif