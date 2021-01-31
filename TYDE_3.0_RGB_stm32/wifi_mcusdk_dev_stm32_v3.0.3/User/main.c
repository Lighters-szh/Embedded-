/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: main.c
**描        述: 主函数
**使 用 说 明 : 无
**
**
**--------------当前版本修订---------------------------------------------------

** 版  本: v2.0
** 日　期: 2016年3月29日
** 描　述: 1:优化代码结构
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
函数名称 : Reset
功能描述 : 系统复位
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void Reset(void)
{
  NVIC_SystemReset();
}


/*****************************************************************************
函数名称 : NVIC_Configuration
功能描述 : NVIC配置
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : RCC_Configuration
功能描述 : 时钟配置
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void RCC_Configuration(void)
{
  //启动DMA时钟 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
  
  //Enable AFIO clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //使能JTAG口作为GPIO  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}



/*****************************************************************************
函数名称 : Bsp_Init
功能描述 : 外设初始化
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : wifi_work_state_led
功能描述 : wifi状态led控制
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : main
功能描述 : 主函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
  
  //调用wifi初始化  
  wifi_protocol_init();

  //RGB上电控制
  RGB_Power_Control();
  
  while(1)
  {
    wifi_uart_service();
    
    Key_Scan();
    
    wifi_work_state_led();
  }
	
}




