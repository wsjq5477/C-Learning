/********************************
PID.H
路径规划部分头文件
********************************/
#ifndef _PID_H_
#define _PID_H_

//***************函数声明*************//
extern void speed_PID(void);
extern void steer_PD(void);

//*****************************************************
//定义PID结构体
//*****************************************************
typedef struct PID
{
		int SetPoint; //设定目标 Desired Value
		double Proportion; //比例常数 Proportional Const
		double Integral; //积分常数 Integral Const
		double Derivative; //微分常数 Derivative Const
		int LastError; //Error[-1]
		int PrevError; //Error[-2]
} PID;
//*****************************************************
//定义相关宏
//*****************************************************
#define P_DATA 1
#define I_DATA 1
#define D_DATA 0

//*****************************************************
//声明PID实体
//*****************************************************
static PID sPID;
static PID *sptr = &sPID;  

/*****************************函数声明***********************************/ 
void IncPIDInit(void);
int IncPIDCalc(int NextPoint);




#endif