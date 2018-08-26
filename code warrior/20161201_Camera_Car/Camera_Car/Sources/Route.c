/***************************************************************************
    文件名称: Route.c

    作者:     智能车项目摄像头组

    版本:     2016-11-28
    
    说明:     图像处理与路径规划子程序，完成黑线检测以及路径规划

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "Route.h"
#include "Image.h"
#include "SCI.h"

/*********************引用声明****************************/  
extern unsigned char edge_count;
extern unsigned char black_line[40];
extern unsigned char image_two[40][90]; 


/*********************定义声明****************************/
int average;
unsigned char left_edge_flag;
unsigned char right_edge_flag;
unsigned char left_flag;
unsigned char right_flag;


//unsigned char left_maxflag;
//unsigned char right_maxflag;

/*************************************************************
函数：void route_judge(void);
功能：赛道判别
说明：根据黑线坐标计算总体偏移量和偏移率
注：  简单测试时使用
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
       
    binaryzation();          //二值化处理
        
    route_extract();         //黑线提取
    
    //***************边沿线检测和丢线修正*****************
    if(black_line[39]==0&&black_line[38]==0&&black_line[37]==0&&black_line[36]==0&&black_line[35]==0)                
    {
        for(i=0;i<5;i++)     //判断是左边沿还是右边沿
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
        //根据最近的5行标定基准,middle和edge_count测试完毕
        for(i=39;i>34;i--)                   
        {
            middle+=black_line[i];
        }
        middle = middle/5;  
           
        //总体偏移量积分计算
        edge_count++;
        for(i=39;i>edge_count;i--)               //去除边沿线
        {
            if(black_line[i]-45>0)           //黑线在中线右侧
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
        if(left_extent>right_extent)             //求重心
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

