/*************************************************
    文件名称：main.h
    
    功能描述：包含了main.c所需的宏定义，函数声明等
              
    作者：    彭亮
    
    编译环境：Freescale CodeWarrier V5.0 For S12
    
    版本：    V1.2
    
    说明：
    
    修改记录：
    
    创建日期：2016-10-10
**************************************************/
#ifndef CRG_H
#define CRG_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "CRG.h"
#include "ADC.h"
#include "SCI.h"
#include "PIT.h"


#define START_FLAG "gh"
#define RXD_BUF 100
#define TXD_BUF 100
#define DATA_FRA_START 0X24
#define DATA_FRA_END 0X21


/*******************************函数声明*********************************/

void sys_init();
void delay(unsigned int loop_times);
void get_AD(struct AD *p_AD1_data, struct AD *p_AD0_data);
void data_to_send_convert(struct AD const *p_AD1_data, struct AD const *p_AD0_data, unsigned char *a_uc_ad_data);
void show_value(struct AD const *p_AD1_data, struct AD const *p_AD0_data);
void convert_num(struct AD *p_AD1_data, struct AD *p_AD0_data, unsigned char uc_num);
void map(struct AD *p_AD1_data, struct AD *p_AD0_data);
void quick_sort(unsigned int *ui_p_arr, char c_start, char c_end);
void rev_data();
char get_pc_data();
void send_data(unsigned char *uc_p_send_data, unsigned char uc_data_len);
void parse_data(unsigned char *a_uc_rev_data);
void rxd_txd_buf_init(); 

/************************* 全局变量声明*******************************/
volatile unsigned char g_uc_get_flag;
volatile unsigned char g_uc_sec_flag;         //中断标志位，加volatile防止编译器过度优化
volatile unsigned char g_uc_key_flag;

unsigned char g_uc_rxd_buf[RXD_BUF];    //接收数据缓冲区，模拟队列
unsigned char g_uc_rxd_data_flag;    //接收区有数据标志
unsigned char g_uc_rxd_line_in;    //模拟入队指针

unsigned char g_uc_txd_buf[TXD_BUF];   //发送数据缓冲区，模拟队列
unsigned char g_uc_txd_busy_flag;     //串口忙标志
unsigned char g_uc_txd_buf_len;      //发送缓冲区数据长度
unsigned char g_uc_txd_line_next;   //指向发送缓冲区下一个元素   

unsigned char g_uc_cmd_flag;      //PC发送的字符正确标志位

jmp_buf buf;    //错误处理结构体

unsigned char const g_c_uc_led_num_table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                          0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};                                           
                                    
struct AD{
    unsigned int ui_ana;
    unsigned int ui_dig;
    unsigned char a_uc_ana[4];
    unsigned char a_uc_dig[4];
};    //AD结构体

#endif