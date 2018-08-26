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
#include "Image.h"

/*********************��������******************************/  
extern unsigned char image[40][90];    //��������

extern unsigned char image_two[40][90];       //��������
unsigned char black_line[40];

/*************************************************************
������void binaryzation(void);
���ܣ���ͼ����ж�ֵ������
˵����ʵ���ú�����ֵ��ΧΪ0x30-0x60������Ϊ0x80����
*************************************************************/ 


/*************************************************************
������void route_extract(void);
���ܣ�������ȡ�ӳ���
˵������������������ȡ�������в����˵���������
*************************************************************/ 
void route_extract()
{
    unsigned char i,j;
    unsigned char amount;
    unsigned int temp_black;
    
    
    for(i=39;i>34;i--)                  //�ɽ���Զ������ȡ,Ĭ��ǰ5�в�������������ز���
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)          
        {
            if(image_two[i][j]==1)      //�Ժ�������ȡ��ֵ�õ������е�
            {
                temp_black+=j;          //�ۼ�
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }
    
    for(i=34;i!=0;i--)                  //6-40��ÿ�д���һ���е������10���������жϺ��ߣ����˱�����
    {
        temp_black = 0;
        amount = 0;
        
        for(j=black_line[i+1]-10;j<black_line[i+1]+11;j++)          
        {    
            if(image_two[i][j]==1)
            {
                temp_black+=j;    
                amount++;    
            }
        }
        if(temp_black==0)               //�����ж�������
        {
            temp_black = black_line[i+1];
        }
        black_line[i] = (unsigned char)(temp_black/amount); 
    }
}

