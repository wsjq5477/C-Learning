/***************************************************************************
    �ļ�����: Route.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-08
    
    ˵��:     ͼ������·���滮�ӳ�����ɺ��߼���Լ�·���滮

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Route.h"

/*********************��������******************************/
extern unsigned char image[40][90];
unsigned char black_line[40];
unsigned char left_edge[40];
unsigned char right_edge[40];
unsigned char left_flag,right_flag;

/*************************************************************
������void route_extract(void);
���ܣ����߼�����������ȡ
˵������ͼ���������������߷ֱ���ȡ�����������
*************************************************************/ 
void route_extract()
{
    signed char i,j;
    unsigned char amount;
    unsigned int temp_black;
    
    for(i=39;i>34;i--)                //�ɽ���Զ������ȡ,Ĭ��ǰ5�в�������������ز���
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)          
        {
            if(image[i][j]==1)        //�Ժ���ˮƽ����ȡ��ֵ
            {
                temp_black+=j;        //�ۼ�
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }
    
    for(i=34;i>-1;i--)                 //6-40��ÿ������һ�к������,������ͻ�����ж�Ϊ����
    {
        for(j=0;j<90;j++)          
        {
            temp_black = 0;
            amount = 0;
            
            if(image[i][j]==1)
            {
                if(j-black_line[i+1]<5||black_line[i+1]-j<5) //�����к����������5�������ж�Ϊ����
                {
                    temp_black+=j;    
                    amount++;
                }
                else
                {
                    if(image[i][j-1]==0)   //��Ϊ������,���ж�Ϊ�Ҳ��������
                    {
                        right_edge[i] = j;  
                    }
                    if(image[i][j+1]==0)   //��Ϊ�½���,���ж�Ϊ����������
                    {
                        left_edge[i] = j;
                    }
                }
                black_line[i] = (unsigned char)(temp_black/amount); 
            }
        }
    }
}
/*************************************************************
������void easy_extract(void);
���ܣ�������ȡ
˵����ͨ��30�еĺ���ƫ�����ж��������
*************************************************************/ 
void easy_extract()
{
    unsigned char j;
    unsigned char left_line,right_line;
    
    for(j=0;j<89;j++)             //�����������½���
    {
        if(image[30][j+1]==1)     //���������(���������)������������(�����ұ���)  
        {
            if(image[30][j]==0)
            {
                left_line = j+1;
            }
        }                     
    }
    
    for(j=89;j>0;j--)             //�������Ҽ���½���
    {
        if(image[30][j]==1)       //���������(�����ұ���)������������(���������)
        {
            if(image[30][j-1]==0)
            {
                right_line = j;
            }
        }
    }
                  
    if(((left_line + right_line)/2)>55)
    {
        left_flag = 1;
    }
    if(((left_line + right_line)/2)<35)
    {
        right_flag = 1;
    }   
}


/*************************************************************
������void route_judge(void);
���ܣ������б�
˵��������ȡ�����ݽ��������б�
*************************************************************/
void route_judge(void)
{
    unsigned char left_flag = 0;
    unsigned char right_flag = 0;
    unsigned char leftedge_flag = 0;
    unsigned char rightedge_flag = 0;
    unsigned int left_extent = 0;
    unsigned int right_extent = 0;
    
    //���������������̶�(ƫ����)����
    signed char i;
    for(i=39;i>0;i--)
    {
        if(black_line[i]-45>0)           //�����������Ҳ�
        {
            right_flag++;
            right_extent+=black_line[i]-45;
        }
        else
        {
            left_flag++;
            left_extent+=black_line[i]-45;
        }    
        if(left_edge[i]==1)
        {
            leftedge_flag++;
        }
        if(right_edge[i]==1)
        {
            rightedge_flag++;
        }
        
    }
    
    //1.�������:�������;2.ת�ǹ�С:����ȴ��;3.ת�ǹ�������ƫ��,����С���Ȱڽ�
    if(left_flag-right_flag>10)          //��������
    {
        if(left_extent+right_extent>100) //��ǹ�С,����ƫ������ֵ��ʵ�ʲⶨ
        {
             //��ת�ϴ�Ƕ�(�ⶨ),�����ƫ�������
        }
        else 
        {
             //������ת
        }          
    }   
    else if(right_flag-left_flag>10)     //��������
    {
        if(left_extent+right_extent>100) //��ǹ�С,����ƫ������ֵ��ʵ�ʲⶨ
        {
             //��ת�ϴ�Ƕ�(�ⶨ),�����ƫ�������
        }
        else 
        {
             //������ת
        }          
    }
    else     //ƫת��ͷ��ֱ�߳���С�Ƕ�ƫ��
    {
        if(black_line[10]>55)
        {
             //����С�����Ұڽ�
        }
        if(black_line[10]<35)
        {
             //����С������ڽ�
        }
        
    }
        
    //���������ж�
    if(leftedge_flag>5)       
    {
        //��Ƕ���ת   
    }
    if(rightedge_flag>5)       
    {
        //��Ƕ���ת   
    }    
}
