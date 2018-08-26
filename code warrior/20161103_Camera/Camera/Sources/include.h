#ifndef _INCLUDE_H
#define _INCLUDE_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/*****************************函数声明***********************************/
#include  "CRG.h"
#include  "SCI.h"
#include  "sys_init.h"
#include  "stdio.h"
#include  "Image.h"




/*********声明*************************/
//#define  INTERVAL_NUM  5        //间隔行数采集图像数据
#define  ROW           40       //40行，每行90个数据
#define  COLUMN        90  
#define  ROW_MIN       5       //行数最大和最小
#define  ROW_MAX       205
#define  black 0
#define  white 255
#endif