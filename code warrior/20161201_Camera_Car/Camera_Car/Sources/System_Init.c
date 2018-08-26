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
    PWM_Init();  
    TIM_Init();  
    SCI_Init();
    
    
    DDRT = 0x00; 
    DDRA = 0x00;   
                 
    EnableInterrupts;         
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //ͨ��ѡ�����벶׽����
    TSCR1 = 0x80;                         //1000 0000,TEN = 1����ʱ����������
    TCTL4 = 0x09;                         //0000 1001,ͨ��0Ϊ01�����ز�׽��ͨ��1Ϊ10�½��ز�׽
    TIE = 0x03;                           //0000 0011,����Cn0��Cn1�����ж�����
    TFLG1 = 0xFF;                         //�������б�־λ    
}    
                                                                                                     
void PWM_Init(void)
{
    /***
    PWME=0x00;        //�ж�ʹ�ܹر�
      
    PWMPRCLK=0x30;    //CLOCKBԤ��Ƶ8=64/8=8M
    PWMSCLB=0x04;     //��ʱ��B������Ƶ,CLOCKSB=CLOCKB/(2*4)=8/8=1M=1000000
       
    PWMCLK=0xC0;      //ͨ��6,7ѡ��CLOCKSB
    PWMCTL=0x80;      //ͨ��6,7����
    
    PWMPER67=20000;    //����20000,������Ϊ50Hz(20ms����)
    PWMDTY67=1210;     //�ߵ�ƽ1370us,�м�λ��
      
    �ߵ�ƽΪ1210usʱ��һ��Ϊ�м�λ��;                1370
    �ߵ�ƽΪ1075usʱ��һ��Ϊ�����λ��(��ת);        1100
    �ߵ�ƽΪ1345usʱ��һ��Ϊ���ұ�λ��(��ת);        1530
    
    PWMDTY67=1210;    //ֱ��                    
    PWMDTY67=1075;    //��ת��ͷ
    PWMDTY67=1345;    //��ת��ͷ   
    
    
    PWMPOL=0xFF;      //����ѡ��ʼΪ�ߵ�ƽ
    PWMCAE=0x00;      //ͨ�������                                                      
    PWME = 0x80;
    ****/
    			     
}


           