 /***********************************************************
    �ļ�����:CRG.c
    ��Ŀ����:run.mcp
    �� ��:   ��˼�������ܳ�������
    �� ��:   2014.10.26
    ˵ ��:   ����Ƶ��ѡ��
    �޸ļ�¼:
***********************************************************/
#include  <hidef.h>            /*  common defines and macros   */
#include  "derivative.h"       /*  derivative-specific definitions   */

#include  "CRG.h"              /*  some deal about PLL  */



/**********************************������������*******************************************/
extern void SetBusCLK_32M(void);                         //��������Ƶ��Ϊ32M
extern void SetBusCLK_40M(void);                         //��������Ƶ��Ϊ40M
extern void SetBusCLK_48M(void);                         //��������Ƶ��Ϊ48M
extern void SetBusCLK_64M(void);                         //��������Ƶ��Ϊ64M
extern void SetBusCLK_80M(void);                         //��������Ƶ��Ϊ80M

/*************************************************************************************************************
  ��������  : void SetBusCLK_32M(void)
  
  ���ܼ��  : ��������Ƶ��Ϊ32M
  
  ��ڲ���  : ��
  
  ���ڲ���  : �� 
***************************************************************************************************************/
void SetBusCLK_32M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //��ʹ�����໷ʱ��
   
    PLLCTL_PLLON=1;                        //���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0x40 | 0x0F;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
                               /*  
                                *  0b01000011
                                *    ||||||||__
                                *    |||||||___ 
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______
                                *    |||_______SYNR[5:0]���ڸı����໷ʱ�ӱ�Ƶ 
                                *    ||________VCOFRQ[1:0]=0:1,����VCO_clock��48~80M֮��
                                *    |_________VCOFRQ[1:0]���ڿ���VCO_clock������
                                */
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0x40 | 0x07;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz;
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0x40 | 0x03;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
    #endif    
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=64MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]���ڸı����໷ʱ�ӷ�Ƶ 
                            *    ||________REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12M֮��
                            *    |_________REFFRQ[1:0]�������òο�ʱ��REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //������ʱ���ȴ�ʱ��Ƶ���ȶ�
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //ʱ��Ƶ�����ȶ������໷Ƶ������
    {
        ;
    }
    
    CLKSEL_PLLSEL =1;                      //ʹ�����໷ʱ��
}
/*************************************************************************************************************
  ��������  : void SetBusCLK_40M(void)
  
  ���ܼ��  : ��������Ƶ��Ϊ40M
  
  ��ڲ���  : ��
  
  ���ڲ���  : �� 
***************************************************************************************************************/
void SetBusCLK_40M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //��ʹ�����໷ʱ��
   
    PLLCTL_PLLON=1;                        //���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0x40 | 0x13;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
                               /*  
                                *  0b01000011
                                *    ||||||||__
                                *    |||||||___ 
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______
                                *    |||_______SYNR[5:0]���ڸı����໷ʱ�ӱ�Ƶ 
                                *    ||________VCOFRQ[1:0]=0:1,����VCO_clock��48~80M֮��
                                *    |_________VCOFRQ[1:0]���ڿ���VCO_clock������
                                */
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0x40 | 0x09;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0x40 | 0x04;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
    #endif    
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=80MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]���ڸı����໷ʱ�ӷ�Ƶ 
                            *    ||________REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12M֮��
                            *    |_________REFFRQ[1:0]�������òο�ʱ��REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //������ʱ���ȴ�ʱ��Ƶ���ȶ�
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //ʱ��Ƶ�����ȶ������໷Ƶ������
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //ʹ�����໷ʱ��
}
/*************************************************************************************************************
  ��������  : void SetBusCLK_48M(void)
  
  ���ܼ��  : ��������Ƶ��Ϊ48M
  
  ��ڲ���  : ��
  
  ���ڲ���  : �� 
***************************************************************************************************************/
void SetBusCLK_48M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //��ʹ�����໷ʱ��
   
    PLLCTL_PLLON=1;                        //���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
                
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x17;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
                               /*  
                                *  0b01000011
                                *    ||||||||__
                                *    |||||||___ 
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______
                                *    |||_______SYNR[5:0]���ڸı����໷ʱ�ӱ�Ƶ 
                                *    ||________VCOFRQ[1:0]=0:1,����VCO_clock��48~80M֮��
                                *    |_________VCOFRQ[1:0]���ڿ���VCO_clock������
                                */
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x0B;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x05;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
    #endif    
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=96MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]���ڸı����໷ʱ�ӷ�Ƶ 
                            *    ||________REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12M֮��
                            *    |_________REFFRQ[1:0]�������òο�ʱ��REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //������ʱ���ȴ�ʱ��Ƶ���ȶ�
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //ʱ��Ƶ�����ȶ������໷Ƶ������
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //ʹ�����໷ʱ��
}
/*************************************************************************************************************
  ��������  : void SetBusCLK_64M(void)
  
  ���ܼ��  : ��������Ƶ��Ϊ64M
  
  ��ڲ���  : ��
  
  ���ڲ���  : �� 
***************************************************************************************************************/

void SetBusCLK_64M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //��ʹ�����໷ʱ��
   
    PLLCTL_PLLON=1;                        //���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
                
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x1F;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
                               /*  
                                *  0b01000011
                                *    ||||||||__
                                *    |||||||___ 
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______
                                *    |||_______SYNR[5:0]���ڸı����໷ʱ�ӱ�Ƶ 
                                *    ||________VCOFRQ[1:0]=0:1,����VCO_clock��48~80M֮��
                                *    |_________VCOFRQ[1:0]���ڿ���VCO_clock������
                                */
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x0F;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x07;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
    #endif 
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=128MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]���ڸı����໷ʱ�ӷ�Ƶ 
                            *    ||________REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12M֮��
                            *    |_________REFFRQ[1:0]�������òο�ʱ��REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //������ʱ���ȴ�ʱ��Ƶ���ȶ�
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //ʱ��Ƶ�����ȶ������໷Ƶ������
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //ʹ�����໷ʱ��
}
/*************************************************************************************************************
  ��������  : void SetBusCLK_80M(void)
  
  ���ܼ��  : ��������Ƶ��Ϊ80M
  
  ��ڲ���  : ��
  
  ���ڲ���  : �� 
***************************************************************************************************************/


void SetBusCLK_80M(void)
{  
    
    CLKSEL_PLLSEL = 0;                     //��ʹ�����໷ʱ��
   
    PLLCTL_PLLON=1;                        //���໷��·����
    
    /* SYNR = (VCOFRQ1 | VCOFRQ0 | SYNR5 | SYNR4 | //
                 SYNR3 | SYNR2 | SYNR1 | SYNR0 | //
                )   */ 
    
    #ifdef OSC_FREQ_4MHz
        SYNR =0xC0 | 0x27;     // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
                               /*  
                                *  0b01000011
                                *    ||||||||__
                                *    |||||||___ 
                                *    ||||||____
                                *    |||||_____ 
                                *    ||||______
                                *    |||_______SYNR[5:0]���ڸı����໷ʱ�ӱ�Ƶ 
                                *    ||________VCOFRQ[1:0]=0:1,����VCO_clock��48~80M֮��
                                *    |_________VCOFRQ[1:0]���ڿ���VCO_clock������
                                */
    #endif
    
    #ifdef OSC_FREQ_8MHz     
        SYNR =0xC0 | 0x13;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
    #endif
    
    #ifdef OSC_FREQ_16MHz     
        SYNR =0xC0 | 0x09;      // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
    #endif 
                            
    /* REFDV = (REFFRQ1 | REFFRQ1 | REFDV5 | REFDV4 | //
                 REFDV3 | REFDV2 | REFDV1 | REFDV0 | //
                )   */                                              
    REFDV=0x80 | 0x01;
                           // VCO_clock=2*osc_clock*(1+SYNR)/(1+REFDV)=160MHz; 
                           /*  
                            *  0b10000001
                            *    ||||||||__
                            *    |||||||___ 
                            *    ||||||____ 
                            *    |||||_____ 
                            *    ||||______
                            *    |||_______REFDV[5:0]���ڸı����໷ʱ�ӷ�Ƶ 
                            *    ||________REFFRQ[1:0]=1:0,��Ϊ�ο�ʱ����6~12M֮��
                            *    |_________REFFRQ[1:0]�������òο�ʱ��REF_clock,REF_clock=osc_clock/(REFDV+1)=16/1+1=8M                            */
                                                 
    POSTDIV=0x00;                          //VCO_clock = PLL_clock
    
    _asm(nop);                             //������ʱ���ȴ�ʱ��Ƶ���ȶ�
    
    _asm(nop);
    
    while(!(CRGFLG_LOCK==1))               //ʱ��Ƶ�����ȶ������໷Ƶ������
        {
            ;
        }
    
    CLKSEL_PLLSEL =1;                      //ʹ�����໷ʱ��
}

