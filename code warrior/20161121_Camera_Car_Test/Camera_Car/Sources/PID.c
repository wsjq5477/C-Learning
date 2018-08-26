/***************************************************************************
    文件名称: PID.c

    作者:     智能车项目摄像头组

    版本:     2016-11-10
    
    说明:     完成舵机的PD控制和电机的PID控制

    修改记录: 无
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "PID.h"
#include "Image.h"

#define T 10              //采样周期为10ms
#define left_maxpwm  1530 //极限转角(左转)
#define right_maxpwm 1370 //极限转角(右转)


extern unsigned char speed[4];
extern unsigned char Kp;  //电机PID数据
extern unsigned char Ki;  //Ki = 1/Ti
extern unsigned char Kd;  //Kd = Td

extern unsigned char KP;  //舵机PD数据
extern unsigned char KD;  

extern unsigned char black_line[40];

/*************************************************************
函数：void speed_PID(void);
功能：完成速度的PID控制
说明：通过编码器测得电机转速并完成PID控制
      1.首先使Ki=Kd=0,从小到大调节Kp;
      2.当调节到合适值时,令Kp=5*Kp/6,Ki从大到小整定
      3.达到稳定后调节Kd,一般在(1/3-1/4)/Ki
*************************************************************/ 
void speed_PID()
{
    unsigned int D_PWM;
    unsigned int PWM;
    signed int A,B,C;
    
    float t_Kp = (float)Kp;
    float t_Ki = (float)Ki;
    float t_Kd = (float)Kd;
    
    
    A = (signed int)(t_Kp+t_Kp*T*t_Ki+t_Kd/T);
    B = (signed int)(t_Kp+2*t_Kp*t_Kd/T);
    C = (signed int)(t_Kp*t_Kd/T);
    
    PWM = PWMDTY0;
    D_PWM = A*(speed[3]-speed[2])+B*(speed[2]-speed[1])+C*(speed[1]-speed[0]);
    PWM = D_PWM + PWM;
    
    //PWMDTY0 = PWM;     
}


/*************************************************************
函数：void steer_PD(void);
功能：舵机的PD控制
说明：读取黑线数据,判定弯道角度并进行舵机的PD控制
*************************************************************/ 
void steer_PD()
{
    unsigned int steer_PWM;
    unsigned char average;
    unsigned char left_f,right_f;
    
    route_extract();                           //提取黑线中心坐标
    
    if(black_line[29]>black_line[9])           //左弯道
    {
        average = black_line[29]-black_line[9];//共40行，用30行和10行的黑线坐标差作为斜率
        left_f = 1;
    }
    else
    { 
        average = black_line[9]-black_line[29];
        right_f = 1;
    }
    
    if(average>60)                             //若弯道角超出舵机极限打角,这里极限打角需测量
    {                                
        if(left_f==1)
        {
            steer_PWM = left_maxpwm;           //左转打到极限角度
            left_f = 0;
        }
        if(right_f==1)
        {
            steer_PWM = right_maxpwm;          //右转打到极限角度
            left_f = 0;
        }
    }
    else
    {
        steer_PWM = KP*(black_line[9]+black_line[19]+black_line[29])+KD*average;
    }
    
    PWMDTY67 = steer_PWM;
}







