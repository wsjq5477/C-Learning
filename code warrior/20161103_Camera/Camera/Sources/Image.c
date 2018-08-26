/***********************************************************
    文件名称: Image.c
    说 明:   图像处理程序
***********************************************************/
#include "include.h"
#include "Image.h"
/***********变量声明******************************/
unsigned char Image_Data[ROW][COLUMN]={{0}}; //图像数组
unsigned char uc_lie,Line_C;//hang表示摄像头实际采集的行数，uc_lie表示每行采集的点数
unsigned int  hang;                                //Line_C表示我们实际用到的行数                                 
unsigned char flag=0;          //图像采集与处理的标志位1表示图像处理，0表示图像采集

unsigned char INTERVAL_NUM;




/*****************函数声明*********************************/
 

/************************************************** 
** 函数名称: 串口发射端程序
** 功能描述: 发送图像采集像素到DEMOKtool
** 说明：    在发送图像数据前要按照协议发送0XFF图像头
***************************************************/
void SCI0_Transmit(void) 
{
    unsigned char i,j;
    
    while(!(SCI0SR1&0x80)) 
    {
        ;
    }
    SCI0DRH = 0;
    SCI0DRL = 0xFF;                    //先发送一个帧头ff
    
    
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
** 函数名称: 中断处理函数
** 功能描述: 行中断处理函数
** 输    入: 无 
** 输    出: 无 
** 说明：  
***************************************************/ 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vtimch0 void HREF_Count(void)
{
    unsigned char l_delay;  //用于延时消隐
    TFLG1_C2F = 1; //行中断标志清零
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
          
        for(l_delay = 0;l_delay < 55;l_delay++);   //短延时，去除消隐，根据实际图像确定参数，此为在80M的情况下获得参数
          
        for(uc_lie = 0; uc_lie<COLUMN; uc_lie++)   
        {
           	Image_Data[Line_C][uc_lie] = PORTA_PA0;
            _asm(nop);   //短延时，确保图像直道时图像两边对称,此为在80M的情况下获得参数  
            _asm(nop);
            _asm(nop);
            _asm(nop);   //短延时，确保图像直道时图像两边对称,此为在80M的情况下获得参数  
            _asm(nop);
            _asm(nop);
            _asm(nop);   //短延时，确保图像直道时图像两边对称,此为在80M的情况下获得参数      
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
** 函数名称: 中断处理函数
** 功能描述: 场中断处理函数
** 输    入: 无 
** 输    出: 无 
** 说明：  
***************************************************/
interrupt VectorNumber_Vtimch1 void VSYN_interrupt(void)
{
    TFLG1_C1F = 1; //清场中断
    TFLG1_C2F = 1; //清行中断
    hang = 0;
    Line_C = 0; //行计数器 

    TIE_C1I = 1; 
    TIE_C0I = 1;  
}
#pragma CODE_SEG DEFAULT


