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
    PAC_Init(); 
    SCI0_Init();
    PWM_Init();
    PIT_Init();  
    
    DDRT = 0x00; 
    DDRA = 0x00;   
                 
    EnableInterrupts;         
}

void TIM_Init(void)
{   
    TIOS = 0x00;                          //ͨ��ѡ�����벶׽����
    TSCR1 = 0x80;                         //1000 0000,TEN = 1����ʱ����������
    TCTL4 = 0xA9;                         //1010 1001,ͨ��2��3λ�����ز�׽��ͨ��0Ϊ01�����ز�׽��ͨ��1Ϊ10�½��ز�׽
    TIE = 0x0F;                           //0000 1111,����Cn3��Cn2��Cn0��Cn1�����ж�����
    TFLG1 = 0xFF;                         //�������б�־λ    
}    

void PAC_Init(void)
{}
                                                                                                     
void PWM_Init(void)
{
    PWME=0x00;        //�ж�ʹ�ܹر�
    
    DDRP_DDRP2 = 1;
    PTP_PTP2 = 0; 
        
    PWMPRCLK=0x33;    //CLOCKA,BԤ��Ƶ8=64/8=8M
    
    PWMSCLA=0x04;
    PWMSCLB=0x04;     //��ʱ��B������Ƶ,CLOCKSB=CLOCKB/(2*4)=8/8=1M=1000000
    PWMCTL_CON67 = 1;
       
    PWMCLK_PCLK7 = 1;
    PWMCLK_PCLK0 = 1;
    PWMCLK_PCLK1 = 1; //ͨ��0,1ѡ��CLOCKSA;ͨ��6,7ѡ��CLOCKSB
    
    PWMPER67=20000;   //����20000,������Ϊ50Hz(20ms����)
    PWMPER0 = 200;    //�����ת
    PWMPER1 = 200;    //�����ת
    
    PWMDTY67=1210;    //�ߵ�ƽ1210us,�м�λ��
    PWMDTY0 = 0;
    PWMDTY1 = 50;      //ռ�ձ�25%    
    
    PWMPOL=0xFF;      //����ѡ��ʼΪ�ߵ�ƽ
    PWMCAE=0x00;      //ͨ�������
                                                          
    PWME_PWME7= 1;
    PWME_PWME0= 1;
    PWME_PWME1= 1;     
}
/****************************************
������void PIT_init(void)
���ܣ���ʼ��PIT�����þ�ȷ��ʱʱ��Ϊ10ms
˵����
****************************************/
void PIT_Init(void)
{

    PITCFLMT = 0x00;       //��ֹPITģ��
 
    PITCE_PCE0 = 1;        //ʹ�ܶ�ʱ��ͨ��0

    PITMUX = 0x00;        //��ʱ��ͨ��0ʹ��Base Timer0 (΢������0) 
                          
    /* PITMTLD0 = (PMTLD7~PMTLD0)   */
    PITMTLD0 = 0x31;      
    /* PITLD0 = (PLD15~PLD0)   */                        
    PITLD0 = 0x31FF;       //����16λ�������ļ��ؼĴ���
                           /*  PITMTLD0��PITLD0��ͬ������ʱ��ͨ��0�Ķ�ʱ����
                            *  ��ʱ����Ϊ($PITMTLD0+1)*($PITLD0+1)���ڲ�����ʱ������
                            *  ���($PITMTLD0+1)*($PITLD0+1) = 0.64M,����ʱ����Ϊ4M���ڲ�����ʱ������
                            *  �ڲ�����ʱ������Ϊ1/64M�����Զ�ʱ����Ϊ0.64M*(1/64M) = 0.01s
                            */
    PITINTE = 0x01;         //ʹ��PITͨ��0���ж�
                         
  
    PITCFLMT = 0x80;        //ʹ��PITģ��
}
           