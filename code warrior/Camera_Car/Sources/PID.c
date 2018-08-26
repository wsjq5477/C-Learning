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
#include "Route.h"
#include "SCI.h"
#include "Speed.h"

#define T 10              //采样周期为10ms
#define left_maxpwm  1321 //极限转角(左转)
#define right_maxpwm 1099 //极限转角(右转)
#define middle_pwm   1210 //中线转角(直线)
#define k_pwm        
#define speed_end    50



/***************引用声明********************/
extern unsigned char lost_line_flag;
extern int average;
extern unsigned char left_flag;
extern unsigned char right_flag;
extern unsigned char black_line[40];
//unsigned int g_i_CurrentVelocity = 0;
extern unsigned int g_i_CurrentVelocity ;

//************定义声明**********************

unsigned char KP = 1;  //舵机PD数据
unsigned char KD = 0;  


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
		int temp_pwm = 0;
		temp_pwm = IncPIDCalc(g_i_CurrentVelocity);
		if(temp_pwm > 3)
		{
			temp_pwm  = 3;
		}
		if(temp_pwm < -3)
	  	{
	  	 	temp_pwm = -3;
		}
		if(temp_pwm >= -3 && temp_pwm <= 3)
		{
			 PWMDTY1 += temp_pwm;     
		}
		
   
}

//*****************************************************
//PID参数初始化
//*****************************************************
void IncPIDInit(void)
{
		sptr->LastError = 0; //Error[-1]
		sptr->PrevError = 0; //Error[-2]
		sptr->Proportion = P_DATA; //比例常数 Proportional Const
		sptr->Integral = I_DATA; //积分常数Integral Const
		sptr->Derivative = D_DATA; //微分常数 Derivative Const
		sptr->SetPoint =95; //目标是100
}


//*****************************************************
//增量式PID控制设计
//*****************************************************
int IncPIDCalc(int NextPoint)
{
		int iError, iIncpid; //当前误差
		iError = sptr->SetPoint - NextPoint; //增量计算
		iIncpid = sptr->Proportion * iError //E[k]项
				- sptr->Integral * sptr->LastError //E[k－1]项
				+ sptr->Derivative * sptr->PrevError; //E[k－2]项
		sptr->PrevError = sptr->LastError; //存储误差，用于下次计算
		sptr->LastError = iError;
		return(iIncpid); //返回增量值
}


/*************************************************************
函数：void steer_PD(void);
功能：舵机的PD控制
说明：读取黑线数据,判定弯道角度并进行舵机的PD控制
*************************************************************/ 
void steer_PD()
{
    
    unsigned int steer_PWM;
    unsigned int think_PWM;
    unsigned int last_PWM;
    route_judge();
    if(lost_line_flag==1)
    {
      lost_line_flag = 0;
      
    }
    else
    {
          
        if(average>35)                             //若弯道角超出舵机极限打角
            {                                
                if(left_flag==1)
                {
                    steer_PWM = left_maxpwm ;          //左转打到极限角度
                    left_flag = 0;
                }
                if(right_flag==1)
                {
                    steer_PWM = right_maxpwm;          //右转打到极限角度
                    right_flag = 0;
                }
            }      
            else if(average>3)                         //弯道超出偏移阈值才偏转
            {
                //think_PWM = middle_pwm +(left_maxpwm-right_maxpwm)*黑线位置/90
                if(left_flag==1)
                {
                    think_PWM = 1219 + 19*average/6;       //3-1219;35-1320
                    left_flag = 0;
                }
                if(right_flag==1)
                {
                    think_PWM = 1200 - 19*average/6;       //3-1200;35-1099
                    right_flag = 0;
                }
                last_PWM = PWMDTY67;
                steer_PWM = last_PWM +(think_PWM-last_PWM);                 
            }
            else                                       //直线或小弯
            { 
                steer_PWM = 1210;    
            }
            
            PWMDTY67 = steer_PWM;  
    }
}