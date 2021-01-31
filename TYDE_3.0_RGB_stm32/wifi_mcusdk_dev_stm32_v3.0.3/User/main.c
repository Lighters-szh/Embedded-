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

#include "wifi.h"


TYPE_BUFFER_S FlashBuffer;


unsigned char water_level;
unsigned char light;
unsigned char sleep;
unsigned char work_state;
unsigned short temper_set;
unsigned short temper_current;
  
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
�������� : NVIC_Configuration
�������� : NVIC����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void NVIC_Configuration(void)
{
#ifdef ENABLE_BOOT
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,OFFSET_FIRMWARE_L);
#else
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0);
#endif
  
  //Configure the NVIC Preemption Priority Bits
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
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
�������� : Bsp_Init
�������� : �����ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Bsp_Init(void)
{
  USART1_Init(9600);
  USART3_Init(9600);
  //
  TIM2_Count_Init();
  TIM3_Count_Init(99,719);//1ms
  //
  KEY_Init();
  RGB_LED_Init();
}


/*****************************************************************************
�������� : wifi_work_state_led
�������� : wifi״̬led����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void wifi_work_state_led(void)
{
    uint8_t wifi_state = mcu_get_wifi_work_state();
    static uint8_t count = 0;

    switch(wifi_state)
    {
    case SMART_CONFIG_STATE:
      if(Get_Time3_Value() % 2 == 0)
      {
        LED_Control(LED1,1);
      }
      else
      {
        LED_Control(LED1,0);
      }
      break;
      
    case AP_STATE:
      if(Get_Time3_Value() == 0)
      {
        LED_Control(LED1,1);
      }
      else if(Get_Time3_Value() == 6)
      {
        LED_Control(LED1,0);
      }
      break;
      
    case WIFI_NOT_CONNECTED:
        LED_Control(LED1,0);
      break;
      
    case WIFI_CONNECTED:
      LED_Control(LED1,1);
      break;
      
    default:
      LED_Control(LED1,0);
      break;
    }
}


/*****************************************************************************
�������� : main
�������� : ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void main(void)
{
  uint8_t wifi_state;
  static uint8_t cnt = 0;

  Read_Flash((unsigned char *)&FlashBuffer,PARA_ADDR,sizeof(FlashBuffer));

  if((FlashBuffer.magic_code != 0) || (FlashBuffer.magic_code != 0xffffffff))
  {
    Earse_Flash(PARA_ADDR);
    memset(&FlashBuffer,0,sizeof(FlashBuffer));
    FlashBuffer.bright_value = 0xff;
    
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
    {
      
    }
  }
  
  RCC_Configuration();
  
  NVIC_Configuration();
  
  Bsp_Init();
  
  //����wifi��ʼ��  
  wifi_protocol_init();

  //RGB�ϵ����
  RGB_Power_Control();
  
  while(1)
  {
    wifi_uart_service();
    
    Key_Scan();
    
    wifi_work_state_led();
  }
	
}




