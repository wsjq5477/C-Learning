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
unsigned char edge_count; 
unsigned char black_line[40]; 
unsigned char image_two[40][90]; 
unsigned char image[40][90];
  
         
unsigned char c_lie=0,g_lie=0;
unsigned char lie_count=0,hang_count=0; 
 

unsigned int get_n[]={  67, 72, 77, 82, 87, 92, 97,102,107,112,     //ͼ�񶨾�ɼ���Ӧ������ͷ����
                       117,121,125,129,132,135,138,141,144,147,
                       150,152,154,156,158,160,162,164,166,168,
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
������void binaryzation(void);
���ܣ���ͼ����ж�ֵ������
˵����ʵ���ú�����ֵ��ΧΪ0x30-0x60������Ϊ0x80����
*************************************************************/ 
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
������void route_extract(void);
���ܣ�������ȡ�ӳ���
˵������������������ȡ�������в����˵���������
*************************************************************/ 
void route_extract()
{     
    signed char i,j;
    unsigned char min,max;
    int amount;
    int temp_black; 
    int temp_j;
    
    edge_count = 0;                     //�����߼���������

    for(i=39;i>34;i--)                  //�ɽ���Զ������ȡ,Ĭ��ǰ5�в�������������ز���
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)              
        {
            if(image_two[i][j]==1)      //�Ժ�������ȡ��ֵ�õ������е�
            {
                temp_j=(int)j;
                temp_black+=temp_j;     //�ۼ�
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }  
    for(i=34;i>-1;i--)                  //6-40��
    {
        temp_black = 0;
        amount = 0;
        if(black_line[i+1]-5>0)         //ȷ�����鲻Խ��
        {
            min = black_line[i+1]-5;            
        }
        else
        {
            min = 0;
        }
        if(black_line[i+1]+5<89)
        {
            max = black_line[i+1]+5;
        }
        else
        {
            max = 89;
        }
     
        for(j=min;j<max;j++)            //ÿ�д���һ���е������5���������жϺ���,�������
        {    
            if(image_two[i][j]==1)
            {
                temp_j=(int)j;
                temp_black+=temp_j;    
                amount++;    
            }
        }
      
        if(temp_black==0)              
        {
            if(i<20)                    //0-19��,�������ж�Ϊ��������,��¼�����ߵ�����
            {
                temp_black = 45;
                amount = 1;    
                edge_count++;   
            }
            else                        //20-34��,�����ж�������
            {
                temp_black = black_line[i+1];
                amount = 1;    
            }
        }
    
        black_line[i] = (unsigned char)(temp_black/amount); 
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
    delay(10000);     //�ȴ�ͼ��ɼ���ϲſɷ�������
   
    binaryzation();
     
    for(i=0;i<40;i++)         
    {                            
        for(j=0;j<90;j++)                      
        {                    
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
˵����OV5116��3�ŽӸ���ad;5��HSΪ���ж�,��PT0;
*************************************************************/
#pragma CODE_SEG NON_BANKED
void interrupt 8 IC0ISR(void)     //�������ж�
{  
    TFLG1_C2F=1;                   //�����־λ               
    if(lie_count==get_n[c_lie])    //�����������õ���ɼ����������
    {
        delay(1);                  //����
        for(hang_count=0;hang_count<=hang_end;hang_count++)
        {  
            image[c_lie][hang_count] = PORTA;   //�����д�������     
        }
        c_lie++;                   
    }
    lie_count++;                   //�ɼ���������������1
    if(lie_count==40)
    {
        TIE_C2I = 0;
    }
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
    TIE_C2I = 1; 
    TIE_C1I = 1; 
}  

#pragma CODE_SEG DEFAULT 
 