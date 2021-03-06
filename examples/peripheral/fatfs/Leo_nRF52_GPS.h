/******************** (C) COPYRIGHT 2018 王成宾********************
 * 文件名  ：Leo_nRF52_GPS     
 * 平台    ：nRF52832 
 * 描述    ：通过串口接收GPS数据并解析  
 * 作者    ：王成宾
**********************************************************************/

#ifndef LEO_NRF52_GPS_H
#define LEO_NRF52_GPS_H


#include "Leo_nRF52_config.h"
#include "app_uart.h"
#include "minmea.h"


#ifdef __cplusplus
extern "C" {
#endif
    

//@brief GPS数据缓存区结构体定义
/*--------------------------------------------------------------------------*/
struct Leo_gps_buffer{
    int8_t      buffer[LEO_GPS_BUFFER_MAXLENGTH];       //GPS串口数据存储缓存区  char字符
    int8_t*     pSave;                                  //缓存区中当前存储的位置
    int8_t*     pLoad;                                  //读取缓存区的位置
    int8_t*     pStart;                                 //缓存区的起始地址
    int8_t*     pEnd;                                   //缓存区的末尾地址
    uint16_t    Number;                                 //当前缓存区内存储的字符个数
};
    
      
    
    

//@brief GPS通信 初始化
/*--------------------------------------------------------------------------*/
//<*参数说明:
//<*
//<*返回值说明:
//<*    0:      初始化成功
//<*    其它:   失败
/*--------------------------------------------------------------------------*/
uint8_t Leo_nRF52_GPS_Initial(void);


//@brief 对存入G_GPS_Sentence内的完整GPS语句 进行解析
/*--------------------------------------------------------------------------*/
//<*参数说明:
//<*返回值说明:
//<*    0 解析成功，获取数据
//<*    1 数据解析错误，有可能奇偶检校错误 或 数据判断错误！
/*--------------------------------------------------------------------------*/
uint8_t Leo_GPS_Decode(void);
    
    	
	
		
#ifdef __cplusplus
}
#endif


#endif  /* Leo_nRF52_GPS.h */		
