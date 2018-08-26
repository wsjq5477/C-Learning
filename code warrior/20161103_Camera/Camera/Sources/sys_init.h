/********************************
sys_init.H

MC9S08AW60 sys_init0模块的头文件
********************************/

#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_

//***************函数声明*************//
extern void sys_init(void);

extern void TIM_init(void); 

void PA_Init(void);
void init_PIT(void);
//void zhongduanyouxianji(void);

#endif