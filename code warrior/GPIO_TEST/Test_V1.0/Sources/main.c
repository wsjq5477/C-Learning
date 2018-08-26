/******************************************************************************************
    �ļ����ƣ�Test_V1.0.c
    
    ��Ŀ���ƣ�Test_V1.0.mcp
    
    ���ߣ�    ����
    
    �汾��    V1.0
    
    ˵����    ��ȷ��ʱһ��AAD˫ͨ�����������������������ʾ
    
    �������ڣ�2016-9-26
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "PIT.h"
#include "CRG.h"

/**********************************�궨��*************************************************/
#define  SEG_DD    DDRA                                               //����ڷ���
#define  DIG_DD    DDRH                                               //λ��ڷ���
#define  SEG_NUM   PORTA                                              //��������
#define  DIG_NUM   PTH                                                //λ������

/***********************************��������**********************************************/
void Delay(unsigned int loop_times);                                  //��ʱ�ӳ���
void AD_Init(void);	                                                
void PIT_init(void); 
void SetBusCLK_32M(void);                                             //��������Ƶ��Ϊ32M

/**********************************ȫ�ֱ�������*******************************************/
const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //������������α���                                                
unsigned char g_pitflag = 0;                    //����ȫ�ֱ���
unsigned int convert_value0;
unsigned int convert_value1;
unsigned int SM_SBUFF[8]; 
                                        
/**********************************������*************************************************/
void main(void)
{  
     
     unsigned char led_dig;                                              //�����λѡֵ
     unsigned char i;                                                    //ѭ������
     
     EnableInterrupts;
                                                                         
     SEG_DD = 0xFF;                                                      //A�˿�����Ϊ�����
     DIG_DD = 0xFF;                                                      //H�˿�����Ϊ�����
     SEG_NUM = 0x00;                                                     //A�˿����ȫ0
     DIG_NUM = 0x00;                                                     //H�˿����ȫ0

     SetBusCLK_32M();                                                    //����Ƶ��32M
     AD_Init();                                                          //ADģ���ʼ��
     PIT_init();                                                         //PITģ���ʼ��
     Delay(20);                                                          
        
     for(;;)
     {  
         led_dig = 0x01; 
         for(i=0;i<8;i++) 
         {
             SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]];
             DIG_NUM = led_dig;
             led_dig = led_dig<<1;
             Delay(1);
         } 
         
     }
}


/***************************************************************************************
    ������       interrupt VectorNumber_Vpit0 void PIT_ISR(void)
    ���ܣ�       PITͨ��0�жϷ������
    ˵����
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{                                    
  	PITTF_PTF0 = 1;                                                     //���жϱ�־
  	
  	g_pitflag++;                                                        //g_second_flag��1
  	
  	if(g_pitflag == 10)                                                 //���g_second_flag = 10��˵��1sʱ�䵽
  	{   
  	    g_pitflag = 0;                                                  //��ʱ����Ϊ4M*(1/40M) = 0.1s,����0.1*10 = 1s  	                                                          //g_second_flag   
        
        while(!ATD0STAT2L_CCF0)                                         //�ȴ�ATD0ת�����
        {
          ; 
        }                                                                         
        convert_value0 = ATD0DR0;                                       //��ȡATD0ת�����
        SM_SBUFF[3] =  convert_value0/1000;                             //�������ֲ�����
        SM_SBUFF[2] =  convert_value0/100%10;                                 
        SM_SBUFF[1] =  convert_value0/10%10;
        SM_SBUFF[0] =  convert_value0%10; 
         
        while(!ATD0STAT2L_CCF1)                                         //�ȴ�ATD1ת�����
        {
          ;
        }              
        convert_value1 = ATD0DR1;                                       //��ȡת�����
        SM_SBUFF[7] =  convert_value1/1000;                             //�������ֲ�����
        SM_SBUFF[6] =  convert_value1/100%10;                                 
        SM_SBUFF[5] =  convert_value1/10%10;
        SM_SBUFF[4] =  convert_value1%10;               
    }                      
}
#pragma CODE_SEG DEFAULT

/****************************************
������void delay(void)
���ܣ���ʱ
˵������ʱ500*loop_times����������
****************************************/
void Delay(unsigned int loop_times)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i<loop_times; loop_i++) 
    {
        for (loop_j=0; loop_j<4000;loop_j++) 
        {
            ;
        }
    }
}
