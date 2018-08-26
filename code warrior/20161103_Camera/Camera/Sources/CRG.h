/********************************
CRG.H

MC9S12XS128 CRG模块的头文件
********************************/
#ifndef _CRG_H
#define _CRG_H


//#define OSC_FREQ_4MHz
//#define OSC_FREQ_8MHz     
#define OSC_FREQ_16MHz       //定义晶振频率

/*****************************函数声明***********************************/ 
void SetBusCLK_32M(void);                         //设置总线频率为32M
void SetBusCLK_40M(void);                         //设置总线频率为40M
void SetBusCLK_48M(void);                         //设置总线频率为48M
void SetBusCLK_64M(void);                         //设置总线频率为64M
void SetBusCLK_80M(void);                         //设置总线频率为80M

#endif