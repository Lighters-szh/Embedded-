#ifndef __INCLUDE_H__
#define __INCLUDE_H__

//==========================================================================
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
//#include 	<intrins.h>
#include	<ctype.h>
#include        "stm32f10x.h"

#include        "user_flash.h"


//(7+1K)+28K+28K
#define         BASIC_FLASH_ADDR                        0x08000000

#define         OFFSET_PARA                             0x1c00                                
#define         OFFSET_FIRMWARE_L                       0x2000
#define         OFFSET_FIRMWARE_H                       0x9000

#define         PARA_ADDR                               (BASIC_FLASH_ADDR + OFFSET_PARA)
#define         FIREWARE_ADDR_L                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_L)   //
#define         FIREWARE_ADDR_H                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_H)   //

#define         FIREWARE_UPDATE_FLAG                    0x55555555

#endif