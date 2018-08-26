#include <hidef.h>       
#include <MC9S12XS128.h> 
#pragma LINK_INFO DERIVATIVE "mc9s12xs128" 
 
/*�ִ��Ƽ�����ͷ�źŲɼ�����*/ 
 
//-�Ա���:http://sochange.taobao.com/----------- 
 
//�ͷ���ϵ��:������15812442294    ������ϵ�ˣ�������15920998230 
 
//��ӭ��ϵ���� qq:974205767��������æ��һ���������ߣ� 
 
//�ɼ����������ڶ�ά���� ta[lie][hang_end]��; 
//�ӿڶ���: ��ͬ���� PT2   ��ͬ����PT1  �ź����ǽ� A0  

  #define lie_end  260  //����ͷ�ļ����� 
  #define hang_end  150 //ÿһ�����ĵ� 
  #define lie  52   //�ɼ�������       
 
 
  void delay(unsigned int num)    
  { 
     unsigned int i,j;  
      for(i=0;i<num;i++)  
         for(j=0;j<58;j++);  
  }  
   
  unsigned char c_lie=0,g_lie=0; 
  unsigned int lie_count=0,hang_count=0, chesudu; 
    
  unsigned char ta[lie][hang_end]; //�ɼ��������ݴ�ŵĶ�ά���� 
 
  unsigned int get_n[]={  16,29,41,52,62,71,79,86,92,98, 
                         103,108,112,116,120,124,128,132,135,138, 
                         141,144,147,150,153,156,159,162,165,168, 
                         170,172,174,176,178,180,182,184,186,188, 
                         190,192,194,196,198,200,202,204,206,208, 
                         210,211,212,242,243,245,246,247,248,249,250};//����ɼ�ͼ�� 
 
void chaopin(void);  //��Ƶ����        
void TIM_init(void);//��ʱ���� 
void shijian(void); 
         
void main(void)  
{      
    
  DDRA=0X00; //���˿����ݷ����ʼ�� 
  DDRJ=0X02; 
  DDRK=0X30; 
  DDRM=0X00; 
  DDRJ=0XBF; 
  chaopin(); //��Ƶ������ʼ��     
  TIM_init();//������������ʼ��   
  shijian(); 
  for(;;)  
   
  { 
   
  //��Ҫ����ĺ����������� 
  } 
 
} 
 
 void chaopin(void)//64M 
{   
    CLKSEL=0X00;    
    PLLCTL_PLLON=1;    
    SYNR =0xc0 | 0x07;                         
    REFDV=0xc0 | 0x01;  
    POSTDIV=0x00;        
    _asm(nop);           
    _asm(nop); 
    _asm(nop); 
    _asm(nop); 
    while(!(CRGFLG_LOCK==1));    
    CLKSEL_PLLSEL =1;                     
} 

void shijian(void) 
{
   PITCFLMT_PITE=0;        
   PITCE_PCE0=1;           
   PITLD0=9999;  //1  ���� 
   PITMTLD0=5;       
   PITMUX=0X00; 
   PITINTE_PINTE0=1;      
   PITCFLMT_PITE=1;            
} 
 
 
 
//=======ͼ��ɼ��жϺ������Ѿ���ȫ���Բɼ�ͼ���========= 
 
 #pragma CODE_SEG NON_BANKED 
 
 void interrupt 10 IC2ISR(void)      //���ж� 
 {
     
     TFLG1_C2F=1;//�����ж� 
   
     if(lie_count==get_n[c_lie])       //����ɼ�ͼ����ж����� 
     {   
         delay(1);//�������Ľ�����ȷ�����ɼ��������ź� 
               
         for(hang_count=0;hang_count<=hang_end;hang_count++)   //��ȡһ�е�����
         {
             ta[c_lie][hang_count]=PORTA_PA0;//�ɼ����ݵĽӿ�      
         }      
         c_lie++;//��ά�������Լ� 
     } 
     lie_count++;//����ͷ���Լ�  
 } 
   
 
  
void interrupt 9 IC1ISR(void)     //���жϣ�����������   
{            
   TFLG1_C1F=1; //�峡�ж� 
   hang_count=0;
   lie_count=0; 
   c_lie=0;    
 
}   
 
#pragma CODE_SEG DEFAULT  
 
 





 


