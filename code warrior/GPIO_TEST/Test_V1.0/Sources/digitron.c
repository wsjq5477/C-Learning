#include <hidef.h>      
#include "derivative.h"     

#define  SEG_DD    DDRA                                               //����ڷ���
#define  DIG_DD    DDRH                                               //λ��ڷ���
#define  SEG_NUM   PORTA                                              //��������
#define  DIG_NUM   PTH                                                //λ������

/**********************************ȫ�ֱ�������*******************************************/
const unsigned char g_LED_Seg_Table[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                           0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00};     
                                                //������������α���
                                                
