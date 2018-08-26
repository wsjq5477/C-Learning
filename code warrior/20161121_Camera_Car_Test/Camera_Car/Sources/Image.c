/***************************************************************************
    文件名称: Image.c

    作者:     智能车项目摄像头组

    版本:     2016-11-04
    
    说明:     图像采集和图像发送子程序

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Image.h"
#include "Route.h"
#include "SCI.h"

/*********************宏定义******************************/
#define lie_end  260
#define hang_end 91
#define lie  52   

/*********************变量声明******************************/
extern unsigned char black_line[40];       //引用声明
unsigned char image_two[40][90];    

unsigned char image[40][90];               //定义声明
unsigned char c_lie=0,g_lie=0;
unsigned char lie_count=0,hang_count=0; 
 

unsigned int get_n[]={  16, 29, 41, 52, 62, 71, 79, 86, 92, 98,     //图像定距采集对应的摄像头行数
                       103,108,112,116,120,124,128,132,135,138,
                       141,144,147,150,153,156,159,162,165,168,
                       170,172,174,176,178,180,182,184,186,188,
                       190,192,194,196,198,200,202,204,206,208,
                       210,211,212,242,243,245,246,247,248,249,250};  
                       
/*************************************************************
函数：void delay(unsigned int num);
功能：延时函数
说明：基于64M总线的去消隐延时
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
函数：void send_image(void);
功能：发送图像至串口助手
说明：调试摄像头时使用
*************************************************************/ 
void send_image()
{
    
    unsigned char i,j;
    unsigned char sdata;   
    delay(10000);           //等待图像采集完毕才可发送数据 
    
    binaryzation();
    
    for(i=0;i<40;i++)         
    {                            
        for(j=0;j<90;j++)                      
        {     
                              
            //**************二值化处理后的图像************************                 
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
    for(;;)     //测试时仅发送一副图,若想继续发送需复位程序
    {
        ;
    }       
}
 
/*************************************************************
函数：void send_line(void);
功能：发送处理后的黑线和左右边沿线至串口助手
说明：
*************************************************************/ 
void send_line()
{
    unsigned char i,j;
    unsigned char sdata;
    
    delay(10000);
    
    binaryzation();          //二值化处理
    
    route_extract();         //黑线提取
       
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
函数：中断处理函数
功能：行中断处理
说明：OV5116的4脚D1为二值化输出,接PA0;5脚HS为行中断,接PT2;
*************************************************************/
#pragma CODE_SEG NON_BANKED
void interrupt 8 IC2ISR(void)     //进入行中断
{  
    TFLG1_C2F=1;                   //清零标志位               
    if(lie_count==get_n[c_lie])    //当列数和设置的需采集的列数相等
    {
        delay(1);                  //消隐
        for(hang_count=0;hang_count<=hang_end;hang_count++)
        {  
            image[c_lie][hang_count] = PORTB;   //将整行存入数组     
        }
        c_lie++;                   
    }
    lie_count++;                   //采集到该行则行数加1
}

/*************************************************************
函数：中断处理函数
功能：场中断处理
说明：OV5116的6脚VS为场中断,接PT1
*************************************************************/
void interrupt 9 IC1ISR(void)      //进入场中断
{              
    TFLG1_C1F = 1;                 //清零标志位
    TFLG1_C2F=1;                   //清零标志位 
    hang_count = 0;                //清零行数和列数
    lie_count = 0;
    c_lie = 0;    
}  

#pragma CODE_SEG DEFAULT 
 