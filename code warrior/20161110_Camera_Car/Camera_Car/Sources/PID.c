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


#define T 10              //采样周期为10ms
#define left_maxpwm  1530 //极限转角(左转)
#define right_maxpwm 1130 //极限转角(右转)
#define middle_pwm  1370 //中线转角(直线)



/***************引用声明********************/
extern int average;
extern unsigned char left_edge_flag;
extern unsigned char right_edge_flag;
extern unsigned char left_flag;
extern unsigned char right_flag;
extern unsigned char black_line[40];

//*************暂时不调用数据*****************
extern unsigned char speed[4];
extern unsigned char Kp;  //电机PID数据
extern unsigned char Ki;  //Ki = 1/Ti
extern unsigned char Kd;  //Kd = Td


//************调试数据**********************
unsigned char KP = 1;  //舵机PD数据
unsigned char KD = 0;  
unsigned int steer_PWM = 1370;

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
    unsigned int think_PWM;
    //unsigned char sdata;
    route_judge();
    
    //*************边沿线检测和丢现修正***************
    if(left_edge_flag==1)
    {
        steer_PWM = left_maxpwm;
        left_edge_flag = 0;
        //average = 0;
    }
    else if(right_edge_flag==1)
    {
        steer_PWM = right_maxpwm;
        right_edge_flag = 0;
        //average = 0;
    }
    else
    {
    
        if(average>35)                             //若弯道角超出舵机极限打角,这里极限打角需测量
        {                                
            if(left_flag==1)
            {
                steer_PWM = left_maxpwm ;          //左转打到极限角度
                left_flag = 0;
                //average = 0;
            }
            if(right_flag==1)
            {
                steer_PWM = right_maxpwm;          //右转打到极限角度
                right_flag = 0;
                //average = 0;
            }
        }      
        else if(average>3)                         //弯道超出偏移阈值才偏转
        {
            //think_PWM = middle_pwm +(left_maxpwm-right_maxpwm)*黑线位置/90
            if(left_flag==1)
            {
                think_PWM = 1370 + 24*average/5;
                left_flag = 0;
            }
            if(right_flag==1)
            {
                think_PWM = 1370 - 24*average/5;
                right_flag = 0;
            }
           
            steer_PWM = think_PWM + 0*(think_PWM-steer_PWM);       
        }
        else                                       //直线或小弯
        { 
            steer_PWM = 1370;    
        }
        
        
        PWMDTY67 = steer_PWM;
        
        /***************发送PID计算的PWM至上位机*******************                    
        sdata = 'a';
        send_data(sdata);            
        sdata =  48+(unsigned char)(average/10%10);
        send_data(sdata);
        sdata =  48+(unsigned char)(average%10);  
        send_data(sdata);
           
        sdata = 'p';
        send_data(sdata);
        sdata =  48+(unsigned char)(steer_PWM/1000);
        send_data(sdata);
        sdata =  48+(unsigned char)(steer_PWM/100%10);             //将结果拆分并发送
        send_data(sdata);                     
        sdata =  48+(unsigned char)(steer_PWM/10%10);
        send_data(sdata);
        sdata =  48+(unsigned char)(steer_PWM%10);  
        send_data(sdata);
        sdata = '\n';
        send_data(sdata);
        
           
        for(;;) 
        {
            ;
        } 
        
        ************************************************************/
    }
      
}







