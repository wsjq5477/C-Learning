/***************************************************************************
    �ļ�����: PWM.c

    ��Ŀ����: Compositive_Test_V3.0.mpc 

    ����:     ����

    �汾:     V1.0
    
    ˵��:     ���PWM�ĳ�ʼ��

    �޸ļ�¼: ��
***************************************************************************/
#include <hidef.h>     
#include "derivative.h"      /* derivative-specific definitions */
#include "PWM.h"

void PWM_init(void) 
{
    PWME = 0x00;              //��ֹPWM
    
    PWMPRCLK = 0x06;          //CLOCKAԤ��Ƶ����Ϊ3
    
    PWMSCLA = 125;            //SA��Ƶ��������Ϊ125
    
    PWMCLK = 0x02;			      //ͨ��0,1ѡ��ClockSA��ΪPWMʱ��
    
    PWMPOL = 0xFF;            //�������忪ʼʱ����ߵ�ƽ    
    
    PWMCAE = 0x00;            //�����
    
    PWMCTL = 0x08;            //����ͨ��0��ͨ��1����
    
    PWMPER1 = 200;			      //ͨ��1���ڼĴ�������Ϊ200
                               
    PWMDTY1 = 0;			        //ͨ��1ռ�ձȼĴ�������Ϊ0%
    
    PWME = 0x03;              //ʹ��PWM                                                       
}