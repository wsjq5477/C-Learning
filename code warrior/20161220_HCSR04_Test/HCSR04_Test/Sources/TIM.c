/***************************************************************************
    �ļ�����: TIM.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-12-20
    
    ˵��:     TIMģ��ĳ�ʼ�����жϷ����ӳ���

    �޸ļ�¼: 
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "TIM.h"

/******************************��������************************************/
void TIM_Init(void);
void Measure(void);
void Data_split(unsigned int value);

/*************************ȫ�ֱ�������**********************************/
unsigned char echo_flag = 0;             //������ϱ�־λ
unsigned int temp_count = 0;
unsigned int range = 0;
unsigned char SM_SBUFF[4];

/*************************************************************
������void TIM_Init(void);
���ܣ�TIM��ʼ��
˵����
*************************************************************/ 
void TIM_Init(void)
{   
    TSCR1 = 0x90;
    TIOS  = 0x00;
    PACTL_PAEN = 0;     //�ر������ۼ���ʹ��
    PACNT = 0x0000;   
    PACTL_PAMOD = 1;    //�ſ�ʱ���ۼ�
    PACTL_PEDGE = 0;    //�ߵ�ƽ����
    PACTL_CLK1 = 0;     //64��Ƶ,PACLK=1MHz
    PACTL_CLK0= 1;
    PACTL_PAI = 1;                                                                           
    PACTL_PAEN = 1;     //�������ۼ���ʹ��
}   


/**************************************************************************************
������void Measure(void) 
���ܣ����㳬����ģ������ľ���
˵����
***************************************************************************************/
void Measure(void) 
{                                                                           
    if(echo_flag==1)
    {           
        range = (temp_count/10)*34/10+40;                //��λΪmm,20mm-4000mm
    }   	
    Data_split(range);
}

/****************************************
������void Data_split(void)
���ܣ����ݲ��
˵������4λ�����Ϊ��4�����ݲ������������
****************************************/
void Data_split(unsigned int value)
{   
    unsigned int temp;
    temp = value;
                                
    SM_SBUFF[3] =  48+temp/1000;                  //�������ֲ�����
    SM_SBUFF[2] =  48+temp/100%10;                                 
    SM_SBUFF[1] =  48+temp/10%10;
    SM_SBUFF[0] =  48+temp%10; 
}

/******************************�жϷ����ӳ���************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED  
interrupt VectorNumber_Vtimpaie void ECT_0_ISR(void)    //PT7�ж�
{    
    PAFLG_PAIF = 1;
    
    temp_count = PACNT;                                //һ��1us
    echo_flag = 1;
    
    PACNT = 0;    
}                                     
#pragma CODE_SEG DEFAULT 