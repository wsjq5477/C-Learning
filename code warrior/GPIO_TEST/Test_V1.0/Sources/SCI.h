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
extern unsigned char SCI0_rec_dat(void);
extern void uart_putchar( unsigned char ch); 
extern void send_data_ch0(unsigned int data);
extern void send_data_ch1(unsigned int data);


#endif