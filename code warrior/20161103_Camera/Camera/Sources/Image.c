/***********************************************************
    �ļ�����: Image.c
    ˵ ��:   ͼ�������
***********************************************************/
#include "include.h"
#include "Image.h"
/***********��������******************************/
unsigned char Image_Data[ROW][COLUMN]={{0}}; //ͼ������
unsigned char uc_lie,Line_C;//hang��ʾ����ͷʵ�ʲɼ���������uc_lie��ʾÿ�вɼ��ĵ���
unsigned int  hang;                                //Line_C��ʾ����ʵ���õ�������                                 
unsigned char flag=0;          //ͼ��ɼ��봦��ı�־λ1��ʾͼ����0��ʾͼ��ɼ�

unsigned char INTERVAL_NUM;




/*****************��������*********************************/
 

/************************************************** 
** ��������: ���ڷ���˳���
** ��������: ����ͼ��ɼ����ص�DEMOKtool
** ˵����    �ڷ���ͼ������ǰҪ����Э�鷢��0XFFͼ��ͷ
***************************************************/
void SCI0_Transmit(void) 
{
    unsigned char i,j;
    
    while(!(SCI0SR1&0x80)) 
    {
        ;
    }
    SCI0DRH = 0;
    SCI0DRL = 0xFF;                    //�ȷ���һ��֡ͷff
    
    
    for(i=0; i<ROW; i++) 
    {
        for(j=0; j<COLUMN; j++) 
        {
            while(!(SCI0SR1&0x80)); 
            SCI0DRL = Image_Data[i][j];
                           
        }    
    }   
    
    for(;;)
    {
        ;
    }
}


/************************************************** 
** ��������: �жϴ�����
** ��������: ���жϴ�����
** ��    ��: �� 
** ��    ��: �� 
** ˵����  
***************************************************/ 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vtimch0 void HREF_Count(void)
{
    unsigned char l_delay;  //������ʱ����
    TFLG1_C2F = 1; //���жϱ�־����
    hang++;
 
    if(Line_C < 20)
    {
        INTERVAL_NUM = 3;
    } 
    else if(Line_C < 30)
    {
        INTERVAL_NUM = 6;
    }
    else
    {
        INTERVAL_NUM = 9;
    }
    if(hang % INTERVAL_NUM==0)   
    {
          
        for(l_delay = 0;l_delay < 55;l_delay++);   //����ʱ��ȥ������������ʵ��ͼ��ȷ����������Ϊ��80M������»�ò���
          
        for(uc_lie = 0; uc_lie<COLUMN; uc_lie++)   
        {
           	Image_Data[Line_C][uc_lie] = PORTA_PA0;
            _asm(nop);   //����ʱ��ȷ��ͼ��ֱ��ʱͼ�����߶Գ�,��Ϊ��80M������»�ò���  
            _asm(nop);
            _asm(nop);
            _asm(nop);   //����ʱ��ȷ��ͼ��ֱ��ʱͼ�����߶Գ�,��Ϊ��80M������»�ò���  
            _asm(nop);
            _asm(nop);
            _asm(nop);   //����ʱ��ȷ��ͼ��ֱ��ʱͼ�����߶Գ�,��Ϊ��80M������»�ò���      
         }
        Line_C++;
        if(Line_C==40)
        {
            flag = 1;
            TIE_C0I = 0;
        }
        
    }  //end of if(hang%INTERVAL_NUM==0)
}

/************************************************** 
** ��������: �жϴ�����
** ��������: ���жϴ�����
** ��    ��: �� 
** ��    ��: �� 
** ˵����  
***************************************************/
interrupt VectorNumber_Vtimch1 void VSYN_interrupt(void)
{
    TFLG1_C1F = 1; //�峡�ж�
    TFLG1_C2F = 1; //�����ж�
    hang = 0;
    Line_C = 0; //�м����� 

    TIE_C1I = 1; 
    TIE_C0I = 1;  
}
#pragma CODE_SEG DEFAULT


