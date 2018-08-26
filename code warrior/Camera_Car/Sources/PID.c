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
#include "Speed.h"

#define T 10              //��������Ϊ10ms
#define left_maxpwm  1321 //����ת��(��ת)
#define right_maxpwm 1099 //����ת��(��ת)
#define middle_pwm   1210 //����ת��(ֱ��)
#define k_pwm        
#define speed_end    50



/***************��������********************/
extern unsigned char lost_line_flag;
extern int average;
extern unsigned char left_flag;
extern unsigned char right_flag;
extern unsigned char black_line[40];
//unsigned int g_i_CurrentVelocity = 0;
extern unsigned int g_i_CurrentVelocity ;

//************��������**********************

unsigned char KP = 1;  //���PD����
unsigned char KD = 0;  


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
//PID������ʼ��
//*****************************************************
void IncPIDInit(void)
{
		sptr->LastError = 0; //Error[-1]
		sptr->PrevError = 0; //Error[-2]
		sptr->Proportion = P_DATA; //�������� Proportional Const
		sptr->Integral = I_DATA; //���ֳ���Integral Const
		sptr->Derivative = D_DATA; //΢�ֳ��� Derivative Const
		sptr->SetPoint =95; //Ŀ����100
}


//*****************************************************
//����ʽPID�������
//*****************************************************
int IncPIDCalc(int NextPoint)
{
		int iError, iIncpid; //��ǰ���
		iError = sptr->SetPoint - NextPoint; //��������
		iIncpid = sptr->Proportion * iError //E[k]��
				- sptr->Integral * sptr->LastError //E[k��1]��
				+ sptr->Derivative * sptr->PrevError; //E[k��2]��
		sptr->PrevError = sptr->LastError; //�洢�������´μ���
		sptr->LastError = iError;
		return(iIncpid); //��������ֵ
}


/*************************************************************
������void steer_PD(void);
���ܣ������PD����
˵������ȡ��������,�ж�����ǶȲ����ж����PD����
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
          
        if(average>35)                             //������ǳ���������޴��
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
            else                                       //ֱ�߻�С��
            { 
                steer_PWM = 1210;    
            }
            
            PWMDTY67 = steer_PWM;  
    }
}