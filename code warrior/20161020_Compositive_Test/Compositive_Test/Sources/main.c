/******************************************************************************************
    �ļ����ƣ�main.c
    
    ��Ŀ���ƣ�Compositive_Test_V3.0.mpc
    
    ���ߣ�    ����
    
    �汾��    V1.0
    
    ˵����    1.��ȷ��ʱһ��;
              2.AD˫ͨ��ÿ��һ�����һ��;
              3.�Բ������ݽ��������˲����������ѹֵ;
              4.���뿪�ؿ����������ʾ��ͬͨ���ĵ�ѹֵ�Ͳɼ���������;
              5.�����յ���λ�����ַ�ʱ���͵�ѹֵ�Ͳɼ���������(ʹ��ͨ��Э��);
    
    �������ڣ�2016-10-21
******************************************************************************************/

#include <hidef.h>    
#include "derivative.h"     
#include "AD.h"
#include "SCI.h"
#include "PIT.h"
#include "PWM.h"
#include "CRG.h"

/****************************************�궨��*******************************************/
#define  SEG_DD    DDRA                                               //����ڷ���
#define  DIG_DD    DDRT                                               //λ��ڷ���
#define  SEG_NUM   PORTA                                              //��������
#define  DIG_NUM   PTT                                                //λ������
#define  SW_DD     DDRB_DDRB4                                         //���뿪�����ݷ���Ĵ�������
#define  SW        PORTB_PB4                                          //���뿪�ؿ���λI/O�ڶ���

/***************************************��������******************************************/
void system_init(void);

void Num_test(void);
void Num_refresh(void);

void AD_acquisition(void);
void Data_split(unsigned int value0,unsigned int value1);
void Filtering(void);

void PWM_output(unsigned int value0,unsigned int value1);

unsigned char read_data(void);
void rev_data(void);
void send_data(unsigned char *value,unsigned char length);
void send_value();

void Delay(unsigned int loop_times);                                  //��ʱ�ӳ���

/*************************************ȫ�ֱ�������****************************************/

unsigned char const g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //������������α���           
unsigned int convert_value0;
unsigned int convert_value1;
unsigned int filtering_D_value0 = 0;
unsigned int filtering_D_value1 = 0;
unsigned int filtering_A_value0 = 0;
unsigned int filtering_A_value1 = 0;
    
unsigned int value0_buf[10];
unsigned int value1_buf[10];

unsigned char ch0_D[4] = 0;           //�ֱ�������ͨ������������ģ����
unsigned char ch1_D[4] = 0; 
unsigned char ch0_A[4] = 0;
unsigned char ch1_A[4] = 0;
                                                                                          
unsigned char g_pitflag = 0;
unsigned char g_sciflag = 0;
unsigned char g_keyflag = 0;
unsigned char Rev_data; 

unsigned char g_data_flag = 0;
unsigned char g_txd_buf[20];          //���巢�ͻ�����
unsigned char g_txd_length;           //���ͻ��������ݳ���

unsigned char g_rxd_buf[20];          //������ջ�����
unsigned char g_rxd_busy = 0;         //���ڽ��ջ����������ݱ�־λ�����ڽ��ջ�������Ϊ��ʱΪ1
unsigned char g_rxd_data_flag;        //������Ч��־λ
unsigned char g_rxd_length;           //���ջ��������ݳ���
unsigned char g_rxd_in_pointer;       //���ջ����������ָ�룬ָ����һ���洢��Ԫ
unsigned char g_rxd_out_pointer;      //���ջ�����������ָ�룬ָ����һ���洢��Ԫ 

/****************************************������*******************************************/

void main(void) 
{       
    unsigned char sw;
    
    system_init();
             
    for(;;)
    {  
        sw = SW;                       
        if(sw==1)                       //�жϲ��뿪��״̬
        {
            g_keyflag = 0;
        }
        else
        {
            g_keyflag = 1;
        }
        
        if(g_pitflag==1)                //PIT��־λΪ1�����AD����
        {
            AD_acquisition();
            g_pitflag = 0;
        }
        
        if(g_sciflag==1)
        {
            g_sciflag = 0; 
            rev_data();
            if(g_data_flag==1)
            {
                g_data_flag = 0;            
            }           
        }
   
        Num_refresh(); 
                                               
        PWM_output(convert_value0,convert_value1);  
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
    PITTF_PTF0 = 1;
    
    g_pitflag = 1;                             
}
#pragma CODE_SEG DEFAULT

/***************************************************************************************
    �������ƣ�   interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
    �������ܣ�   �����ж�ʹ��
    ���ú�����   ��
    ��ڲ�����   ��
    ���ڲ�����   ��
    ����ֵ��     ��
    ������  	
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
    if (SCI0SR1_RDRF) 
    {
       g_sciflag = 1;
              
       g_rxd_buf[g_rxd_in_pointer] = SCI0DRL;              //�����ݶ�����ջ�����    
       g_rxd_in_pointer++;                                 //�����ָ���1��ָ����һ���洢��Ԫ
       if(g_rxd_in_pointer==20)                            //��������ָ�볬���˻�����β��ַ����ָ���׵�ַ
       {
           g_rxd_in_pointer = 0;
       }
       
       g_rxd_busy = 1;                                     //���ջ����������ݱ�־λ��1       
    }
    
}
#pragma CODE_SEG DEFAULT


/*************************************************************
������void system_init(void)
���ܣ�ϵͳ��ʼ��
˵��������GPIO����ģ���ʼ��
*************************************************************/
void system_init(void)
{                                                                        
    SEG_DD = 0xFF;                                                  //A�˿�����Ϊ�����
    DIG_DD = 0xFF;                                                  //H�˿�����Ϊ�����
    SEG_NUM = 0x00;                                                 //A�˿����ȫ0
    DIG_NUM = 0x00;                                                 //H�˿����ȫ0
    SW_DD = 0;                                                      //PB4Ϊ�����     

    SetBusCLK_64M();                                                //����Ƶ��64M
    AD_Init();                                                      //ADģ���ʼ��
    PIT_init();                                                     //PITģ���ʼ��
    PWM_init();                                                     //PWM��ʼ��
    SCI0_Init();                                                    //SCI��ʼ��
   
    Num_test();                                                     //�������ʾ����
    
    EnableInterrupts;                                               //�ж�ʹ��
}

/*************************************************************
������void delay(unsigned int loop_times)
���ܣ���ʱ
˵������ʱ500*loop_times����������
*************************************************************/
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

/*************************************************************
������void Num_test(void)
���ܣ����������
˵���������������ʾ0-7
*************************************************************/
void Num_test(void)
{  
    unsigned char led_dig;                                          //�����λ��
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

/*************************************************************
������void Num_refresh(void)
���ܣ�ˢ�������
˵�����жϲ��뿪�ص�״̬��ѡ��ͨ��ˢ��8λ�����
*************************************************************/
void Num_refresh(void)
{  
    unsigned char led_dig;                                          //�����λ��
    unsigned char i;
    unsigned char ch[8];
    
    led_dig = 0x01; 
    
    if(g_keyflag==0)                                                //��keyflag==0������ʾͨ��0��ֵ
    {
        for(i=0;i<4;i++)                                            //����λΪ��ѹת��ֵ������λΪ������
        {
            ch[i] = ch0_D[i];
        }
        for(i=0;i<4;i++)          
        {
            ch[i+4] = ch0_A[i];
        }      
    }
    else                                                            //��keyflag==1,����ʾͨ��1��ֵ                                                      
    {
        for(i=0;i<4;i++)          
        {
            ch[i] = ch1_D[i];
        }
        for(i=0;i<4;i++)          
        {
            ch[i+4] = ch1_A[i];
        }
    }
                                                                    
    for(i=0;i<8;i++)                                                //����ˢ�������
    {
        if(i==7)
        { 
            SEG_NUM = g_LED_Seg_Table[ch[i]]|0x80;                  //�ڵ���λ(����ѹת��ֵ����λ)��ʾС����    
        }
        else
        {
            SEG_NUM = g_LED_Seg_Table[ch[i]];
        }
        SEG_NUM = g_LED_Seg_Table[ch[i]];
        DIG_NUM = led_dig;
        led_dig = led_dig<<1;
        Delay(1);       
    }  
}

/*************************************************************
������void AD_acquisition(void)
���ܣ�AD�ɼ�
˵�����ɼ���·AD����
*************************************************************/
void AD_acquisition(void)
{     
    PITCFLMT = 0x00;                                            //����PITģ��
  
    ATD0CTL5 = 0x10;                                            //��ͨ�����β�����ͨ��0��1  
    
    while(!ATD0STAT2L_CCF0)                                     //�ȴ�ATD0ת�����
    {
      ; 
    }                                                                         
    convert_value0 = ATD0DR0;                                   //��ȡATD0ת�����
    
    while(!ATD0STAT2L_CCF1)                                     //�ȴ�ATD1ת�����
    {
      ;
    }              
    convert_value1 = ATD0DR1;                                   //��ȡATD0ת����� 
   
    Filtering();                                                //�����˲�
        
    Data_split(filtering_D_value0,filtering_D_value1);          //�������
    
    PITCFLMT = 0x80;                                            //ʹ��PITģ��
}

/************************************************************
������void Filtering(unsigned int value0,unsigned int value1);
���ܣ������˲�
˵����������ͨ�������������������˲�
************************************************************/
void Filtering(void)
{
    static unsigned char count = 0;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned int temp;
  
    value0_buf[count] = convert_value0;
    value1_buf[count] = convert_value1;
      
    count++;   
     
    if(count==10)
    {
        count = 0;
        for(j=0;j<9;j++)                                   //��ͨ��0�˲�
        {
            for(i=0;i<9-j;i++)
            {
                if(value0_buf[i]>value0_buf[i+1])          //ð�ݷ�����
                {
                    temp = value0_buf[i];
                    value0_buf[i] = value0_buf[i+1];
                    value0_buf[i+1] = temp;
                }
            }
        }
        filtering_D_value0 = (value0_buf[4]+value0_buf[5])/2;
        
        for(j=0;j<9;j++)                                   //��ͨ��1�˲�
        {            
            for(i=0;i<9-j;i++)
            {             
                if(value1_buf[i]>value1_buf[i+1])
                {
                    temp = value1_buf[i];
                    value1_buf[i] = value1_buf[i+1];
                    value1_buf[i+1] = temp;
                }
            }
        } 
        filtering_D_value1 = (value1_buf[4]+value1_buf[5])/2;
    }
}
/************************************************************
������Data_split(convert_value0,convert_value1); 
���ܣ����ݲ��
˵����������ͨ���������������ѹת��ֵ��ֺ�����4��������
************************************************************/
void Data_split(unsigned int value0,unsigned int value1)
{
    ch0_D[3] = value0/1000;                                           //���ͨ��0������������������
    ch0_D[2] = value0/100%10;
    ch0_D[1] = value0/10%10;
    ch0_D[0] = value0%10;         
                                      
    filtering_A_value0 = (unsigned int)(((long)value0)*4600/4096);    //���ͨ��0�ĵ�ѹת��ֵ����������
    ch0_A[3] = (unsigned char)(filtering_A_value0/1000);                     
    ch0_A[2] = (unsigned char)(filtering_A_value0/100%10);                                 
    ch0_A[1] = (unsigned char)(filtering_A_value0/10%10);
    ch0_A[0] = (unsigned char)(filtering_A_value0%10); 
    
    ch1_D[3] = value1/1000;                                           //���ͨ��1������������������
    ch1_D[2] = value1/100%10;
    ch1_D[1] = value1/10%10;
    ch1_D[0] = value1%10;   
    
    filtering_A_value1 = (unsigned int)(((long)value1)*4600/4096);
    ch1_A[3] = (unsigned char)(filtering_A_value1/1000);              //���ͨ��0�ĵ�ѹת��ֵ����������
    ch1_A[2] = (unsigned char)(filtering_A_value1/100%10);                                 
    ch1_A[1] = (unsigned char)(filtering_A_value1/10%10);
    ch1_A[0] = (unsigned char)(filtering_A_value1%10);   
}

/************************************************************
������void PWM_output(unsigned int value0,unsigned int value1)
���ܣ���������
˵���������յ�������AD���ݷ�������λ��
************************************************************/
void PWM_output(unsigned int value0,unsigned int value1)
{
    unsigned long temp_dty;
    
    temp_dty = (value0+value1)*100/4096;               //����ѹ����Ϊռ�ձ�
    
    PWMDTY1 = (unsigned char)(temp_dty);    
}

/*************************************************************
�����vvoid send_data(int data)
����: ���ݷ����ӳ���
˵���������ݴ����������λ��
*************************************************************/
void send_data(unsigned char *value,unsigned char length)
{
    unsigned char i; 

    g_txd_length = length;                 //��¼���ݿ鳤��
    
    SCI0DRL = '@';                         //������ʼλ             
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
    
    SCI0DRL = g_txd_length;                //�������ݳ���            
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
    
    for(i=0;i<length;i++)                  //�������ݿ�
    {
        SCI0DRL = value[length-1];
        while(!SCI0SR1_TC)                        
        {            
            ;
        }     
    }
    
    SCI0DRL = '$';                         //���ͽ���λ
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
}

/*************************************************************
�����vvoid send_value();
����: �������ݷ����ӳ���
˵����������ͨ������������ģ�����ֱ�������λ��
*************************************************************/
void send_value()
{
    send_data(" ch0_D = ",9);
    send_data(ch0_D,4);
    
    send_data(" ch0_A = ",9);
    send_data(ch0_A,4);
    
    send_data(" ch1_D = ",9);
    send_data(ch1_D,4);
    
    send_data(" ch1_A = ",9);
    send_data(ch1_A,4);         
}


/*************************************************************
�����vvoid read_data(void);
����: ���ݶ�ȡ�ӳ���
˵���������ջ����������ݶ�ȡ����
*************************************************************/
unsigned char read_data()
{
    unsigned char temp_data;
    
    if((g_rxd_out_pointer!=g_rxd_in_pointer)&&(g_rxd_out_pointer<20))  //���������ָ�벻���������ָ������Ч����ʾ���ջ������������ݣ�����Ϊ��
    {
        temp_data = g_rxd_buf[g_rxd_in_pointer];                       //�ӽ��ջ�������ȡһ������  
        g_rxd_out_pointer++;                                           //������ָ���1��ָ����һ���洢��Ԫ
    
        if(g_rxd_out_pointer!=g_rxd_in_pointer)                        //���������ָ����������ָ�룬��ʾ���ջ������ѿ�
        {
            g_rxd_busy = 0;                                            //���ջ�����Ϊ��
        } 
       
        if(g_rxd_out_pointer==20)                                      //���������ָ�볬���˻�����β��ַ����ָ���׵�ַ
        {
            g_rxd_out_pointer = 0;
        } 
        
        return temp_data;
    }  
    else
    {                                                                  
        g_rxd_busy = 0;                                                //���ջ�������
        g_rxd_data_flag = 0;                                           //������Ч
    }
}

/*************************************************************
�����vvoid rev_data(void);
����: ���ݽ����ӳ���
˵�������ջ�����������
*************************************************************/
void rev_data()
{
    static unsigned char i;
    
    unsigned char tempdata;
    unsigned int datalength; 
    unsigned char rev_data[20];
    unsigned char revflag = 0;
    unsigned char startflag = 0;
    unsigned char dataflag = 0;
    
    
    
    tempdata = read_data();                         //��ȡ����������
    if(g_rxd_busy!=0)                               //���ջ�����������
    {
        if(g_rxd_data_flag!=0)                      //������Ч
        {
            revflag = 1; 
            rev_data[i] = tempdata;  
        }     
    }
    
    if(revflag==1)
    {
        revflag = 0;
       
        if(tempdata=='@')                           //���֡ͷ�Ƿ���ȷ
        {
            i = 1;                                  //��ȷ����һ֡��ʼ�������ݳ���
            startflag = 1; 
        }
        
        if(startflag==1)                            
        {
            datalength = tempdata;                  //�������ݳ���
            i = 2; 
            dataflag = 1;                           //��һ֡��ʼ��������
        }
        
        if(dataflag==1)            
        {
            if(i<datalength+2)                      //�жϽ������ݰ��ĳ����Ƿ���Ԥ����Χ��
            {
                rev_data[i] = tempdata;             //������������
                i++;
            }
            else if(tempdata=='$')                  //�ж�֡β�Ƿ���ȷ
            {
                if(rev_data[2]=='A')                //�˴��ж������Ƿ�Ϊ'A',��ͬ������Ը����ж�����
                {
                    g_data_flag = 1;
                }
            }   
            else
            {
                send_data("ERROR",5);
            }
        }       
    }    
}

