/********************************
SCI.H

MC9S08AW60 SCIģ���ͷ�ļ�
********************************/

#ifndef _SCI_H_
#define _SCI_H_

//***************�궨��*************//
#define   BAUDRATE   9600    

//***************��������*************//
extern void SCI0_Init(void);
unsigned char SCI0_rec_dat(void);
extern void uart_putchar(unsigned char ch);
extern void putstr(char ch[]);

#endif