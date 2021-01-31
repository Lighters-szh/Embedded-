/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: main.c
**��        ��: ������
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
#define MAIN_GLOBAL
#include "include.h"

uint8_t value[1024];

/*****************************************************************************
�������� : NVIC_Configuration
�������� : NVIC����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0);
  
  //Configure the NVIC Preemption Priority Bits
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}
/*****************************************************************************
�������� : Exist_MainProgram
�������� : �жϳ����Ƿ����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Exist_MainProgram(uint32_t *pulApp)
{
  // See if the first location is 0xfffffffff or something that does not
  // look like a stack pointer, or if the second location is 0xffffffff or
  // something that does not look like a reset vector.
  //
  if((pulApp[0] == 0xffffffff) || ((pulApp[0] & 0xfff00000) != 0x20000000) ||
     (pulApp[1] == 0xffffffff) || ((pulApp[1] & 0xfff00001) != 0x08000001))
  {
    return 0;
  }
  else
    return 1;  
}
/*****************************************************************************
�������� : RCC_Configuration
�������� : ʱ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void RCC_Configuration(void)
{
  //����DMAʱ�� 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
  
  //Enable AFIO clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //ʹ��JTAG����ΪGPIO  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
/*****************************************************************************
�������� : JumpToApp
�������� : ������ת
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void JumpToApp(uint32_t app_address)                  
{
  typedef  void (*pFunction)(void);
  pFunction Jump_To_Application;
  uint32_t JumpAddress;
  
  JumpAddress = *(__IO uint32_t*) (app_address + 4);
  Jump_To_Application = (pFunction) JumpAddress;
  
  __set_MSP(*(__IO uint32_t*) app_address);
  Jump_To_Application();
}
/*****************************************************************************
�������� : update_firmware
�������� : �̼�����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t update_firmware(void)
{
  uint32_t src,obj;
  uint8_t t;
  uint32_t offset;
  
  src = FIREWARE_ADDR_H;
  obj = FIREWARE_ADDR_L;
  
  for(t = 0; t < 28; t ++)
  {
    offset = t * 1024;
    if(Read_Flash(value,src + offset,1024) == ERROR)
   {
      return ERROR;
    }
    
    if(Earse_Flash(obj + offset) == ERROR)
    {
      return ERROR;
    }
    
    if(Write_Flash(obj + offset,value,1024) == ERROR)
    {
      return ERROR;
    }
  }
}
/*****************************************************************************
�������� : Reset
�������� : ϵͳ��λ
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Reset(void)
{
  NVIC_SystemReset();
}
/*****************************************************************************
�������� : main
�������� : ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void main (void)
{
  /*
  //����(����������RAM״̬��)
  FLASH_Unlock();
  FLASH_ReadOutProtection(DISABLE); 
  */
  uint32_t magic_code;
  uint32_t app_address;
  
  RCC_Configuration();
  
  NVIC_Configuration();
  
  //
  Read_Flash((unsigned char *)&magic_code,PARA_ADDR,4);
    
  if(magic_code == FIREWARE_UPDATE_FLAG)
  {
    if(update_firmware() != SUCCESS)
    {
      Reset();
    }
  }  
  
  //�жϳ����Ƿ����
  app_address = FIREWARE_ADDR_L;

  if(Exist_MainProgram((uint32_t *)app_address) == 1)
  {
    JumpToApp(app_address); 
  }

  while(1)
  {

  }
}

