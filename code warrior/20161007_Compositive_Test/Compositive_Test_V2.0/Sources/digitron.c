#include <hidef.h>      
#include "derivative.h"     

#define  SEG_DD    DDRA                                               //段码口方向
#define  DIG_DD    DDRH                                               //位码口方向
#define  SEG_NUM   PORTA                                              //段码数据
#define  DIG_NUM   PTH                                                //位码数据

/**********************************全局变量声明*******************************************/
const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //共阴数码管字形表定义
                                                
