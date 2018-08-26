/***************************************************************************
    �ļ�����: PID.c

    ����:     ���ܳ���Ŀ����ͷ��

    �汾:     2016-11-10
    
    ˵��:     ��ɶ����PD���ƺ͵����PID����

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "PID.h"
#include "Image.h"
#include "Route.h"
#include "SCI.h"


#define T 10              //��������Ϊ10ms
#define left_maxpwm  1345 //����ת��(��ת)
#define right_maxpwm 1075 //����ת��(��ת)
#define middle_pwm   1210 //����ת��(ֱ��)



/***************��������********************/
extern int average;
extern unsigned char left_edge_flag;
extern unsigned char right_edge_flag;
extern unsigned char left_flag;
extern unsigned char right_flag;
extern unsigned char black_line[40];

//*************��ʱ����������*****************
extern unsigned char speed[4];
extern unsigned char Kp;  //���PID����
extern unsigned char Ki;  //Ki = 1/Ti
extern unsigned char Kd;  //Kd = Td


//************��������**********************
unsigned char KP = 1;  //���PD����
unsigned char KD = 0;  
unsigned int steer_PWM = 1360;

/*************************************************************
������void speed_PID(void);
���ܣ�����ٶȵ�PID����
˵����ͨ����������õ��ת�ٲ����PID����
      1.����ʹKi=Kd=0,��С�������Kp;
      2.�����ڵ�����ֵʱ,��Kp=5*Kp/6,Ki�Ӵ�С����
      3.�ﵽ�ȶ������Kd,һ����(1/3-1/4)/Ki
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
    D_PWM = A*(speed[3]-speed[2])+B*(speed[2]-speed[1])+C*(speed[1]-speed[0]);	//�������
    PWM = D_PWM + PWM;	
    
    //PWMDTY0 = PWM;     
}


/*************************************************************
������void steer_PD(void);
���ܣ������PD����
˵������ȡ��������,�ж�����ǶȲ����ж����PD����
*************************************************************/ 
void steer_PD()
{
    unsigned int think_PWM;
    route_judge();
    
    //*************�����߼��Ͷ�������***************
    if(left_edge_flag==1)
    {
        steer_PWM = left_maxpwm;
        left_edge_flag = 0;
    }
    else if(right_edge_flag==1)
    {
        steer_PWM = right_maxpwm;
        right_edge_flag = 0;
    }
    else
    {
    
        if(average>35)                             //������ǳ���������޴��,���Ｋ�޴�������
        {                                
            if(left_flag==1)
            {
                steer_PWM = left_maxpwm ;          //��ת�򵽼��޽Ƕ�
                left_flag = 0;
            }
            if(right_flag==1)
            {
                steer_PWM = right_maxpwm;          //��ת�򵽼��޽Ƕ�
                right_flag = 0;
            }
        }      
        else if(average>3)                         //�������ƫ����ֵ��ƫת
        {
            //think_PWM = middle_pwm +(left_maxpwm-right_maxpwm)*����λ��/90
            if(left_flag==1)
            {
                think_PWM = 1210 + 3*average;
                left_flag = 0;
            }
            if(right_flag==1)
            {
                think_PWM = 1210 - 3*average;
                right_flag = 0;
            }
           
            steer_PWM = think_PWM + 0*(think_PWM-steer_PWM);       
        }
        else                                       //ֱ�߻�С��
        { 
            steer_PWM = 1210;    
        }
        
        
        PWMDTY67 = steer_PWM;
    }
      
}







