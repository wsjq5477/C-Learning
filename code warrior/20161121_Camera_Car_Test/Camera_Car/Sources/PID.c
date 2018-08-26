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

#define T 10              //��������Ϊ10ms
#define left_maxpwm  1530 //����ת��(��ת)
#define right_maxpwm 1370 //����ת��(��ת)


extern unsigned char speed[4];
extern unsigned char Kp;  //���PID����
extern unsigned char Ki;  //Ki = 1/Ti
extern unsigned char Kd;  //Kd = Td

extern unsigned char KP;  //���PD����
extern unsigned char KD;  

extern unsigned char black_line[40];

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
    D_PWM = A*(speed[3]-speed[2])+B*(speed[2]-speed[1])+C*(speed[1]-speed[0]);
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
    unsigned int steer_PWM;
    unsigned char average;
    unsigned char left_f,right_f;
    
    route_extract();                           //��ȡ������������
    
    if(black_line[29]>black_line[9])           //�����
    {
        average = black_line[29]-black_line[9];//��40�У���30�к�10�еĺ����������Ϊб��
        left_f = 1;
    }
    else
    { 
        average = black_line[9]-black_line[29];
        right_f = 1;
    }
    
    if(average>60)                             //������ǳ���������޴��,���Ｋ�޴�������
    {                                
        if(left_f==1)
        {
            steer_PWM = left_maxpwm;           //��ת�򵽼��޽Ƕ�
            left_f = 0;
        }
        if(right_f==1)
        {
            steer_PWM = right_maxpwm;          //��ת�򵽼��޽Ƕ�
            left_f = 0;
        }
    }
    else
    {
        steer_PWM = KP*(black_line[9]+black_line[19]+black_line[29])+KD*average;
    }
    
    PWMDTY67 = steer_PWM;
}







