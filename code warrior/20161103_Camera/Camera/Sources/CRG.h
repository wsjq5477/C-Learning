/********************************
CRG.H

MC9S12XS128 CRGģ���ͷ�ļ�
********************************/
#ifndef _CRG_H
#define _CRG_H


//#define OSC_FREQ_4MHz
//#define OSC_FREQ_8MHz     
#define OSC_FREQ_16MHz       //���徧��Ƶ��

/*****************************��������***********************************/ 
void SetBusCLK_32M(void);                         //��������Ƶ��Ϊ32M
void SetBusCLK_40M(void);                         //��������Ƶ��Ϊ40M
void SetBusCLK_48M(void);                         //��������Ƶ��Ϊ48M
void SetBusCLK_64M(void);                         //��������Ƶ��Ϊ64M
void SetBusCLK_80M(void);                         //��������Ƶ��Ϊ80M

#endif