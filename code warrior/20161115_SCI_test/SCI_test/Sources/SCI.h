/********************************
SCI.H

MC9S08AW60 SCIģ���ͷ�ļ�
********************************/

#ifndef _SCI_H_
#define _SCI_H_

//***************�궨��*************//
//#define   BAUDRATE   9600                                                 //���ò�����Ϊ9600

//***************��������*************//
extern void SCI_Init(void);
extern void send_data(unsigned char text);
extern void simulation(void);
extern void send_test(void);

#endif