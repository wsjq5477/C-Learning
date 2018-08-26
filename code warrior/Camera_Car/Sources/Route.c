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
unsigned char left_flag;
unsigned char right_flag;

/*************************************************************
������void route_judge(void);
���ܣ������б�
˵�������ݺ��������������ƫ������ƫ����
ע��  �򵥲���ʱʹ��
*************************************************************/
void route_judge(void)
{    
    unsigned char i = 0;
    int middle = 0;
    int left_extent = 0;
    int right_extent = 0;
    int left_count = 0;
    int right_count = 0;
    average = 0;
       
    binaryzation();          //��ֵ������
        
    route_extract();         //������ȡ
    

    //���������5�б궨��׼,middle��edge_count�������
    for(i=39;i>34;i--)                   
    {
        middle+=black_line[i];
    }
    middle = middle/5;  
       
    //����ƫ�������ּ���
    edge_count++;
    for(i=39;i>edge_count;i--)            //ȥ��������
    {
        if(black_line[i]-45>0)            //�����������Ҳ�
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

