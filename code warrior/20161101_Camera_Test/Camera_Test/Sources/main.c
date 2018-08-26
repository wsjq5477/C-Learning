#include <hidef.h>       
#include <MC9S12XS128.h> 
#pragma LINK_INFO DERIVATIVE "mc9s12xs128" 
 
/*手创科技摄像头信号采集程序*/ 
 
//-淘宝店:http://sochange.taobao.com/----------- 
 
//客服联系人:刘生：15812442294    技术联系人：廖生：15920998230 
 
//欢迎联系技术 qq:974205767（因工作繁忙，一般晚上在线） 
 
//采集到的数据在二维数组 ta[lie][hang_end]里; 
//接口定义: 行同步接 PT2   场同步接PT1  信号线是接 A0  

  #define lie_end  260  //摄像头的极限行 
  #define hang_end  150 //每一行最多的点 
  #define lie  52   //采集的行数       
 
 
  void delay(unsigned int num)    
  { 
     unsigned int i,j;  
      for(i=0;i<num;i++)  
         for(j=0;j<58;j++);  
  }  
   
  unsigned char c_lie=0,g_lie=0; 
  unsigned int lie_count=0,hang_count=0, chesudu; 
    
  unsigned char ta[lie][hang_end]; //采集到的数据存放的二维数组 
 
  unsigned int get_n[]={  16,29,41,52,62,71,79,86,92,98, 
                         103,108,112,116,120,124,128,132,135,138, 
                         141,144,147,150,153,156,159,162,165,168, 
                         170,172,174,176,178,180,182,184,186,188, 
                         190,192,194,196,198,200,202,204,206,208, 
                         210,211,212,242,243,245,246,247,248,249,250};//定距采集图像 
 
void chaopin(void);  //超频程序        
void TIM_init(void);//定时程序 
void shijian(void); 
         
void main(void)  
{      
    
  DDRA=0X00; //各端口数据方向初始化 
  DDRJ=0X02; 
  DDRK=0X30; 
  DDRM=0X00; 
  DDRJ=0XBF; 
  chaopin(); //超频函数初始化     
  TIM_init();//计数器函数初始化   
  shijian(); 
  for(;;)  
   
  { 
   
  //把要处理的函数放在这里 
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
   PITLD0=9999;  //1  毫秒 
   PITMTLD0=5;       
   PITMUX=0X00; 
   PITINTE_PINTE0=1;      
   PITCFLMT_PITE=1;            
} 
 
 
 
//=======图像采集中断函数，已经完全可以采集图像的========= 
 
 #pragma CODE_SEG NON_BANKED 
 
 void interrupt 10 IC2ISR(void)      //行中断 
 {
     
     TFLG1_C2F=1;//清行中断 
   
     if(lie_count==get_n[c_lie])       //定距采集图像的判断条件 
     {   
         delay(1);//黑线中心矫正，确保不采集到消隐信号 
               
         for(hang_count=0;hang_count<=hang_end;hang_count++)   //读取一行的数据
         {
             ta[c_lie][hang_count]=PORTA_PA0;//采集数据的接口      
         }      
         c_lie++;//二维数组行自加 
     } 
     lie_count++;//摄像头行自加  
 } 
   
 
  
void interrupt 9 IC1ISR(void)     //场中断，各数据清零   
{            
   TFLG1_C1F=1; //清场中断 
   hang_count=0;
   lie_count=0; 
   c_lie=0;    
 
}   
 
#pragma CODE_SEG DEFAULT  
 
 





 


