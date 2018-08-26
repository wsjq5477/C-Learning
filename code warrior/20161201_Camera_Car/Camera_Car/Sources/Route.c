/***************************************************************************
    �ļ�����: Route.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-28
    
    ˵��:     ͼ������·���滮�ӳ�����ɺ��߼���Լ�·���滮

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Route.h"
#include "Image.h"
#include "SCI.h"

/*********************��������****************************/  
extern unsigned char edge_count;
extern unsigned char black_line[40];
extern unsigned char image_two[40][90]; 


/*********************��������****************************/
int average;
unsigned char left_edge_flag;
unsigned char right_edge_flag;
unsigned char left_flag;
unsigned char right_flag;


//unsigned char left_maxflag;
//unsigned char right_maxflag;

/*************************************************************
������void route_judge(void);
���ܣ������б�
˵�������ݺ��������������ƫ������ƫ����
ע��  �򵥲���ʱʹ��
*************************************************************/
void route_judge(void)
{    
    unsigned char i = 0;
    unsigned char left_edge_count = 0;
    unsigned char right_edge_count = 0;
    int middle = 0;
    int left_extent = 0;
    int right_extent = 0;
    int left_count = 0;
    int right_count = 0;
    average = 0;
       
    binaryzation();          //��ֵ������
        
    route_extract();         //������ȡ
    
    //***************�����߼��Ͷ�������*****************
    if(black_line[39]==0&&black_line[38]==0&&black_line[37]==0&&black_line[36]==0&&black_line[35]==0)                
    {
        for(i=0;i<5;i++)     //�ж�������ػ����ұ���
        {
            if(black_line[i]<45)
            {
                left_edge_count++;
            }
            else
            {
                right_edge_count++;
            }
        }
        if(left_edge_count>right_edge_count)
        {
            left_edge_count = 0;
            right_edge_count = 0;
            left_edge_flag = 1;
        }
        else
        {
            left_edge_count = 0;
            right_edge_count = 0;
            right_edge_flag = 1;
        }
        
    }
    else
    { 
        //���������5�б궨��׼,middle��edge_count�������
        for(i=39;i>34;i--)                   
        {
            middle+=black_line[i];
        }
        middle = middle/5;  
           
        //����ƫ�������ּ���
        edge_count++;
        for(i=39;i>edge_count;i--)               //ȥ��������
        {
            if(black_line[i]-45>0)           //�����������Ҳ�
            {
                right_count++;
                right_extent+=(black_line[i]-45);
            }
            else                                 
            {
                left_count++;
                left_extent+=(45-black_line[i]);
            }    
        }
        if(left_extent>right_extent)             //������
        {
            average = (left_extent-right_extent)/(right_count+left_count);
            left_flag = 1; 
        }
        else 
        {
            average = (right_extent-left_extent)/(right_count+left_count);
            right_flag = 1;
        }
    }
}

