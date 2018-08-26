#include <hidef.h>    
#include <string.h> 
#include "derivative.h"    
#include "SCI.h"
#include "CRG.h"

/****************************************宏定义*******************************************/
#define  SEG_DD    DDRA                                               //段码口方向
#define  DIG_DD    DDRT                                               //位码口方向
#define  SEG_NUM   PORTA                                              //段码数据
#define  DIG_NUM   PTT                                                //位码数据
#define  SW_DD     DDRB_DDRB4                                         //拨码开关数据方向寄存器定义
#define  SW        PORTB_PB4                                          //拨码开关控制位I/O口定义

/***************************************函数声明******************************************/
void system_init(void);
void Num_test(void);
void Num_refresh(void);
int read_data(void);
void rev_data(void);
void send_data(unsigned char *value,unsigned int length);
void Delay(unsigned int loop_times);


/*************************************全局变量声明****************************************/

unsigned char g_data_flag = 0;
unsigned char g_sciflag = 0;
unsigned char g_txd_buf[20];          //定义发送缓冲区
unsigned int g_txd_length;            //发送缓冲区数据长度

unsigned char g_rxd_buf[20];          //定义接收缓冲区
unsigned char g_rxd_busy = 0;         //串口接收缓冲区有数据标志位，串口接收缓冲区不为空时为1
unsigned char g_rxd_data_flag;        //数据有效标志位
unsigned int g_rxd_length;            //接收缓冲区数据长度
unsigned int g_rxd_in_pointer;        //接收缓冲区入队列指针，指向下一个存储单元
unsigned int g_rxd_out_pointer;       //接收缓冲区出队列指针，指向下一个存储单元 

/****************************************主函数*******************************************/

void main(void) 
{       
    system_init();
    for(;;)
    {             
        if(g_sciflag==1)
        {
            g_sciflag = 0; 
            rev_data();
            send_data("RIGHT",5);
            if(g_data_flag==1)
            {
                g_data_flag = 0;
                send_data("RIGHT",5);
            }            
        } 
    }
}

/***************************************************************************************
    函数名称：   interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
    函数功能：   接收中断使能
    调用函数：   无
    入口参数：   无
    出口参数：   无
    返回值：     无
    其他：  	
***************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{
    if (SCI0SR1_RDRF) 
    {
       g_sciflag = 1;
              
       g_rxd_buf[g_rxd_in_pointer] = SCI0DRL;              //将数据读入接收缓冲区    
       g_rxd_in_pointer++;                                 //入队列指针加1，指向下一个存储单元
       if(g_rxd_in_pointer==20)                            //如果入队列指针超出了缓冲区尾地址，则指向首地址
       {
           g_rxd_in_pointer = 0;
       }
       
       g_rxd_busy = 1;                                    //接收缓冲区有数据标志位置1       
    }
    
}
#pragma CODE_SEG DEFAULT


/*************************************************************
函数：void system_init(void)
功能：系统初始化
说明：配置GPIO并将模块初始化
*************************************************************/
void system_init(void)
{                                                                        
    SEG_DD = 0xFF;                                                  //A端口设置为输出口
    DIG_DD = 0xFF;                                                  //H端口设置为输出口
    SEG_NUM = 0x00;                                                 //A端口输出全0
    DIG_NUM = 0x00;                                                 //H端口输出全0   

    SetBusCLK_64M();                                                //总线频率64M
    SCI0_Init();                                                    //SCI初始化
    
    EnableInterrupts;                                               //中断使能
}


/****************************************
函数：void delay(unsigned int loop_times)
功能：延时
说明：延时500*loop_times个机器周期
****************************************/
void Delay(unsigned int loop_times)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i<loop_times; loop_i++) 
    {
        for (loop_j=0; loop_j<4000;loop_j++) 
        {
            ;
        }
    }
}


/*************************************************************
函数void send_data(int data)
功能: 数据发送子程序
说明：将数据打包后发送至上位机
*************************************************************/
void send_data(unsigned char *value,unsigned int length)
{
    unsigned char i; 

    g_txd_length = length;                 //记录数据块长度
    
    SCI0DRL = 0x97;                        //发送起始位             
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
    
    SCI0DRL = g_txd_length;                //发送数据长度            
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
    
    for(i=0;i<length;i++)                  //发送数据块
    {
        SCI0DRL = value[length-1];
        while(!SCI0SR1_TC)                        
        {            
            ;
        }     
    }
    
    SCI0DRL = 0x98;                        //发送结束位
    while(!SCI0SR1_TC)                        
    {            
        ;
    } 
}

/*************************************************************
函数void read_data(void);
功能: 数据读取子程序
说明：将接收缓冲区的数据读取出来
*************************************************************/
int read_data()
{
    unsigned int temp_data;
    
    if((g_rxd_out_pointer!=g_rxd_in_pointer)&&(g_rxd_out_pointer<20))  //如果出队列指针不等于入队列指针且有效，表示接收缓冲区在有数据，否则为空
    {
        temp_data = g_rxd_buf[g_rxd_in_pointer];                        //从接收缓冲区读取一个数据  
        g_rxd_out_pointer++;                                           //出队列指针加1，指向下一个存储单元
    
        if(g_rxd_out_pointer!=g_rxd_in_pointer)                        //如果出队列指针等于入队列指针，表示接收缓冲区已空
        {
            g_rxd_busy = 0;                                            //接收缓冲区为空
        } 
       
        if(g_rxd_out_pointer==20)                                      //如果出队列指针超出了缓冲区尾地址，则指向首地址
        {
            g_rxd_out_pointer = 0;
        } 
        
        return temp_data;
    }  
    else
    {                                                                  
        g_rxd_busy = 0;                                                //接收缓冲区空
        g_rxd_data_flag = 0;                                           //数据无效
    }
}

/*************************************************************
函数void rev_data(void);
功能: 数据接收子程序
说明：接收缓冲区的数据
*************************************************************/
void rev_data()
{
    
    static unsigned char i;
    
    unsigned int tempdata;
    unsigned int datalength; 
    unsigned char rev_data[20];
    unsigned char revflag = 0;
    unsigned char startflag = 0;
    unsigned char dataflag = 0;
    
    tempdata = read_data();                         //读取缓冲区数据
    if(g_rxd_busy!=0)                               //接收缓冲区有数据
    {
        if(g_rxd_data_flag!=0)                      //数据有效
        {
            revflag = 1; 
            rev_data[i] = tempdata;  
        }     
    }
    
    if(revflag==1)
    {
        revflag = 0;
       
        if(tempdata==0x97)                          //检测帧头是否正确
        {
            i = 1;                                  //正确则下一帧开始接收数据长度
            startflag = 1; 
        }
        
        if(startflag==1)                            
        {
            datalength = tempdata;                  //接收数据长度
            i = 2; 
            dataflag = 1;                           //下一帧开始接收数据
        }
        
        if(dataflag==1)            
        {
            if(i<datalength+2)                      //判断接收数据包的长度是否在预定范围内
            {
                rev_data[i] = tempdata;             //正常接收数据
                i++;
            }
            else if(tempdata==0x98)                 //判断帧尾是否正确
            {
                if(rev_data[2]=='A')                //此处判断数据是否为'A',不同情况可以更改判断条件
                {
                    g_data_flag = 1;
                }
            }   
            else
            {
                send_data("ERROR",5);
            }
        }       
    }    
}







