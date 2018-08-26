/***************************************************************************
    文件名称: PIT.c

    项目名称: Test_V1.0 

    作者:     季齐

    版本:     V1.0
    
    说明:     完成PIT定时1秒的初始化

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      
#include "PIT.h"

void PIT_init(void) 
{
    PITCFLMT = 0x00;      //禁用PIT模块
    PITCE_PCE0 = 1;       //使用通道0
    PITMUX = 0x00;        //通道0使用0为基准计数
    PITMTLD0 = 0x3F;      //t=(PITMTLD0+1)*(PITLD0+1)/f
    PITLD0 = 0xC34F;      //定时0.1s，取PITMTLD=63(0x27),PITLD=49999(0x270F)
    PITINTE = 0x01;       //通道0中断使能打开
    PITCFLMT = 0x80;      //使能PIT模块
}