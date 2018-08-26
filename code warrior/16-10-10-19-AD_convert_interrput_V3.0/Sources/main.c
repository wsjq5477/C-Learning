/*************************************************
    文件名称：main.c
    
    功能描述：AD2路通道1S采样一次
              低四位显示采样的模拟量，高四位显示采样的数字量
              按下按键后， 数码管能够在AD1, AD0两路通道中转换显示
              PC发送自定义的字符串，单片机发送ATD1, ATD0的采样的数字值及模拟值 
              
    作者：    彭亮
    
    编译环境：Freescale CodeWarrier V5.0 For S12
    
    版本：    V3.0
    
    说明：
    
    修改记录：
    
    创建日期：2016-10-10
**************************************************/

#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include <setjmp.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "main.h"                                           
                                             
//************主程序************//
/************************************************************************
    函数名称：   void main(void)
    函数功能：   实现AD多通道连续采样，并用数码管显示采样结果，并与PC通信
    调用函数：   void sys_init() -------- main.c文件中
                 void rev_data() -------- main.c文件中
                 void send_data(unsigned char *uc_p_send_data, unsigned char uc_data_len)-------- main.c文件中
                 void get_AD(struct AD *p_AD1_data, struct AD *p_AD0_data)-------- main.c文件中 
                 void data_to_send_convert(struct AD const *p_AD1_data, struct AD const *p_AD0_data, unsigned char *a_uc_ad_data)-------- main.c文件中 
                 void show_value(struct AD const *p_AD1_data, struct AD const *p_AD0_data)-------- main.c文件中 
                 void convert_num(struct AD *p_AD1_data, struct AD *p_AD0_data, unsigned char uc_num)-------- main.c文件中   	
*************************************************************************/                                   
void main(void) 
{  
    unsigned char a_uc_ad_data[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    struct AD AD0_data = {0, 0, {0, 0, 0, 0}, {0, 0, 0}};
    struct AD AD1_data = {0, 0, {0, 0, 0, 0}, {0, 0, 0}};
    unsigned char uc_a[6] = {'R', 'e', 'a', 'd', 'y', '\n'};
    g_uc_get_flag = 0;
    g_uc_sec_flag = 0;
    g_uc_key_flag = 0;
    g_uc_cmd_flag = 0;
                                
    sys_init();                    //初始化
    rxd_txd_buf_init();          //缓冲区初始化
    EnableInterrupts;                //开中断     

    send_data(uc_a, 6);              //串口发送表示正常工作状态     

    /***错误转移到主函数内***/ 
    switch(setjmp(buf))
    {
        case 1: PORTB = 0XFE; break;  //串口忙
        case 2: PORTB = 0XFD; break; //数据格式错误，并转移到错误处理函数内
        case 3: PORTB = 0XFB; break; //数据长度错误，发送错误信息
        default: break;
    }
    
    for(;;) 
    {             
        if(g_uc_sec_flag)                            //判断1S定时时间是否到达
        {
            g_uc_sec_flag = 0;
            get_AD(&AD1_data, &AD0_data);               //获取采样值, 先数后模
            convert_num(&AD1_data, &AD0_data, 4);    //采样值拆分
        }
        
        
        if(g_uc_get_flag)                            //判断PC是否接收字符
        {
            g_uc_get_flag = 0;
            rev_data();                              //接收字符   
            if(g_uc_cmd_flag)                        //判断字符有效性
            {               
                g_uc_cmd_flag = 0;
                data_to_send_convert(&AD1_data, &AD0_data, a_uc_ad_data); //采样数据格式转换
                send_data(a_uc_ad_data, 21);           //发送采样数据              
            }
        }        
        show_value(&AD1_data, &AD0_data);              //数据显示在数码管上  
    } /* loop forever */
    /* please make sure that you never leave main */
}


/****************************************
函数：SCI中断服务子程序
功能：接收标志
说明：判断PC发送字符有效性
****************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED               //中断寻址，进入未分页区             
interrupt VectorNumber_Vsci0 void SCI0_ISR(void)
{   
    if(SCI0SR1_RDRF)
    {  
        g_uc_txd_busy_flag = 1;
        if(SCI0SR1_IDLE)
        {
            g_uc_get_flag = 1;
            g_uc_txd_busy_flag = 0;
        } 
        g_uc_rxd_buf[g_uc_rxd_line_in++] = SCI0DRL;   // 把接收到的数据存入接收缓冲区,入队列指针加1，指向下一个存储单元     
        if(g_uc_rxd_line_in == RXD_BUF)
        {
            g_uc_rxd_line_in = 0;   // 如果入队列指针超出了缓冲区尾地址，则指向首地址
        }
        g_uc_rxd_data_flag = 1;   // 标志置1，表示接收缓冲区中有数据
    }
       
    if(SCI0SR1_TC)
    {          
        if(g_uc_txd_line_next < g_uc_txd_buf_len)  // 检查数据是否发送完毕，还有数据则发送另一个字节数据
        {
            SCI0DRL = g_uc_txd_buf[g_uc_txd_line_next++];      //把数据缓冲区指针指向的数据移到发送寄存器, 调整数据缓冲区指针，指向下一个数据
        }
        else 
        {
            g_uc_txd_busy_flag = 0;        // 数据发送完毕, 清零发送串口忙位
            SCI0CR2 = 0X2C;                //发送完毕，关闭此次发送中断
        }
    }
    
}
#pragma CODE_SEG DEFAULT                           //进入分页区（默认）


/****************************************
函数：PIT中断服务子程序
功能：定时标志
说明：判断1S定时时间是否到达
****************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED               //中断寻址，进入未分页区
interrupt VectorNumber_Vpit0 void PIT_ISR(void)
{
    static unsigned char uc_pit_cnt = 0;
    
    PITTF_PTF0 = 1;                     //清零标志位
    
    if(10 == ++uc_pit_cnt)
    {
        uc_pit_cnt = 0;
        g_uc_sec_flag = 1;           //一秒到，标志位置一   
    }    
}
#pragma CODE_SEG DEFAULT                           //进入分页区（默认） 

/****************************************
函数：外部按键中断服务子程序
功能：按键转换状态
说明：按下按键转换状态
****************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Virq void VIRQ_ISR(void) 
{
    g_uc_key_flag = 1;                     //按键中断触发                                  
}
#pragma CODE_SEG DEFAULT 


/******************函数实现***********************/
/****************************************
函数：void get_AD(unsigned int *p_ui_AD1, unsigned int *p_ui_AD0)
功能：接收采样值
说明：获得AD结果寄存器中的采样值
调用函数：void map(unsigned int *p_ui_AD1, unsigned int *p_ui_AD0)-------- main.c文件中 
****************************************/
void get_AD(struct AD *p_AD1_data, struct AD *p_AD0_data)
{
    unsigned char i;
    unsigned int ui_ad0_data_get[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int ui_ad1_data_get[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    i = 0;
       
    DisableInterrupts;
    
    for(i = 0; i < 20; i++)
    {
        ATD0CTL5 = 0X10;       /*  
                                *  0b00010000
                                *    ||||||||__
                                *    |||||||___[CD~CA]模拟输入通道选择码
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______多通道采样模式位
                                *    |||_______连续转换序列模式位，1是连续转换；0是只进行一次转换
                                *    ||________特殊通道转换使能位
                                *    |_________只读位 
                                */
        while(!ATD0STAT2_CCF0)
        {
            ;
        }
        ui_ad0_data_get[i] = ATD0DR0;        //收集AD0采样值
        while(!ATD0STAT2_CCF1)
        {
            ;
        }      
        ui_ad1_data_get[i] = ATD0DR1;         //收集AD1采样值
    }
    
    EnableInterrupts;
    
    quick_sort(ui_ad0_data_get, 0, 19);       //快速排序，排完后在下面选中间6个值取平均完成粗略滤波
    quick_sort(ui_ad1_data_get, 0, 19);
    
    p_AD0_data->ui_dig = (unsigned int)((ui_ad0_data_get[7] + ui_ad0_data_get[8] + ui_ad0_data_get[9] +
                                         ui_ad0_data_get[10] + ui_ad0_data_get[11] + ui_ad0_data_get[12]) / 6);        //收集采样值
      
    p_AD1_data->ui_dig = (unsigned int)((ui_ad1_data_get[7] + ui_ad1_data_get[8] + ui_ad1_data_get[9] +
                                         ui_ad1_data_get[10] + ui_ad1_data_get[11] + ui_ad0_data_get[12]) / 6);
       
    map(p_AD1_data, p_AD0_data);          //数字量映射为模拟量
}


/****************************************
函数：void rxd_txd_buf_init()
功能：初始化
说明：缓冲区初始化
****************************************/
void rxd_txd_buf_init()
{
    g_uc_txd_busy_flag = 0;
    g_uc_rxd_data_flag = 0;
    g_uc_rxd_line_in = 0;
    g_uc_txd_buf_len = 0;
    g_uc_txd_line_next = 0;
}


/****************************************
函数：char get_pc_data()
功能：读取数据
说明：读取缓冲区内数据
****************************************/
char get_pc_data(unsigned char *uc_p_rxd_data_vaild_flag)
{
    unsigned char uc_temp_char;   // 临时变量，保存从接收缓冲区中读取的数据
    static unsigned char uc_rxd_line_out = 0;  // 模拟队列出队指针
    
    if((uc_rxd_line_out != g_uc_rxd_line_in) && (uc_rxd_line_out < RXD_BUF)) // 如果出队列指针不等于入队列指针且有效，表示接收缓冲区在有数据，否则为空
    {
        uc_temp_char = g_uc_rxd_buf[uc_rxd_line_out++];   // 读取一个字节的数据，并指向下一个数据
        if(uc_rxd_line_out == RXD_BUF)
        {
            uc_rxd_line_out = 0;    // 如果出队列指针超出了缓冲区尾地址，则指向首地址
        }
        *uc_p_rxd_data_vaild_flag = 1;   // 数据有效标志置1
        if(uc_rxd_line_out == g_uc_rxd_line_in)
        {
            g_uc_rxd_data_flag = 0;   // 如果出队列指针等于入队列指针，表示接收缓冲区已空
        }
        return uc_temp_char;    // 返回读取的数据
    }
    else
    {
        g_uc_rxd_data_flag = 0;          //接收缓冲区空
        *uc_p_rxd_data_vaild_flag = 0;    //数据有效标志位清零
        return '\0'; 
    }
}


/****************************************
函数：void send_data(unsigned char *uc_p_send_data, unsigned char uc_data_len)
功能：发送数据
说明：把发送数据存入发送缓冲区内并启动发送
****************************************/
void send_data(unsigned char *uc_p_send_data, unsigned char uc_data_len)
{
    unsigned char i;
    i = 0;
    
    if(1 == g_uc_txd_busy_flag) // 串口忙位为1时表示串口正在使用，正在使用则退出，避免后续的数据覆盖正在发送的数据
    {
       longjmp(buf, 1);     // 表示未能进入串口发送操作，转移
    }
    for(i = 0; i < uc_data_len; i++)
    {
        g_uc_txd_buf[i] = uc_p_send_data[i];  // 把要发送的数据转移到发送缓冲区
    }
    g_uc_txd_busy_flag = 1;  // 数据准备完毕，把串口忙位置1，表示正在使用串口
    g_uc_txd_buf_len = uc_data_len;  // 设置发送缓冲区中数据的长度
    g_uc_txd_line_next = 0;    // 把数据缓冲区的指针指到第一个数据
    
    SCI0CR2 = 0X6C;                  //开SCI中断
    SCI0DRL = g_uc_txd_buf[0];     //启动数据发送
}


/****************************************
函数：void rev_data()
功能：判断数据
说明：读取缓冲区内数据并且比较判断是否为所需指令
调用函数：void parse_data(unsigned char *a_uc_rev_data)-------- main.c文件中
          char get_pc_data(unsigned char *uc_p_rxd_data_vaild_flag)-------- main.c文件中
****************************************/
void rev_data()
{
    unsigned char i, uc_temp_data, uc_rxd_data_vaild_flag;
    unsigned char a_uc_rev_data[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    i = 0;
    uc_rxd_data_vaild_flag = 0;
    uc_temp_data = 1;
    
    while('\0' != uc_temp_data)         //接收缓冲区不为空
    {
        uc_temp_data = get_pc_data(&uc_rxd_data_vaild_flag);   //从接收缓冲区读取数据
        if(uc_rxd_data_vaild_flag)                   //判断此次数据是否有效
        {
            a_uc_rev_data[i++] = uc_temp_data;         //接收标志位清零
        }
    }
    if((i - 3) != (a_uc_rev_data[1] - '0'))
    {
        send_data("W", 1);                 //数据长度错误，发送错误信息
        longjmp(buf, 3);                   //转移到错误处理体
    }

    parse_data(a_uc_rev_data);      //解析数据包
    if(0 == strcmp(a_uc_rev_data, START_FLAG))   //判断数据是否为所需数据
    {
        g_uc_cmd_flag = 1;              //从PC获取的数据有效标志位置1                 
    }
}


/****************************************
函数：void parse_data(unsigned char *a_uc_rev_data)
功能：解析数据
说明：解析得到的数据包
调用函数：void send_data(unsigned char *uc_p_send_data, unsigned char uc_data_len)-------- main.c文件中
****************************************/
void parse_data(unsigned char *a_uc_rev_data)
{
    unsigned char i;
    unsigned char uc_data_len;
    
    uc_data_len = a_uc_rev_data[1] - '0';   //获得数据长度   
    if((DATA_FRA_START == a_uc_rev_data[0]) && (DATA_FRA_END == a_uc_rev_data[uc_data_len + 2]))    //判断数据有效性  
    {
        for(i = 0; i < uc_data_len; i++)
        {
            a_uc_rev_data[i] = a_uc_rev_data[i + 2];  //去除多余信息，保留所需数据
        }
        a_uc_rev_data[i] = '\0';
    }
    else
    {
        longjmp(buf, 2);            //数据格式错误，并转移到错误处理体内
    }
}


/****************************************
函数：char partition(unsigned int *ui_p_arr, char c_low, char c_high)
功能：分区
说明：快速排序的分区
****************************************/
char partition(unsigned int *ui_p_arr, char c_low, char c_high)
{
    unsigned int ui_key;
    
    ui_key = ui_p_arr[c_low];     //备份比较值
    while(c_low < c_high)
    {
        while(c_low < c_high && ui_p_arr[c_high] >= ui_key)
        {
            c_high--;     //向前搜索更小值
        }
        if(c_low < c_high)
        {
            ui_p_arr[c_low++] = ui_p_arr[c_high];   //如果找到更小值交换位置
        }
        while(c_low < c_high && ui_p_arr[c_low] <= ui_key)
        {
            c_low++;    //向后搜索更大值
        }
        if(c_low < c_high)
        {
            ui_p_arr[c_high--] = ui_p_arr[c_low];    //如果找到更大值交换位置
        }
    }
    ui_p_arr[c_low] = ui_key;       //确定切割元素的位置
    return c_low;
}


/****************************************
函数：char partition(unsigned int *ui_p_arr, char c_low, char c_high)
功能：快速排序
说明：对数据进行快排
调用函数：char partition(unsigned int *ui_p_arr, char c_low, char c_high)-------- main.c文件中  
****************************************/
void quick_sort(unsigned int *ui_p_arr, char c_start, char c_end)
{
    char c_pos;
    
    if(c_start < c_end)
    {
        c_pos = partition(ui_p_arr, c_start, c_end);    //分区，左边的都小于参考值，右边的都大于
        quick_sort(ui_p_arr, c_start, c_pos-1);    //递归进行操作
        quick_sort(ui_p_arr, c_pos + 1, c_end);
    }
}


/****************************************
函数：void map(unsigned int *p_ui_AD1, unsigned int *p_ui_AD0)
功能：映射
说明：将数字量映射为模拟量
****************************************/
void map(struct AD *p_AD1_data, struct AD *p_AD0_data)
{
   
    p_AD0_data->ui_ana = (unsigned int)(p_AD0_data->ui_dig * 1.224);
    p_AD1_data->ui_ana = (unsigned int)(p_AD1_data->ui_dig * 1.224); //采样模拟电压值 
}


/****************************************
函数：void sys_init()
功能：初始化
说明：AD, SCI, PIT, 端口，时钟的初始化
调用函数：void set_bus_clk_32m()-------- CRG.c文件中                
          void ADC_init()-------- ADC.c文件中                     
          void SCI_init(32, 9600)-------- SCI.c文件中             
          void PIT_init()-------- PIT.c文件中
****************************************/
void sys_init()
{
    DDRA = 0XFF;                    //端口的初始化         
    DDRB = 0X0F;
    DDRT = 0XFF;
    PORTA = 0X00;
    PORTB = 0XFF;
    PTT = 0X00; 
    
    IRQCR = 0xC0;                   //外部中断的初始化                  
    
    set_bus_clk_32m();              //利用PLL进行系统时钟超频设置  
    ADC_init();                     //AD初始化
    SCI_init(32, 9600);             //SCI初始化,（采用RS232协议）
    PIT_init(); 
}


/****************************************
函数：void convert_num(unsigned char *p_uc_AD1, unsigned char *p_uc_AD0, unsigned int ui_AD1, unsigned int ui_AD0)
功能：数值转换
说明：AD采样值转换为拆开的单个数字
****************************************/
void convert_num(struct AD *p_AD1_data, struct AD *p_AD0_data, unsigned char uc_num)
{
    unsigned char i;
    i = 0;
    
    for(i = 0; i < uc_num; i++)
    {
       p_AD1_data->a_uc_ana[i] = p_AD1_data->ui_ana % 10;         //整数拆分并存于数据采样数组内          
       p_AD1_data->a_uc_dig[i] = p_AD1_data->ui_dig % 10;
       p_AD1_data->ui_ana /= 10;
       p_AD1_data->ui_dig /= 10;
       
       p_AD0_data->a_uc_ana[i] = p_AD0_data->ui_ana % 10;         //整数拆分并存于数据采样数组内          
       p_AD0_data->a_uc_dig[i] = p_AD0_data->ui_dig % 10;
       p_AD0_data->ui_ana /= 10;
       p_AD0_data->ui_dig /= 10;
    }   
}


/****************************************
函数：void show_value(unsigned char const *p_uc_AD1, unsigned char const *p_uc_AD0)
功能：数值显示
说明：AD采样值显示在数码管上
****************************************/
void show_value(struct AD const *p_AD1_data, struct AD const *p_AD0_data)
{
    unsigned char i, j;
    static unsigned char s_uc_show = 0;
    i = 0;
    j = 0;
    
    if(g_uc_key_flag)                     //判断按键是否转换了状态
    {
        g_uc_key_flag = 0;
        s_uc_show = ~s_uc_show;          //按键按下转换显示
    }
   
    for(i = 0; i< 100; i++)
    {
        PTT = 0X01;
        for(j = 0; j < 4; j++)                 //低四位显示模拟量
        {   
            if(!s_uc_show)
            {
                PORTA = g_c_uc_led_num_table[p_AD1_data->a_uc_ana[j]];  //转换为数码管数字  
            }
            else
            {
                PORTA = g_c_uc_led_num_table[p_AD0_data->a_uc_ana[j]];
            }
            if(3 == j)               //添加小数点
            {
                PORTA |= 0X80;
            }
            delay(1);
            PTT <<= 1;        
        }

        for(j = 0; j < 4; j++)                  //高四位显示数字量
        {   
            if(!s_uc_show)
            {
                PORTA = g_c_uc_led_num_table[p_AD1_data->a_uc_dig[j]];       //转换为数码管数字              
            }
            else
            {
                PORTA = g_c_uc_led_num_table[p_AD0_data->a_uc_dig[j]];
            }
            if(0 == j)              
            {
                PORTA |= 0X80;               //添加分隔符
            }     
            delay(1);
            PTT <<= 1;        
        }
    }   
}


/****************************************
函数：void data_to_send_convert(struct AD const *p_AD1_data, struct AD const *p_AD0_data, unsigned char *a_uc_ad_data )
功能：AD数值转换
说明：AD数值转换为ASCII码并添加格式
****************************************/
void data_to_send_convert(struct AD const *p_AD1_data, struct AD const *p_AD0_data, unsigned char *a_uc_ad_data )
{    

    signed char i;
    i = 0;
    
    for(i = 3; i >= 0; i--)      //AD数据存储
    {
        a_uc_ad_data[3 - i] = p_AD1_data->a_uc_dig[i] + '0';   //转换为ASCII码
        a_uc_ad_data[13 - i] = p_AD0_data->a_uc_dig[i] + '0';   //转换为ASCII码
    }
    
    a_uc_ad_data[4] = ' ';
    a_uc_ad_data[14] = ' ';

    for(i = 3; i >= 0; i--)      
    {
        a_uc_ad_data[8 - i] = p_AD1_data->a_uc_ana[i] + '0';
        a_uc_ad_data[18 - i] = p_AD0_data->a_uc_ana[i] + '0';
    }
    
    a_uc_ad_data[9] = '\n';
    a_uc_ad_data[19] = '\n';
    a_uc_ad_data[20] = '\0';   
}

/****************************************
函数：void delay(void)
功能：延时
说明：延时5000*loop_times个机器周期
****************************************/
void delay(unsigned int loop_times)
{
    unsigned int loop_i,loop_j;
    
    for (loop_i=0; loop_i< loop_times; loop_i++) 
    {
        for (loop_j=0; loop_j<2000;loop_j++) 
        {
            ;
        }
    }
}