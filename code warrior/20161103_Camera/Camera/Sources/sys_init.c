/***********************************************************
    �ļ�����: sys_init.c
    ˵ ��:   ��ʼ������
***********************************************************/


/***************************************************
** ��������: void sys_init(void)
** ��������: CRG\SCI\TIM��ʼ������
** ˵��:     �Ը�����ģ����г�ʼ������
****************************************************/
#include <hidef.h>     
#include "derivative.h" 
#include  "include.h"

/*****************************��������***********************************/
void sys_init(void) 
{
    TIM_init();         //TIM��ʼ��
   
    SCI0_Init();        //SCI��ʼ��
    
    DDRA = 0x00;        //��IO�������������
   
    DDRB = 0X00;
    DDRT = 0X00;

    init_PIT();    
}


void TIM_init(void)
{   

    TIOS = 0x00;                         //ͨ��ѡ�����벶׽����
    TSCR1 = 0x80;                         //1000 0000,TEN = 1����ʱ����������
   
    TCTL4 = 0x18;                         //0001 1000,ͨ��2Ϊ01�����ز�׽��ͨ��1Ϊ10�½��ز�׽
    TIE = 0x06;                           //0000 0110,����Cn1��Cn2�����ж�����
    TFLG1 = 0xFF;                         //�������б�־λ
      
}


void PA_Init(void)
{
    PACTL=0x40;
    PACNT=0x00;
}

void init_PIT(void)
{
    PITINTE=0X01;
    PITCE=0X01;
    PITMUX= 0x00;
    PITFLT=0X01;
    PITLD0= 4000;

    PITMTLD0=200;
    PITCFLMT = 0x81; 
}

/****************
void zhongduanyouxianji(void)
{
  	INT_CFADDR  = 0x70;		  //PIT0�ж����ȼ���Ϊ7 
  	INT_CFDATA5 = 0x05;
  	INT_CFADDR  = 0xE0;		  //TIM1�����жϣ����ȼ���Ϊ6
  	INT_CFDATA6 = 0x07;
  	INT_CFADDR  = 0xE0;		  //TIM0�����жϣ����ȼ���Ϊ5
  	INT_CFDATA7 = 0x06;	   
}
****************/
