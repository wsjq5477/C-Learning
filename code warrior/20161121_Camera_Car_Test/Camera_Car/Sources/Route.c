/***************************************************************************
    文件名称: Route.c

    作者:     智能车项目摄像头组

    版本:     2016-11-08
    
    说明:     图像处理与路径规划子程序，完成黑线检测以及路径规划

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Route.h"
#include "Image.h"

/*********************变量声明******************************/  
extern unsigned char image[40][90];    //引用声明

extern unsigned char image_two[40][90];       //定义声明
unsigned char black_line[40];

/*************************************************************
函数：void binaryzation(void);
功能：对图像进行二值化处理
说明：实验测得黑线阈值范围为0x30-0x60，白线为0x80以上
*************************************************************/ 


/*************************************************************
函数：void route_extract(void);
功能：黑线提取子程序
说明：将赛道的中线提取至数组中并过滤掉赛道边线
*************************************************************/ 
void route_extract()
{
    unsigned char i,j;
    unsigned char amount;
    unsigned int temp_black;
    
    
    for(i=39;i>34;i--)                  //由近及远逐行提取,默认前5行不会出现赛道边沿部分
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)          
        {
            if(image_two[i][j]==1)      //对黑线坐标取均值得到黑线中点
            {
                temp_black+=j;          //累加
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }
    
    for(i=34;i!=0;i--)                  //6-40行每行从上一行中点的左右10个坐标内判断黑线，过滤边沿线
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
        if(temp_black==0)               //若该行丢线则补线
        {
            temp_black = black_line[i+1];
        }
        black_line[i] = (unsigned char)(temp_black/amount); 
    }
}

