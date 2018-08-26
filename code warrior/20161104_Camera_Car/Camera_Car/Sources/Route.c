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

/*********************变量声明******************************/
extern unsigned char image[40][90];
unsigned char black_line[40];
unsigned char left_edge[40];
unsigned char right_edge[40];
unsigned char left_flag,right_flag;

/*************************************************************
函数：void route_extract(void);
功能：黑线及赛道边沿提取
说明：将图像的中心线与边沿线分别提取并存放至数组
*************************************************************/ 
void route_extract()
{
    signed char i,j;
    unsigned char amount;
    unsigned int temp_black;
    
    for(i=39;i>34;i--)                //由近及远逐行提取,默认前5行不会出现赛道边沿部分
    {
        temp_black = 0;
        amount = 0;
        
        for(j=0;j<90;j++)          
        {
            if(image[i][j]==1)        //对黑线水平坐标取均值
            {
                temp_black+=j;        //累加
                amount++;
            }                  
        }
        black_line[i] = (unsigned char)(temp_black/amount);       
    }
    
    for(i=34;i>-1;i--)                 //6-40行每行与上一行黑线相比,若坐标突变则判定为边沿
    {
        for(j=0;j<90;j++)          
        {
            temp_black = 0;
            amount = 0;
            
            if(image[i][j]==1)
            {
                if(j-black_line[i+1]<5||black_line[i+1]-j<5) //与上行黑线坐标相差5以内则判断为黑线
                {
                    temp_black+=j;    
                    amount++;
                }
                else
                {
                    if(image[i][j-1]==0)   //若为上升沿,则判断为右侧弯道边线
                    {
                        right_edge[i] = j;  
                    }
                    if(image[i][j+1]==0)   //若为下降沿,则判断为左侧弯道边线
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
函数：void easy_extract(void);
功能：黑线提取
说明：通过30行的黑线偏移量判断弯道方向
*************************************************************/ 
void easy_extract()
{
    unsigned char j;
    unsigned char left_line,right_line;
    
    for(j=0;j<89;j++)             //从右往左检测下降沿
    {
        if(image[30][j+1]==1)     //后入的数据(中线左边沿)覆盖先入数据(赛道右边沿)  
        {
            if(image[30][j]==0)
            {
                left_line = j+1;
            }
        }                     
    }
    
    for(j=89;j>0;j--)             //从左往右检测下降沿
    {
        if(image[30][j]==1)       //后入的数据(中线右边沿)覆盖先入数据(赛道左边沿)
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
函数：void route_judge(void);
功能：赛道判别
说明：对提取的数据进行赛道判别
*************************************************************/
void route_judge(void)
{
    unsigned char left_flag = 0;
    unsigned char right_flag = 0;
    unsigned char leftedge_flag = 0;
    unsigned char rightedge_flag = 0;
    unsigned int left_extent = 0;
    unsigned int right_extent = 0;
    
    //总体趋势与弯曲程度(偏移量)计算
    signed char i;
    for(i=39;i>0;i--)
    {
        if(black_line[i]-45>0)           //黑线在中线右侧
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
    
    //1.正常弯道:正常打角;2.转角过小:大幅度打角;3.转角过大或出弯偏移,进行小幅度摆角
    if(left_flag-right_flag>10)          //正常左弯
    {
        if(left_extent+right_extent>100) //打角过小,这里偏移量的值需实际测定
        {
             //左转较大角度(测定),或根据偏移量打角
        }
        else 
        {
             //正常左转
        }          
    }   
    else if(right_flag-left_flag>10)     //正常右弯
    {
        if(left_extent+right_extent>100) //打角过小,这里偏移量的值需实际测定
        {
             //右转较大角度(测定),或根据偏移量打角
        }
        else 
        {
             //正常右转
        }          
    }
    else     //偏转过头或直线出弯小角度偏移
    {
        if(black_line[10]>55)
        {
             //进行小幅度右摆角
        }
        if(black_line[10]<35)
        {
             //进行小幅度左摆角
        }
        
    }
        
    //赛道边沿判定
    if(leftedge_flag>5)       
    {
        //大角度右转   
    }
    if(rightedge_flag>5)       
    {
        //大角度左转   
    }    
}
