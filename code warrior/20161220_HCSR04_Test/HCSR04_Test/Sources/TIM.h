/********************************
TIM.H

MC9S08AW60 TIM模块的头文件
********************************/

#ifndef _TIM_H_
#define _TIM_H_
                                          
//***************函数声明*************//
extern void TIM_Init(void);
extern void Measure(void);
extern void Data_split(unsigned int value); 

#endif