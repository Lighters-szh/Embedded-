/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: User_Flash.c
**��        ��: Timer�û�����
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
#define HAL_USER_FLASH_GLOBAL
#include "include.h"


/*****************************************************************************
�������� : FlashProgramU8
�������� : ��Flash�����ֽڱ��
������� : pulData:д���Դ����
           ulAddress:FLASH��Ŀ����ʼ��ַ
           ulCount:д����ֽ���
���ز���: SUCCESS:д��ɹ�
          ERROR:д��ʧ��
*****************************************************************************/
static uint8_t FlashProgramU32(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t Wrdata;
  
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                      //�����ֽڸ���Ϊ4�ı���   
  
  while(ulCount) 
  {
    Wrdata  = *pulData ++;                                             
    Wrdata |= (*pulData ++) << 8;
    Wrdata |= (*pulData ++) << 16;
    Wrdata |= (*pulData ++) << 24;                                 //��4���ֽ���ϳ�һ��������    
    
    FLASH_ProgramWord(ulAddress,Wrdata);
    
    ulAddress += 4;                                                 
    ulCount -= 4;
  }
  
  return(SUCCESS);
}
/*****************************************************************************
�������� : VerifyData
�������� : У��д��FLASH������
������� : pulData       Դ����
           ulAddress     FLASH����ʼ��ַ
           ulCount       У����ֽ���
���ز��� : true          У��ɹ�
           false         У��ʧ��
*****************************************************************************/
static uint8_t VerifyData(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t ReData;
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                         /* ����У����ֽڸ���Ϊ4�ı���  */

  while(ulCount) 
  {
    ReData = (*(__IO uint32_t*)ulAddress);
    
    if(*pulData != (ReData &0xff)) 
    {                         /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 8) &0xff)) 
    {                         /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 16) &0xff)) 
    {                        /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 24) &0xff)) 
    {                        /* У���ֽ�����                 */
      return ERROR;
    }
    
    ulAddress += 4;                                                     /* ����ָ����Flash��ַ          */
    ulCount -= 4;                                                       /* ������У����ֽڸ���         */
    pulData++;                                                          /* ����Դ����ָ��               */
  }
  return SUCCESS;
}
/*****************************************************************************
�������� : Write_Flash
�������� : ��Flash�����ֽڱ��
������� : Addr_Sour:д���Դ����
           Addr_Obj:FLASH��Ŀ����ʼ��ַ
           Length:д����ֽ���
���ز��� : SUCCESS:д��ɹ�
            ERROR:д��ʧ��
ʹ��˵�� : ��
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
�������� : Earse_Flash
�������� : ��Flash����
������� : Addr_Obj:FLASH��Ŀ����ʼ��ַ
���ز��� : SUCCESS:�����ɹ�
            ERROR:����ʧ��
ʹ��˵�� : ��
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
�������� : Read_Flash
�������� : ��ȡFLASH������
������� : pulData:Դ����
           ulAddress:FLASH����ʼ��ַ
           Length:��ȡ���ֽ���
���ز��� : true:��ȡ�ɹ�
           false:��ȡʧ��
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

