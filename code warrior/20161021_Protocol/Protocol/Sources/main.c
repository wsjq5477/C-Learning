#include <hidef.h>    
#include <string.h> 
#include "derivative.h"    
#include "SCI.h"
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
int read_data(void);
void rev_data(void);
void send_data(unsigned char *value,unsigned int length);
void Delay(unsigned int loop_times);


/*************************************ȫ�ֱ�������****************************************/

unsigned char g_data_flag = 0;
unsigned char g_sciflag = 0;
unsigned char g_txd_buf[20];          //���巢�ͻ�����
unsigned int g_txd_length;            //���ͻ��������ݳ���

unsigned char g_rxd_buf[20];          //������ջ�����
unsigned char g_rxd_busy = 0;         //���ڽ��ջ����������ݱ�־λ�����ڽ��ջ�������Ϊ��ʱΪ1
unsigned char g_rxd_data_flag;        //������Ч��־λ
unsigned int g_rxd_length;            //���ջ��������ݳ���
unsigned int g_rxd_in_pointer;        //���ջ����������ָ�룬ָ����һ���洢��Ԫ
unsigned int g_rxd_out_pointer;       //���ջ�����������ָ�룬ָ����һ���洢��Ԫ 

/****************************************������*******************************************/

void main(void) 
{       
    system_init();
    for(;;)
    {             
        if(g_sciflag==1)
        {
            g_sciflag = 0; 
            rev_data();
            send_data("RIGHT",5);
            if(g_data_flag==1)
            {
                g_data_flag = 0;
                send_data("RIGHT",5);
            }            
        } 
    }
}

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
       
       g_rxd_busy = 1;                                    //���ջ����������ݱ�־λ��1       
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

    SetBusCLK_64M();                                                //����Ƶ��64M
    SCI0_Init();                                                    //SCI��ʼ��
    
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


/*************************************************************
�����vvoid send_data(int data)
����: ���ݷ����ӳ���
˵���������ݴ����������λ��
*************************************************************/
void send_data(unsigned char *value,unsigned int length)
{
    unsigned char i; 

    g_txd_length = length;                 //��¼���ݿ鳤��
    
    SCI0DRL = 0x97;                        //������ʼλ             
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
    
    SCI0DRL = 0x98;                        //���ͽ���λ
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
}

/*************************************************************
�����vvoid read_data(void);
����: ���ݶ�ȡ�ӳ���
˵���������ջ����������ݶ�ȡ����
*************************************************************/
int read_data()
{
    unsigned int temp_data;
    
    if((g_rxd_out_pointer!=g_rxd_in_pointer)&&(g_rxd_out_pointer<20))  //���������ָ�벻���������ָ������Ч����ʾ���ջ������������ݣ�����Ϊ��
    {
        temp_data = g_rxd_buf[g_rxd_in_pointer];                        //�ӽ��ջ�������ȡһ������  
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
    
    unsigned int tempdata;
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
       
        if(tempdata==0x97)                          //���֡ͷ�Ƿ���ȷ
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
            else if(tempdata==0x98)                 //�ж�֡β�Ƿ���ȷ
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







