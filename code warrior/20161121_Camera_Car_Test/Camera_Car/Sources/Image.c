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
extern unsigned char black_line[40];       //��������
unsigned char image_two[40][90];    

unsigned char image[40][90];               //��������
unsigned char c_lie=0,g_lie=0;
unsigned char lie_count=0,hang_count=0; 
 

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

void binaryzation(void)
{        
    unsigned char i,j;
    
    for(i=0;i<40;i++)
    {
        for(j=0;j<90;j++)
        {
            if(image[i][j]<0x60)
            {
                image_two[i][j]=1;
            }
            else 
            {
                image_two[i][j]=0;
            }
        }
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
    unsigned char sdata;   
    delay(10000);           //�ȴ�ͼ��ɼ���ϲſɷ������� 
    
    binaryzation();
    
    for(i=0;i<40;i++)         
    {                            
        for(j=0;j<90;j++)                      
        {     
                              
            //**************��ֵ��������ͼ��************************                 
            if(image_two[i][j]==1&&image_two[i][j+1]==1)
            {
                sdata = 17;
            }
            else if(image_two[i][j]==1&&image_two[i][j+1]==0)
            {
                sdata = 16;
            }
            else if(image_two[i][j]==0&&image_two[i][j+1]==1)
            {
                sdata = 1;
            }
            else
            { 
                sdata = 0;
            }
           
            j++; 
            
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
    unsigned char sdata;
    
    delay(10000);
    
    binaryzation();          //��ֵ������
    
    route_extract();         //������ȡ
       
    for(i=0;i<40;i++)         
    {                            
       for(j=0;j<90;j++)
       {
           if(j==black_line[i])
           {
               sdata = 17;
           }
           else if((j+1)==black_line[i])
           {
               sdata = 17;
           }
           else
           {
               sdata = 0;
           }
           j++;
           
           send_data(sdata);
       }   
    }  
    for(;;) 
    {
        ;
    }       
}
/*************************************************************
�������жϴ�����
���ܣ����жϴ���
˵����OV5116��4��D1Ϊ��ֵ�����,��PA0;5��HSΪ���ж�,��PT2;
*************************************************************/
#pragma CODE_SEG NON_BANKED
void interrupt 8 IC2ISR(void)     //�������ж�
{  
    TFLG1_C2F=1;                   //�����־λ               
    if(lie_count==get_n[c_lie])    //�����������õ���ɼ����������
    {
        delay(1);                  //����
        for(hang_count=0;hang_count<=hang_end;hang_count++)
        {  
            image[c_lie][hang_count] = PORTB;   //�����д�������     
        }
        c_lie++;                   
    }
    lie_count++;                   //�ɼ���������������1
}

/*************************************************************
�������жϴ�����
���ܣ����жϴ���
˵����OV5116��6��VSΪ���ж�,��PT1
*************************************************************/
void interrupt 9 IC1ISR(void)      //���볡�ж�
{              
    TFLG1_C1F = 1;                 //�����־λ
    TFLG1_C2F=1;                   //�����־λ 
    hang_count = 0;                //��������������
    lie_count = 0;
    c_lie = 0;    
}  

#pragma CODE_SEG DEFAULT 
 