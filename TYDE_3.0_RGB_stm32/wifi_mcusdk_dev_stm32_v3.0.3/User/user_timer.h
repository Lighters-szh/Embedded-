#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__


/*****************************************************************************
�������� : Get_Count_Value
�������� : ��ȡTim_Countֵ
������� : ��
���ز��� : Time2����ֵ
ʹ��˵�� : ��
*****************************************************************************/
unsigned  int Get_Count_Value(void);

/*****************************************************************************
�������� : TIM2_Count_Init
�������� : TIM2������ʼ��
������� : arr:������ʼֵ/pse:Ԥ��Ƶֵ
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void TIM2_Count_Init(void);
/*****************************************************************************
�������� : TIM3_Count_Init
�������� : TIM3������ʼ��
������� : arr:������ʼֵ/pse:Ԥ��Ƶֵ
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void TIM3_Count_Init(unsigned short int arr,unsigned short int psc);

/*****************************************************************************
�������� : Get_Time3_Value
�������� : ��ȡTim3ֵ
������� : ��
���ز��� : Time3����ֵ
ʹ��˵�� : ��
*****************************************************************************/
unsigned  int Get_Time3_Value(void);


#endif

