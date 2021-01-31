/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: User_Uart.c
**��        ��: uart�û�����
**ʹ �� ˵ �� : ��
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
**
**-----------------------------------------------------------------------------
******************************************************************************/
#define HAL_USER_UART_GLOBAL
#include "include.h"

#include "wifi.h"

//#define         DEBUG_IN_USART1                                               //ͨ��USB-TTL����


/*****************************************************************************
�������� : USART1_Init
�������� : USART1��ʼ��
������� : BaudRate:������
���ز��� : ��ʼ���ɹ�/ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t USART1_Init(uint32_t BaudRate)
{
  USART_InitTypeDef USART_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if(BaudRate > 115200) 
  {                                            //������̫�ߣ����󷵻�
    return(ERROR);
  }
  
  // Enable GPIOA clocks 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  // Enable USART1 clocks 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /*
  *  USART1_TX -> PA9 , USART1_RX -> PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure); 
  //
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  //
  USART_ClearFlag(USART1,USART_FLAG_TC);
  
  //Enable the USART1 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                             //ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                     //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                            //ָ����Ӧ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               //�����ж�
  NVIC_Init(&NVIC_InitStructure);                                               //д������
  
  USART_Cmd(USART1, ENABLE);
  
  return(SUCCESS);
}
/*****************************************************************************
�������� : USART3_Init
�������� : USART3��ʼ��
������� : BaudRate:������
���ز��� : ��ʼ���ɹ�/ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t USART3_Init(uint32_t BaudRate)
{
  USART_InitTypeDef USART_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if(BaudRate > 115200) 
  {                                            //������̫�ߣ����󷵻�
    return(ERROR);
  }
  
  // Enable GPIOA clocks 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  // Enable USART1 clocks 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  /*
  *  USART3_TX -> PB10 , USART13_RX -> PB11
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);		   
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure); 
  //
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
  //
  USART_ClearFlag(USART3,USART_FLAG_TC);
  
  //Enable the USART3 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                             //ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                     //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                            //ָ����Ӧ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               //�����ж�
  NVIC_Init(&NVIC_InitStructure);                                               //д������
  
  USART_Cmd(USART3, ENABLE);
  
  return(SUCCESS);
}
extern uint8_t Timer4_Value;
uint16_t uart_input_count;
uint8_t uart_input_finish;
/*****************************************************************************
�������� : USART1_IRQHandler
�������� : USART1�����жϺ���
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void USART1_IRQHandler(void)
{
  uint8_t ch;
  
  //�����ж�
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    ch = USART_ReceiveData(USART1);
    
#ifdef DEBUG_IN_USART1
    //�����յ��ֽ����ݴ���uart_receive_input
    uart_receive_input(ch);
#endif
  }
}

/*****************************************************************************
�������� : USART1_IRQHandler
�������� : USART1�����жϺ���
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void USART3_IRQHandler(void)
{
  uint8_t ch;
  
  //�����ж�
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    ch = USART_ReceiveData(USART3);

#ifndef DEBUG_IN_USART1
    //�����յ��ֽ����ݴ���uart_receive_input
    uart_receive_input(ch);                                                      //ʹ��TIMx
#endif
  }
}
/*****************************************************************************
�������� : Uart_PutChar
�������� : ���ڷ��ͺ���
������� : ch:����������
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart_PutChar(uint8_t ch)
{
#ifdef DEBUG_IN_USART1
  //����
  USART_SendData(USART1,ch);
  //
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
#else
  //����
  USART_SendData(USART3,ch);
  //
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
#endif
}
