/***************************************************************************
    �ļ�����: Image.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-04
    
    ˵��:     ͼ��ɼ���ͼ�����ӳ���

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Image.h"
#include "Route.h"
#include "SCI.h"

/*********************�궨��******************************/
#define lie_end  260
#define hang_end 91
#define lie  52   

/*********************��������******************************/
extern unsigned char black_line[40];
extern unsigned char left_edge[40];
extern unsigned char right_edge[40];

unsigned char image[40][90];
unsigned char c_lie=0,g_lie=0;
unsigned char lie_count=0,hang_count=0; 
unsigned char sdata; 

unsigned int get_n[]={  16, 29, 41, 52, 62, 71, 79, 86, 92, 98,     //ͼ�񶨾�ɼ���Ӧ������ͷ����
                       103,108,112,116,120,124,128,132,135,138,
                       141,144,147,150,153,156,159,162,165,168,
                       170,172,174,176,178,180,182,184,186,188,
                       190,192,194,196,198,200,202,204,206,208,
                       210,211,212,242,243,245,246,247,248,249,250};  
                       
/*************************************************************
������void delay(unsigned int num);
���ܣ���ʱ����
˵��������64M���ߵ�ȥ������ʱ
*************************************************************/                       
void delay(unsigned int num)   
{ 
    unsigned int i,j; 
    for(i=0;i<num;i++) 
    {
       for(j=0;j<58;j++); 
    }
} 

/*************************************************************
������void send_image(void);
���ܣ�����ͼ������������
˵������������ͷʱʹ��
*************************************************************/ 
void send_image()
{
    unsigned char i,j;
    unsigned int ts1,ts2;
    
    delay(1700);
    
    for(i=1;i<40;i++)         
    {                            
        for(j=1;j<90;j++)                      
        {           
            ts1 = 0;
            ts2 = 0;
                   
            if(image[i][j]==1&&image[i][j+1]==1)    //0x11=17;0x10=16;0x01=1;0x00=0
            {
                ts1 = 16;          
            }
            
            j++;
            
            if(image[i][j]==1&&image[i][j+1]==1) 
            {
                ts2 = 1;           
            }
            
            sdata = ts1+ts2;
        
            send_data(sdata); 
            
             
        }
    }  
    for(;;)     //����ʱ������һ��ͼ,������������踴λ����
    {
        ;
    }       
}

/*************************************************************
������void send_line(void);
���ܣ����ʹ����ĺ��ߺ����ұ���������������
˵����
*************************************************************/ 
void send_line()
{
    unsigned char i,j;
    
    delay(1700);
    
    route_extract();                 //������ȡ
    
    for(i=1;i<40;i++)         
    {                            
        for(j=1;j<90;j++)                      
        {    
                  
            sdata = 0;
                   
            if(black_line[i]==j||black_line[i]==j+1)    //0x11=17;0x10=16;0x01=1;0x00=0
            {
                sdata = 17;          
            }
            /***
            if(left_edge[i]==j||left_edge[i]==j+1)
            {
                sdata = 17;
            }
            if(right_edge[i]==j||left_edge[i]==j+1)
            {
                sdata = 17;
            }
            ***/
            j++;
            
            send_data(sdata);  
        }
    }  
    for(;;) 
    {
        ;
    }       
}

/************************************************** 
��������: ���ڷ���˳���
��������: ����ͼ��ɼ����ص�DEMOKtool
˵����    �ڷ���ͼ������ǰҪ����Э�鷢��0XFFͼ��ͷ
***************************************************/
void send_image_PC(void) 
{
    unsigned char i,j;
    SCI0DRH = 0;
    SCI0DRL = 0xFF;
    
    while(!(SCI0SR1&0x80)) 
    {
        ;
    }
     for(i=0; i<40; i++) 
    {
        for(j=0; j<90; j++) 
        {
            SCI0DRL = image[i][j];
            while(!(SCI0SR1&0x80));                
        }    
    }      
}
/*************************************************************
�������жϴ�����
���ܣ����жϴ���
˵����OV5116��4��D1Ϊ��ֵ�����,��PA0;5��HSΪ���ж�,��PT1;
*************************************************************/
#pragma CODE_SEG NON_BANKED
void interrupt 10 IC2ISR(void)     //�������ж�
{  
    TFLG1_C2F=1;                   //�����־λ               
    if(lie_count==get_n[c_lie])    //�����������õ���ɼ����������
    {
        delay(1);                  //����
        for(hang_count=0;hang_count<=hang_end;hang_count++)
        {  
            image[c_lie][hang_count] = PORTA_PA0;   //�����д�������     
        }
        c_lie++;                   
    }
    lie_count++;                   //�ɼ���������������1
}

/*************************************************************
�������жϴ�����
���ܣ����жϴ���
˵����OV5116��6��VSΪ���ж�,��PT0
*************************************************************/
void interrupt 9 IC1ISR(void)      //���볡�ж�
{              
    TFLG1_C1F = 1;                 //�����־λ
    hang_count = 0;                //��������������
    lie_count = 0;
    c_lie = 0;    
}  

#pragma CODE_SEG DEFAULT 
 