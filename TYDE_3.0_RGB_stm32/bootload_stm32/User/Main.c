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

uint8_t value[1024];

/*****************************************************************************
函数名称 : NVIC_Configuration
功能描述 : NVIC配置
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0);
  
  //Configure the NVIC Preemption Priority Bits
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}
/*****************************************************************************
函数名称 : Exist_MainProgram
功能描述 : 判断程序是否存在
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : JumpToApp
功能描述 : 程序跳转
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : update_firmware
功能描述 : 固件升级
输入参数 : 无
返回参数 : 无
使用说明 : 无
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
函数名称 : main
功能描述 : 主函数
输入参数 : 无
返回参数 : 无
使用说明 : 无
*****************************************************************************/
void main (void)
{
  /*
  //解锁(必须运行在RAM状态下)
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
  
  //判断程序是否存在
  app_address = FIREWARE_ADDR_L;

  if(Exist_MainProgram((uint32_t *)app_address) == 1)
  {
    JumpToApp(app_address); 
  }

  while(1)
  {

  }
}

