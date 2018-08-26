/***************************************************************************
    �ļ�����: System_Init.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-04
    
    ˵��:     ���ϵͳ�ĳ�ʼ��

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "System_Init.h"
#include "SCI.h"
#include "CRG.h"

void Sys_Init(void) 
{
    DisableInterrupts; 
    
    SetBusCLK_64M();
    TIM_Init();  
    SCI_Init();
    
    EnableInterrupts; 
    
    DDRT = 0x00; 
    DDRA = 0x00;     
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //ͨ��ѡ�����벶׽����
    TSCR1 = 0x80;                         //1000 0000,TEN = 1����ʱ����������
    TCTL4 = 0x0A;                         //0000 1001,ͨ��0Ϊ01�����ز�׽��ͨ��1Ϊ10�½��ز�׽
    TIE = 0x03;                           //0000 0011,����Cn0��Cn1�����ж�����
    TFLG1 = 0xFF;                         //�������б�־λ     
}    


           