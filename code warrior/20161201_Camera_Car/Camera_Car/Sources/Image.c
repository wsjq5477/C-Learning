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
unsigned char edge_count; 
unsigned char black_line[40]; 
unsigned char image_two[40][90]; 
unsigned char image[40][90];
  
         
unsigned char c_lie=0,g_lie=0;
unsigned char lie_count=0,hang_count=0; 
 

unsigned int get_n[]={  67, 72, 77, 82, 87, 92, 97,102,107,112,     //图像定距采集对应的摄像头行数
                       117,121,125,129,132,135,138,141,144,147,
                       150,152,154,156,158,160,162,164,166,168,
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

/*************************************************************
函数：void binaryzation(void);
功能：对图像进行二值化处理
说明：实验测得黑线阈值范围为0x30-0x60，白线为0x80以上
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
函数：void route_extract(void);
功能：黑线提取子程序
说明：将赛道的中线提取至数组中并过滤掉赛道边线
*************************************************************/ 
void route_extract()
{     
    signed char i,j;
    unsigned char min,max;
    int amount;
    int temp_black; 
    int temp_j;
    
    edge_count = 0;                     //边沿线计数器清零

    for(i=39;i>34;i--)                  //由近及远逐行提取,默认前5行不会出现赛道边沿部分
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)              
        {
            if(image_two[i][j]==1)      //对黑线坐标取均值得到黑线中点
            {
                temp_j=(int)j;
                temp_black+=temp_j;     //累加
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }  
    for(i=34;i>-1;i--)                  //6-40行
    {
        temp_black = 0;
        amount = 0;
        if(black_line[i+1]-5>0)         //确保数组不越界
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
     
        for(j=min;j<max;j++)            //每行从上一行中点的左右5个坐标内判断黑线,过滤噪点
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
            if(i<20)                    //0-19行,丢线则判定为赛道边沿,记录边沿线的行数
            {
                temp_black = 45;
                amount = 1;    
                edge_count++;   
            }
            else                        //20-34行,若该行丢线则补线
            {
                temp_black = black_line[i+1];
                amount = 1;    
            }
        }
    
        black_line[i] = (unsigned char)(temp_black/amount); 
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
    delay(10000);     //等待图像采集完毕才可发送数据
   
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
说明：OV5116的3脚接高速ad;5脚HS为行中断,接PT0;
*************************************************************/
#pragma CODE_SEG NON_BANKED
void interrupt 8 IC0ISR(void)     //进入行中断
{  
    TFLG1_C2F=1;                   //清零标志位               
    if(lie_count==get_n[c_lie])    //当列数和设置的需采集的列数相等
    {
        delay(1);                  //消隐
        for(hang_count=0;hang_count<=hang_end;hang_count++)
        {  
            image[c_lie][hang_count] = PORTA;   //将整行存入数组     
        }
        c_lie++;                   
    }
    lie_count++;                   //采集到该行则行数加1
    if(lie_count==40)
    {
        TIE_C2I = 0;
    }
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
    TIE_C2I = 1; 
    TIE_C1I = 1; 
}  

#pragma CODE_SEG DEFAULT 
 