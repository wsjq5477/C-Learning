/********************************
sys_init.H

MC9S08AW60 sys_init0ģ���ͷ�ļ�
********************************/

#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_

//***************��������*************//
extern void sys_init(void);

extern void TIM_init(void); 

void PA_Init(void);
void init_PIT(void);
//void zhongduanyouxianji(void);

#endif