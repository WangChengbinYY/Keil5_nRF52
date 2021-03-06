/*
*********************************************************************************************************
*
*    模块名称 : 外部硬件中断配置
*    文件名称 : Leo_INT
*    版    本 : V1.0
*    说    明 : 外部硬件中断设置相关
*
*    修改记录 :
*        版本号    日期          作者     
*        V1.0    2019-01-14     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/


#ifndef LEO_INT_H
#define LEO_INT_H

#include "Leo_Includes.h"


#ifdef __cplusplus
extern "C" {
#endif

/* SDCard 存储暂停中断 初始化  */  
uint8_t ucINTInital_SDCard(void);    
    

/* SDCard 存储暂停中断 启动  */
uint8_t ucINTStart_SDCard(void);  
    

/* 1pps 中断初始化 */
uint8_t ucINTInital_PPS(void);    
    

/* 1pps 中断 启动 */
uint8_t ucINTStart_PPS(void);    

#ifdef __cplusplus
}
#endif


#endif 







