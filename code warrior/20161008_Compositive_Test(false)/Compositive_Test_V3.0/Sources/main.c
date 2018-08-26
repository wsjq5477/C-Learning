/******************************************************************************************
    �ļ����ƣ�main.c
    
    ��Ŀ���ƣ�Compositive_Test_V2.0.mpc
    
    ���ߣ�    ����
    
    �汾��    V1.0
    
    ˵����    ��ȷ��ʱһ��,AD˫ͨ�����������������������ʾ,�����յ���λ�����ַ�ʱ����AD����
    
    �������ڣ�2016-10-6
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "SCI.h"
#include "PIT.h"
#include "CRG.h"

/****************************************�궨��*******************************************/
#define  SEG_DD    DDRA                                               //����ڷ���
#define  DIG_DD    DDRT                                               //λ��ڷ���
#define  SEG_NUM   PORTA                                              //��������
#define  DIG_NUM   PTT                                                //λ������

/***************************************��������******************************************/
void system_init(void);
void Num_test(void);
void Num_refresh(void);
void AD_acquisition(void);
void SCI_send(void);
void Delay(unsigned int loop_times);                                  //��ʱ�ӳ���

/*************************************ȫ�ֱ�������****************************************/

const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //������������α���           
unsigned int SM_SBUFF[8];
float convert_value0;
float convert_value1;                                                                                              
unsigned char g_pitflag = 0;
unsigned char g_sciflag = 0; 
    
                                        
/****************************************������*******************************************/

void main(void) 
{       
    system_init();
    
         
    for(;;)
    {  
        if(g_pitflag==1)
        {
            AD_acquisition();
            g_pitflag = 0;
        }
        
        if(g_sciflag==1)
        {
            SCI_send();
            g_sciflag = 0; 
        }
        
        Num_refresh();   
    }
}

/***************************************************************************************
    �������ƣ�   interrupt VectorNumber_Vsci0 void PIT_ISR(void)
    �������ܣ�   PITͨ��0�жϷ������
    ���ú�����   ��
    ��ڲ�����   ��
    ���ڲ�����   ��
    ����ֵ��     ��
    ������  	
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{          
    static unsigned char i = 1;
    
    PITTF_PTF0 = 1;
             
  	i++;                                                     
  	
  	if(i == 10)                                             //ÿ��0.1�룬����10���ж�˵��1sʱ�䵽
  	{   
  	    i = 0;                                              //ÿ��1�룬����־λ����  	                                                                             
        g_pitflag = 1;
    }                         
}
#pragma CODE_SEG DEFAULT

/*******************************************************************************************
    �������ƣ�   interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
    �������ܣ�   �����ж�ʹ��
    ���ú�����   ��
    ��ڲ�����   ��
    ���ڲ�����   ��
    ����ֵ��     ��
    ������  	
*******************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
    if(SCI0SR1_RDRF)
	  {
		    g_sciflag = 1;  
   	}
      	
}
#pragma CODE_SEG DEFAULT


/****************************************
������void system_init(void)
���ܣ�ϵͳ��ʼ��
˵��������GPIO����ģ���ʼ��
****************************************/
void system_init(void)
{                                                                        
    SEG_DD = 0xFF;                                                  //A�˿�����Ϊ�����
    DIG_DD = 0xFF;                                                  //H�˿�����Ϊ�����
    SEG_NUM = 0x00;                                                 //A�˿����ȫ0
    DIG_NUM = 0x00;                                                 //H�˿����ȫ0

    SetBusCLK_64M();                                                //����Ƶ��64M
    AD_Init();                                                      //ADģ���ʼ��
    PIT_init();                                                     //PITģ���ʼ��
    SCI0_Init();                                                    //SCI��ʼ��
    
    Num_test();                                                     //�������ʾ����
    
    EnableInterrupts;                                               //�ж�ʹ��
}

/****************************************
������void delay(unsigned int loop_times)
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

/****************************************
������void Num_test(void)
���ܣ����������
˵���������������ʾ0-7
****************************************/
void Num_test(void)
{  
    unsigned char led_dig;                                         //�����λ��
    unsigned char j;                                                //ѭ������     
    
    led_dig = 0x01; 
    
    for(j=0;j<8;j++) 
    {
       SEG_NUM = g_LED_Seg_Table[j];
       DIG_NUM = led_dig;
       led_dig = led_dig<<1;
       Delay(300);
    }  
}

/****************************************
������void Num_refresh(int SM_SBUFF2)
���ܣ�ˢ�������
˵��������ˢ��8λ�����
****************************************/
void Num_refresh(void)
{  
    unsigned char led_dig;                                          //�����λ��
    unsigned char i;                                                //ѭ������     
    
    led_dig = 0x01; 
    for(i=0;i<8;i++) 
    {
        if(i==3||i==7)
        { 
            SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]]|0x80;               \
            
        }
        else
        {
            SEG_NUM = g_LED_Seg_Table[SM_SBUFF[i]];
        }
        DIG_NUM = led_dig;
        led_dig = led_dig<<1;
        Delay(1);
    }  
}

/****************************************
������void AD_acquisition(void)
���ܣ�AD�ɼ�
˵�����ɼ���·AD���ݲ����
****************************************/
void AD_acquisition(void)
{
    float temp0;
    float temp1;   
    
    ATD0CTL5 = 0x10;                                            //��ͨ�����β�����ͨ��0��1  
    
    while(!ATD0STAT2L_CCF0)                                     //�ȴ�ATD0ת�����
    {
      ; 
    }                                                                         
    convert_value0 = ATD0DR0;                                   //��ȡATD0ת�����
    temp0 = convert_value0*4600/4096;
    
    SM_SBUFF[3] =  (unsigned int)(temp0/1000);                         //�������ֲ�����
    SM_SBUFF[2] =  (unsigned int)(temp0/100%10);                                 
    SM_SBUFF[1] =  (unsigned int)(temp0/10%10);
    SM_SBUFF[0] =  (unsigned int)(temp0%10); 
         
    while(!ATD0STAT2L_CCF1)                                     //�ȴ�ATD1ת�����
    {
      ;
    }              
    convert_value1 = ATD0DR1;                                   //��ȡATD0ת�����
    temp1 = convert_value1*4600/4096;
    
    SM_SBUFF[7] =  (unsigned int)(temp1/1000);                         //�������ֲ�����
    SM_SBUFF[6] =  (unsigned int)(temp1/100%10);                                 
    SM_SBUFF[5] =  (unsigned int)(temp1/10%10);
    SM_SBUFF[4] =  (unsigned int)(temp1%10);    
}

/****************************************
������void SCI_send(void)
���ܣ���������
˵���������յ�������AD���ݷ�������λ��
****************************************/
void SCI_send(void)
{
    unsigned char Rev_data;      
	  
	  if (SCI0SR1_RDRF) 
     {
         Rev_data = SCI0DRL;                                        //��������      
                                
         if(Rev_data == 'A')                                        //������A������ͨ��0��ͨ��1������
         { 
             SCI_senddata0(convert_value0);   
                     
             SCI_senddata1(convert_value1);                      	 
         } 
     }
}
