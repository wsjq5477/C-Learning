/********************************
PID.H
·���滮����ͷ�ļ�
********************************/
#ifndef _PID_H_
#define _PID_H_

//***************��������*************//
extern void speed_PID(void);
extern void steer_PD(void);

//*****************************************************
//����PID�ṹ��
//*****************************************************
typedef struct PID
{
		int SetPoint; //�趨Ŀ�� Desired Value
		double Proportion; //�������� Proportional Const
		double Integral; //���ֳ��� Integral Const
		double Derivative; //΢�ֳ��� Derivative Const
		int LastError; //Error[-1]
		int PrevError; //Error[-2]
} PID;
//*****************************************************
//������غ�
//*****************************************************
#define P_DATA 1
#define I_DATA 1
#define D_DATA 0

//*****************************************************
//����PIDʵ��
//*****************************************************
static PID sPID;
static PID *sptr = &sPID;  

/*****************************��������***********************************/ 
void IncPIDInit(void);
int IncPIDCalc(int NextPoint);




#endif