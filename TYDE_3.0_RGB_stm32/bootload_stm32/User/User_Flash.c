/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: User_Flash.c
**描        述: Timer用户函数
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
#define HAL_USER_FLASH_GLOBAL
#include "include.h"


/*****************************************************************************
函数名称 : FlashProgramU8
功能描述 : 对Flash进行字节编程
输入参数 : pulData:写入的源数据
           ulAddress:FLASH的目标起始地址
           ulCount:写入的字节数
返回参数: SUCCESS:写入成功
          ERROR:写入失败
*****************************************************************************/
static uint8_t FlashProgramU32(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t Wrdata;
  
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                      //调整字节个数为4的倍数   
  
  while(ulCount) 
  {
    Wrdata  = *pulData ++;                                             
    Wrdata |= (*pulData ++) << 8;
    Wrdata |= (*pulData ++) << 16;
    Wrdata |= (*pulData ++) << 24;                                 //将4个字节组合成一个字数据    
    
    FLASH_ProgramWord(ulAddress,Wrdata);
    
    ulAddress += 4;                                                 
    ulCount -= 4;
  }
  
  return(SUCCESS);
}
/*****************************************************************************
函数名称 : VerifyData
功能描述 : 校验写入FLASH的数据
输入参数 : pulData       源数据
           ulAddress     FLASH的起始地址
           ulCount       校验的字节数
返回参数 : true          校验成功
           false         校验失败
*****************************************************************************/
static uint8_t VerifyData(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t ReData;
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                         /* 调整校验的字节个数为4的倍数  */

  while(ulCount) 
  {
    ReData = (*(__IO uint32_t*)ulAddress);
    
    if(*pulData != (ReData &0xff)) 
    {                         /* 校验字节数据                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 8) &0xff)) 
    {                         /* 校验字节数据                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 16) &0xff)) 
    {                        /* 校验字节数据                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 24) &0xff)) 
    {                        /* 校验字节数据                 */
      return ERROR;
    }
    
    ulAddress += 4;                                                     /* 调整指定的Flash地址          */
    ulCount -= 4;                                                       /* 调整待校验的字节个数         */
    pulData++;                                                          /* 调整源数据指针               */
  }
  return SUCCESS;
}
/*****************************************************************************
函数名称 : Write_Flash
功能描述 : 对Flash进行字节编程
输入参数 : Addr_Sour:写入的源数据
           Addr_Obj:FLASH的目标起始地址
           Length:写入的字节数
返回参数 : SUCCESS:写入成功
            ERROR:写入失败
使用说明 : 无
*****************************************************************************/
uint8_t Write_Flash(uint32_t Addr_Obj,uint8_t *Addr_Sour,uint16_t Length)
{
  FLASH_Unlock();
  
  if(FlashProgramU32(Addr_Sour,Addr_Obj,Length) == ERROR) 
  {
    FLASH_Lock();
    return ERROR;
  } 
  
  if(VerifyData(Addr_Sour,Addr_Obj,Length) == ERROR) 
  {
    FLASH_Lock();
    return ERROR;
  } 
  FLASH_Lock();
  
  return SUCCESS;
}
/*****************************************************************************
函数名称 : Earse_Flash
功能描述 : 对Flash擦除
输入参数 : Addr_Obj:FLASH的目标起始地址
返回参数 : SUCCESS:擦除成功
            ERROR:擦除失败
使用说明 : 无
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj)
{
  FLASH_Unlock();
  
  if(FLASH_ErasePage(Addr_Obj) != FLASH_COMPLETE)
  {
    FLASH_Lock();
    return ERROR;
  }
  
  FLASH_Lock();
  
  return SUCCESS;
}
/*****************************************************************************
函数名称 : Read_Flash
功能描述 : 读取FLASH的数据
输入参数 : pulData:源数据
           ulAddress:FLASH的起始地址
           Length:读取的字节数
返回参数 : true:读取成功
           false:读取失败
*****************************************************************************/
uint8_t Read_Flash(uint8_t *pulData,uint32_t ulAddress,uint32_t Length)
{
  uint32_t ReData;
  uint32_t cnt = 0;
  
  while(cnt < Length)
  {
    ReData = (*(__IO uint32_t*)ulAddress);
    
    *pulData ++ = (ReData & 0xff); 
    cnt ++;
    if(cnt >= Length)
      break;
    
    *pulData ++ = ((ReData >> 8) & 0xff);
    cnt ++;
    if(cnt >= Length)
      break;

    *pulData ++ = (ReData >> 16) & 0xff;
    cnt ++;
    if(cnt >= Length)
      break;

    *pulData ++ = (ReData >> 24) & 0xff;
    cnt ++;
   
    ulAddress += 4;
  }
  return SUCCESS;
}

