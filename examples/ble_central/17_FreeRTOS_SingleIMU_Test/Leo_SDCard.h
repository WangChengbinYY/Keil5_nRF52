/*
*********************************************************************************************************
*
*    模块名称 : SDCard数据存储
*    文件名称 : Leo_FreeRTOS_TASK
*    版    本 : V1.0
*    说    明 : 利用FatFS实现数据的SDCard存储
*
*    修改记录 :
*        版本号    日期          作者     
*        V1.0    2019-01-19     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/

#ifndef LEO_SDCARD_H
#define LEO_SDCARD_H

#include "Leo_Includes.h"



#ifdef __cplusplus
extern "C" {
#endif

	

/*-----------------------------------------------------------------------*/
/* SDCard卡读写操作的初始化                                               */
/*-----------------------------------------------------------------------*/
uint8_t ucSDCard_INIT(void);
    
/*------------------------------------------------------------
 *建立SDCard存储文件                                          
 *  成功返回:0; 失败返回:1;                                    
 *------------------------------------------------------------*/
//uint8_t ucSDCard_OpenFile(void);
    
/*-----------------------------------------------------------------------*/
/* 往 SDCard卡 内写入数据                                                 */
/*----------------------------------------------------------------------*/
uint8_t ucSDCard_SaveData(uint8_t* mbuffer,UINT mLength);
    
/*-----------------------------------------------------------------------*/
/* SDCard卡 操作完成后，关闭文件                                          */
/*----------------------------------------------------------------------*/
uint8_t ucSDCard_CloseFile(void);		

	
    

#ifdef __cplusplus
}
#endif


#endif  