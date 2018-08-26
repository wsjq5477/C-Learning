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
    PWM_Init();  
                  
    EnableInterrupts; 
    
    DDRT = 0x00; 
    DDRA = 0X00;     
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //ͨ��ѡ�����벶׽����
    TSCR1 = 0x80;                         //1000 0000,TEN = 1����ʱ����������
    TCTL4 = 0x18;                         //0001 1000,ͨ��2Ϊ01�����ز�׽��ͨ��1Ϊ10�½��ز�׽
    TIE = 0x06;                           //0000 0110,����Cn1��Cn2�����ж�����
    TFLG1 = 0xFF;                         //�������б�־λ     
}    

void PWM_Init(void)
{
    PWME=0x00;        //�ж�ʹ�ܹر�
      
    PWMPRCLK=0x30;    //CLOCKBԤ��Ƶ8=64/8=8M
    PWMSCLB=0x04;     //��ʱ��B������Ƶ,CLOCKSB=CLOCKB/(2*4)=8/8=1M=1000000   
    PWMCLK=0xC0;      //ͨ��6,7ѡ��CLOCKSB
    PWMCTL=0x80;      //ͨ��6,7����
    
    PWMPER67=20000;    //����20000,������Ϊ50Hz(20ms����)
    PWMDTY67=1520;     //�ߵ�ƽ1520us,�м�λ��
    
    /****************ռ�ձ�����*********************        
    �ߵ�ƽΪ1520usʱ��һ��Ϊ�м�λ�ã�
    �ߵ�ƽΪ920usʱ��һ��Ϊ�����λ�ã�
    �ߵ�ƽΪ2120usʱ��һ��Ϊ���ұ�λ��
                        
    PWMDTY7=920;      //�ߵ�ƽ920us,�����
    PWMDTY7=2500;     //�ߵ�ƽ2120us,���ұ�   
    **********************************************/
    
    PWMPOL=0xFF;      //����ѡ��ʼΪ�ߵ�ƽ
    PWMCAE=0x00;      //ͨ�������
                                                          
    PWME_PWME7=1;

}

           