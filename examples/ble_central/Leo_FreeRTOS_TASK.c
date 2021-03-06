/*
*********************************************************************************************************
*
*    模块名称 : FreeRTOS多任务实现
*    文件名称 : Leo_FreeRTOS_TASK
*    版    本 : V1.0
*    说    明 : 项目中所有任务的建立
*
*    修改记录 :
*        版本号    日期          作者     
*        V1.0    2019-01-19     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/

#include "Leo_FreeRTOS_TASK.h"
#include "Leo_INT.h"
#include "Leo_TIMER.h"
#include "Leo_SDCard.h"



/*========================== 全局变量定义！================================*/
//全局变量_IMU数据采集的 SPI2 实例（UWB 用SPI0；SDCard 用SPI1； IMU 和 压力传感器 用SPI2） 
nrf_drv_spi_t   SPI_CollectData = NRF_DRV_SPI_INSTANCE(configGPIO_SPI_CollectData_INSTANCE);	

//全局变量_时间参数 
uint32_t    G_GPSWeekSecond;                   //GPS周内秒数据
uint16_t    G_MicroSecond;                     //nRF52时间计数器控制的 1s的1000计数值，由外部GPS的1PPS校准 1PPS触发时 将其置0
uint8_t	    G_GPSWeekSecond_Data[7];          //用于数据采集时，记录的当时时刻的时间

//全局变量_IMU_A(U4)和IMU_B(U5)磁强计修正参数
uint8_t	    G_MAG_Coeffi[6]; 

//全局变量_IMU_A(U4)和IMU_B(U5)存放的缓存                
uint8_t	    G_IMU_Data_A[24];                   //第一组IMU_A(U4)存放的数据
uint8_t	    G_IMU_Data_B[24];                   //第二组IMU_A(U5)存放的数据
uint8_t	    G_IMUDataA_Counter;                  //MPU9255中断触发的计数器	    
uint8_t	    G_IMUDataB_Counter;






// 全局变量_SDCard存储缓存                                                         
uint8_t	    G_CollectData[512];                 //SDCard要储存数据的缓存
uint16_t    G_CollectData_Counter;  
// 全局变量_SDCard文件操作标识                                                         
uint8_t     G_SDCard_FileIsOpen;               //标记是否已经打开文件 没打开，默认为0


//uint8_t     G_GPS_Data[33];                     //GPS接收数据

//uint8_t     G_FOOTPressure_Data[25];            //足部压力传感器数据
//uint8_t	    G_FOOTPressure_Counter;             //足部压力传感器数据的计数器

//uint8_t     G_UWBDistance_Data[12];             //UWB测距传感器数据
//uint8_t	    G_UWBDistance_Counter;              //UWB测距传感器数据的计数器


//uint8_t	    G_SDCDBuffer1[1024];               //双缓存buffer
//uint16_t	G_SDCDBuffer1_NUM;	
//uint8_t	    G_SDCDBuffer2[1024];
//uint16_t	G_SDCDBuffer2_NUM;	

///**
// * 全局变量_系统控制   待完善
//*/
//uint8_t     G_Ctrl_DataSave;                    /* 数据存储控制标志位 1存储，0不存储 */










/*=========================================== 任务优先级设定 ============================================*/
/* 0级 */
#define taskPRIO_INIT                        0          /* 系统初始化：仅在开始执行一次，初始化失败则进入死循环LED不听闪烁*/
#define taskPRIO_SDCard_Close                0          //SDCard关闭文件成功  标志位置0  数据不会存储

/* 1级 */
#define taskPRIO_GPS_RxData                  1          //接收GPS数据并解析，解析成功，通知存储 

/* 2级 */
#define taskPRIO_SDCard_Save                 2          //SDCard存储数据

/* 3级 */
#define taskPRIO_CollectData                 3          //采集IMU数据、压力数据，成功则通知存储

/* 4级 */
#define taskPRIO_UWB_RxData                  4          //采集UWB测距数据，成功则通知存储


/*=========================================== 任务相关变量 ============================================*/
/**
 * 全局变量_任务函数句柄
*/
TaskHandle_t    xTaskHandle_TaskINIT            = NULL;         /*系统初始化任务       句柄 */
TaskHandle_t    xTaskHandle_SDCard_Save         = NULL;         /*SDCard存储任务       句柄 */
TaskHandle_t    xTaskHandle_SDCard_Close        = NULL;         /*SDCard 关闭文件任务  句柄 */
TaskHandle_t    xTaskHandle_CollectData         = NULL;         /*5ms触发的采集任务    句柄 */



/**
 * 全局变量_互斥量_SDCard缓存  
*/
SemaphoreHandle_t   xMutex_SDCDBuffer           = NULL;





/*======================================= 全局变量定义 待定！！！！！！==================================================*/
/* 全局变量_时间参数 */
//uint32_t    G_GPSWeekSecond;                   //GPS周内秒数据
//uint16_t    G_MicroSecond;                     //nRF52时间计数器控制的 1s的1000计数值，由外部GPS的1PPS校准 1PPS触发时 将其置0

///* 全局变量_SDCard文件操作标识 */                                                        
//uint8_t     G_SDCard_FileIsOpen;               //标记是否已经打开文件 没打开，默认为0

///* 全局变量_IMU数据采集的 SPI2 实例（UWB 用SPI0；SDCard 用SPI1； IMU 和 压力传感器 用SPI2） */
//nrf_drv_spi_t   SPI_CollectData = NRF_DRV_SPI_INSTANCE(configGPIO_SPI_CollectData_INSTANCE);  			//MPU9255使用的SPI实例		
//uint8_t         G_MPU9255_SPI_xfer_Done = 1;

//uint8_t     G_WRONG_Record[10];                 //采集过程数据出错的记录

//uint8_t	    G_IMU_Data[28];                 //MPU9255传感器存放的数据
//uint8_t	    G_IMUData_Counter;                  //MPU9255中断触发的计数器

//uint8_t     G_GPS_Data[33];                     //GPS接收数据

//uint8_t     G_FOOTPressure_Data[25];            //足部压力传感器数据
//uint8_t	    G_FOOTPressure_Counter;             //足部压力传感器数据的计数器

//uint8_t     G_UWBDistance_Data[12];             //UWB测距传感器数据
//uint8_t	    G_UWBDistance_Counter;              //UWB测距传感器数据的计数器


//uint8_t	    G_SDCDBuffer1[1024];               //双缓存buffer
//uint16_t	G_SDCDBuffer1_NUM;	
//uint8_t	    G_SDCDBuffer2[1024];
//uint16_t	G_SDCDBuffer2_NUM;	

///**
// * 全局变量_系统控制   待完善
//*/
//uint8_t     G_Ctrl_DataSave;                    /* 数据存储控制标志位 1存储，0不存储 */






/**
 * 全局变量初始化函数   待完善  
*/
static void vINIT_Variable(void)
{
    //全局变量_时间参数 
    G_GPSWeekSecond     = 0;                    //GPS周内秒数据
    G_MicroSecond       = 0;                    //nRF52时间计数器控制的 1s的1000计数值，
    //数据采集的整秒记录
    memset(G_GPSWeekSecond_Data,0,7);
    G_GPSWeekSecond_Data[0] = 0xA0;
	G_GPSWeekSecond_Data[1] = 0xA0;
    G_GPSWeekSecond_Data[6] = 0xFF;   
    
    //全局变量_IMU_A(U4)和IMU_B(U5)磁强计修正参数
    memset(G_MAG_Coeffi,0,6);
    G_MAG_Coeffi[5] = 0xFF;     
    
    //全局变量_IMU_A(U4)数据
    memset(G_IMU_Data_A,0,24);
    G_IMU_Data_A[0] = 0xB1;
	G_IMU_Data_A[1] = 0xB1;
    G_IMU_Data_A[23] = 0xFF;
    memset(G_IMU_Data_B,0,24);
    G_IMU_Data_B[0] = 0xB2;
	G_IMU_Data_B[1] = 0xB2;
    G_IMU_Data_B[23] = 0xFF;
    G_IMUDataA_Counter = 0;                  //IMU采集的次数计数值	    
    G_IMUDataB_Counter = 0;      
    
    // 全局变量_SDCard存储缓存        
    memset(G_CollectData,0,512);
    G_CollectData_Counter = 0;    
    //全局变量_SDCard文件操作标识 
    G_SDCard_FileIsOpen = 0;                    //标记是否已经打开文件 没打开，默认为0



//    
//    //GPS接收数据
//    memset(G_GPS_Data,0,33);
//    G_GPS_Data[0] = 0xA3;
//    G_GPS_Data[1] = 0xA4;
//    G_GPS_Data[32] = 0xFF;

//    //足部压力传感器数据
//    memset(G_FOOTPressure_Data,0,25);
//    G_FOOTPressure_Data[0] = 0xA5;
//	G_FOOTPressure_Data[1] = 0xA6;
//    G_FOOTPressure_Data[24] = 0xFF;
//    G_FOOTPressure_Counter = 0;
//    
//    //UWB测距传感器数据
//    memset(G_UWBDistance_Data,0,12);
//    G_UWBDistance_Data[0] = 0xA7;
//	G_UWBDistance_Data[1] = 0xA8;
//    G_UWBDistance_Data[11] = 0xFF;
//    G_UWBDistance_Counter = 0;
 
}


/*------------------------------------------------------------
 *外设相关的初始化，主要包括：
 *  1、全局变量初始化；
 *  2、管教配置：LED 中断；定时器等
 *  3、各类传感器(IMU GPS SDCard)等的初始化
 *------------------------------------------------------------*/
static uint8_t ucINIT_Peripheral()
{
    uint8_t err_code = 0;
/**
 * 0. 全局变量初始化    
 */    
    vINIT_Variable();    
    
/**
 * 1. GPIO管脚设定    
 */
    err_code |= nrfx_gpiote_init();
    
    /* (1) LED 管脚 */
    nrfx_gpiote_out_config_t tconfigGPIO_OUT =  NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
    err_code |= nrfx_gpiote_out_init(configGPIO_LED_R,&tconfigGPIO_OUT);
    nrfx_gpiote_out_set(configGPIO_LED_R);  //输出1，LED灯灭    
    NRF_LOG_INFO(("||Initialize||-->LED----------->error  0x%x"),err_code);
    NRF_LOG_FLUSH();
    
    /* (2) INT中断管脚初始化 */    
    err_code |= ucINTInital_SDCard();    /* SDCard中断管脚初始化 */    
    err_code |= ucINTInital_PPS();       /* 1PPS秒脉冲中断管脚初始化 */
    NRF_LOG_INFO(("||Initialize||-->INT----------->error  0x%x"),err_code);   
    NRF_LOG_FLUSH();    
  
    ucINTStart_SDCard();
/**
 * 2. 计时器初始化    
 */
    /* (1) 1ms 计时器 初始化 使用的TIMR3 */   
//    err_code |= ucTimerInitial_3();      /* TIMER3 计数器初始化*/ 
//    err_code |= ucTimerInitial_2();
//    err_code |= ucTimerInitial_4();
//    NRF_LOG_INFO(("||Initialize||-->TIMER---------->error  0x%x"),err_code);  
//    NRF_LOG_FLUSH();

///**
// * 3. 初始化SDCard 并建立记录文件    
// */

//    err_code |= ucSDCard_INIT();  
//    if(err_code == 0)
//    {
//        G_SDCard_FileIsOpen = 1;
//    }
//    NRF_LOG_INFO(("||Initialize||-->SDCard--------->error  0x%x"),err_code); 
//    NRF_LOG_FLUSH();   

///**
// * 4. 初始化 IMU 
//       MPU9250初始化，利用SPI对I2C进行控制时，利用片选管脚总是失败！！，只能重复 SPI init和uint了！！！
// */
//    
//    /* (1) 初始化IMU_A的SPI配置 并初始化IMU_B  */
//    //关闭 IMU_B 的 nCS管脚
//    err_code |= nrfx_gpiote_out_init(configGPIO_SPI_IMUB_nCS,&tconfigGPIO_OUT);
//    nrfx_gpiote_out_set(configGPIO_SPI_IMUB_nCS);  //输出1     
//    nrf_delay_us(10);
//    nrf_drv_spi_config_t SPI_config = NRF_DRV_SPI_DEFAULT_CONFIG;
//	SPI_config.sck_pin 			= configGPIO_SPI_CollectData_SCK;
//	SPI_config.mosi_pin 		= configGPIO_SPI_CollectData_MOSI;
//	SPI_config.miso_pin 		= configGPIO_SPI_CollectData_MISO;   
//    SPI_config.ss_pin			= configGPIO_SPI_IMUA_nCS;               //第一个IMU的nCS管脚
//	SPI_config.irq_priority	    = SPI_DEFAULT_CONFIG_IRQ_PRIORITY;		//系统SPI中断权限默认设定为 7 
//	SPI_config.orc				= 0xFF;
//	SPI_config.frequency		= NRF_DRV_SPI_FREQ_500K;				//MPU9255 SPI使用的范围为 100KHz~1MHz
//	SPI_config.mode             = NRF_DRV_SPI_MODE_0;                     
//    SPI_config.bit_order        = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;	
//	//依据配置参数 对 实例spi 进行初始化 
//	err_code |= nrf_drv_spi_init(&SPI_CollectData, &SPI_config, NULL,NULL);	
//    NRF_LOG_INFO(("||Initialize||-->IMU_A_SPI-------->error  0x%x"),err_code);
//    NRF_LOG_FLUSH();
//    //初始化 IMU_A
//    G_MAG_Coeffi[0] = 0xA1;
//    G_MAG_Coeffi[1] = 0xA1;
//    G_MAG_Coeffi[5] = 0xFF;
//    err_code |= ucMPU9255_INIT();    
//    NRF_LOG_INFO(("||Initialize||-->IMU_A----------->error  0x%x"),err_code);
//    NRF_LOG_FLUSH();
//    //卸载SPI
//    nrf_drv_spi_uninit(&SPI_CollectData);
//    nrf_delay_ms(3);
//    //关闭 IMU_A nCS 管脚
//    err_code |= nrfx_gpiote_out_init(configGPIO_SPI_IMUA_nCS,&tconfigGPIO_OUT);
//    nrfx_gpiote_out_set(configGPIO_SPI_IMUA_nCS);  //输出1       
        
    
    /* (2) 初始化第二个IMU_B  */  	
//    SPI_config.ss_pin			= configGPIO_SPI_IMUB_nCS;               //第二个IMU的nCS管脚
//    //IMU_B nCS 管脚恢复
//    nrfx_gpiote_out_uninit(configGPIO_SPI_IMUB_nCS);
//	//依据配置参数 对 实例spi 进行初始化 
//    err_code |= nrf_drv_spi_init(&SPI_CollectData, &SPI_config, NULL,NULL);	
//    nrf_delay_ms(3);    
//    //初始化 IMU_B
//    G_MAG_Coeffi[0] = 0xA2;
//    G_MAG_Coeffi[1] = 0xA2;
//    G_MAG_Coeffi[5] = 0xFF;
//    err_code |= ucMPU9255_INIT();    
//    NRF_LOG_INFO(("||Initialize||-->IMU_B----------->error  0x%x"),err_code);
//    NRF_LOG_FLUSH();
//    //卸载SPI
//    nrf_drv_spi_uninit(&SPI_CollectData);
//    nrf_delay_ms(3);
//    //关闭 IMU_B nCS 管脚    
//    err_code |= nrfx_gpiote_out_init(configGPIO_SPI_IMUB_nCS,&tconfigGPIO_OUT);
//    nrfx_gpiote_out_set(configGPIO_SPI_IMUB_nCS);  //输出1      
    
//    /* (3) 配置正确的IMU SPI  */  
//    //配置SPI 其中nCS不设定，初始化
//    SPI_config.ss_pin			= NRF_DRV_SPI_PIN_NOT_USED;         //不使用nCS管脚
//    SPI_config.frequency		= NRF_DRV_SPI_FREQ_1M;    
//    err_code |= nrf_drv_spi_init(&SPI_CollectData, &SPI_config, NULL,NULL);	   
//    NRF_LOG_INFO(("||Initialize||-->SPI_CollectData->error  0x%x"),err_code); 
//    NRF_LOG_FLUSH(); 
    
    
//    while(1)
//    {
//        nrf_delay_ms(500);
//        nrfx_gpiote_out_toggle(configGPIO_LED_R);
//        
//    }
  
    
    return err_code;
}

///*------------------------------------------------------------
// *SDCard新建文件任务 函数
// *------------------------------------------------------------*/
//static void vTask_LED_Quick(void *pvParameters)
//{
//    
//}


/*------------------------------------------------------------
 *SDCard新建文件任务 函数
 *------------------------------------------------------------*/
//static void vTask_SDCard_Open(void *pvParameters)
//{
//    uint8_t i = 0;
//    uint8_t erro_code = 0;
//    while(1)
//    {
//        /*(1) 等待任务通知     */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/
//        
//        erro_code = ucSDCard_OpenFile();
//        if(erro_code == 0)
//        {
//            for(i=0;i<15;i++)
//            {
//                nrfx_gpiote_out_toggle(configGPIO_LED_R);
//                nrf_delay_ms(200);
//            } 
//            G_SDCard_FileIsOpen = 1;
//            nrfx_gpiote_out_clear(configGPIO_LED_R);
//        }        
//    }
//}

/*------------------------------------------------------------
 *SDCard关闭文件任务 函数
 *------------------------------------------------------------*/
static void vTask_SDCard_Close(void *pvParameters)
{
    uint8_t i = 0;
    uint8_t erro_code = 0;
    while(1)
    {
        //(1) 等待任务通知     
        xTaskNotifyWait(0x00000000,     
                        0xFFFFFFFF,     
                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/     
        
        //(3)关闭文件存储
        erro_code = ucSDCard_CloseFile();
        
        //(4)LED灯快闪
        if(erro_code == 0)
        {
            for(i=0;i<15;i++)
            {
                nrfx_gpiote_out_toggle(configGPIO_LED_R);
                nrf_delay_ms(200);
            } 
            nrfx_gpiote_out_clear(configGPIO_LED_R);
            
            NRF_LOG_INFO("File is Closed!!!");
        }        
    }
}

/*------------------------------------------------------------
 *SDCard存储任务 函数
 *------------------------------------------------------------*/
static void vTask_SDCard_Save(void *pvParameters)
{
    uint8_t tData[512] = {0};
    uint16_t tData_Count = 0;
    uint8_t erro_code = 0;
    while(1)
    {
        /*(1) 等待任务通知     */
        xTaskNotifyWait(0x00000000,     
                        0xFFFFFFFF,     
                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/
        
        /*(2) 获取资源     */
        if(G_CollectData_Counter > 0)
        {
            if(xSemaphoreTake( xMutex_SDCDBuffer, ( TickType_t ) 5 ) == pdTRUE)
            {
                memcpy(tData,G_CollectData,G_CollectData_Counter);
                tData_Count = G_CollectData_Counter;
                G_CollectData_Counter = 0; 
                //释放资源
                xSemaphoreGive( xMutex_SDCDBuffer ); 
//                NRF_LOG_INFO("%d",tData_Count);
                //存储 文件如果打开才往里面存储
                if(G_SDCard_FileIsOpen == 1)
                {                    
                    erro_code = ucSDCard_SaveData(tData,tData_Count);                    
                }
                //LEODEBUG
                if(erro_code != 0)
                {
                    NRF_LOG_INFO("SDCard Save is Wrong!!!!!!!!!! %d",erro_code);
                    NRF_LOG_FLUSH();                     
                }                    
            }else
            {
                //LOEDEBUG
                NRF_LOG_INFO("SDCard_Buffer is Busy for SDCard!!!!!!!!!!");
                NRF_LOG_FLUSH(); 
            }
            
        }else{
            continue;
        }
    }        
}




/*------------------------------------------------------------
 *外部传感器数据采集，主要包括：
 *  MPU9255A、MPU9255B、压力传感器
 *------------------------------------------------------------*/
static void vTask_CollectData(void *pvParameters)
{
    while(1)
    {
        /*(1) 等待任务通知     */
        xTaskNotifyWait(0x00000000,     
                        0xFFFFFFFF,     
                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/      
        
        //(2)记录整秒数据
        memcpy(G_GPSWeekSecond_Data+2,&G_GPSWeekSecond,sizeof(G_GPSWeekSecond));
        memcpy(G_CollectData+G_CollectData_Counter,G_GPSWeekSecond_Data,sizeof(G_GPSWeekSecond_Data));
        G_CollectData_Counter = G_CollectData_Counter + sizeof(G_GPSWeekSecond_Data);        
        
        //(3)采集IMU_A 的数据  
        memcpy(G_IMU_Data_A+2,&G_MicroSecond,sizeof(G_MicroSecond));
        nrfx_gpiote_out_clear(configGPIO_SPI_IMUA_nCS);    
        Leo_MPU9255_Read_ACC(G_IMU_Data_A+5);
        Leo_MPU9255_Read_Gyro(G_IMU_Data_A+11);
        Leo_MPU9255_Read_Magnetic(G_IMU_Data_A+17);  
        nrfx_gpiote_out_set(configGPIO_SPI_IMUA_nCS);   
        
        //(4)采集IMU_B 的数据
//        memcpy(G_IMU_Data_B+2,&G_MicroSecond,sizeof(G_MicroSecond));        
//        nrfx_gpiote_out_clear(configGPIO_SPI_IMUB_nCS);    
//        Leo_MPU9255_Read_ACC(G_IMU_Data_B+5);
//        Leo_MPU9255_Read_Gyro(G_IMU_Data_B+11);
//        Leo_MPU9255_Read_Magnetic(G_IMU_Data_B+17);  
//        nrfx_gpiote_out_set(configGPIO_SPI_IMUB_nCS);    
        
        /*(4) 采集压力传感器 的数据  未完成*/      
        
        
        /*(5) 都采集完了,整体存储 
         *    等待4ms，如果还没有释放，则放弃此次存储*/
        if(xSemaphoreTake( xMutex_SDCDBuffer, ( TickType_t ) 4 ) == pdTRUE)
        {
            memcpy(G_CollectData+G_CollectData_Counter,G_IMU_Data_A,sizeof(G_IMU_Data_A));
            G_CollectData_Counter = G_CollectData_Counter + sizeof(G_IMU_Data_A);
//            memcpy(G_CollectData+G_CollectData_Counter,G_IMU_Data_B,sizeof(G_IMU_Data_B));
//            G_CollectData_Counter = G_CollectData_Counter + sizeof(G_IMU_Data_B);        
            //释放资源
            xSemaphoreGive( xMutex_SDCDBuffer ); 
            //通知 SDCard存储任务
            xTaskNotify(xTaskHandle_SDCard_Save,     
                        0,              
                        eNoAction);                
        }else
        {
            //LOEDEBUG
            NRF_LOG_INFO("SDCard_Buffer is Busy for IMU!!!!!!!!!!!!");
            NRF_LOG_FLUSH(); 
        }

    }
}





/*------------------------------------------------------------
 *任务相关的初始化，主要包括：
 *  1、互斥量的初始化
 *  2、各类任务的初始化及启动
 *------------------------------------------------------------*/
static uint8_t ucINIT_Task()
{
    uint8_t erro_code = 0;
    BaseType_t txResult = pdPASS;
    
    /*(1) 互斥量的建立_SDCard缓存 */
    xMutex_SDCDBuffer = xSemaphoreCreateMutex();
    if(xMutex_SDCDBuffer == NULL)
    {
        erro_code = 1;
    }
    
    /*(2) 建立SDCard存储任务 */    
    txResult = xTaskCreate(vTask_SDCard_Save,
                            "SDCardSave",
                            configMINIMAL_STACK_SIZE+400,
                            NULL,
                            taskPRIO_SDCard_Save,
                            &xTaskHandle_SDCard_Save);
    if(txResult != pdPASS)
    {
        erro_code = 1;
    }     
    
    /*(3) 建立SDCard 关闭文件任务 */      
    txResult = xTaskCreate(vTask_SDCard_Close,
                            "SDCardClose",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            taskPRIO_SDCard_Close,
                            &xTaskHandle_SDCard_Close);
    if(txResult != pdPASS)
    {
        erro_code = 1;
    }     
    
    /*(4) 建立采集任务 5ms */
    txResult = xTaskCreate(vTask_CollectData,
                            "CollectData",
                            configMINIMAL_STACK_SIZE+200,
                            NULL,
                            taskPRIO_CollectData,
                            &xTaskHandle_CollectData);
    if(txResult != pdPASS)
    {
        erro_code = 1;
    }
    
    return erro_code;  
}
    



/*------------------------------------------------------------
 *初始化任务  只执行一次                                                                              
 *------------------------------------------------------------*/
static void vTask_StartINIT(void *pvParameters)
{
    uint8_t err_code = 0;
    BaseType_t txResult = pdPASS;
    
/**
 * 0. 外设相关初始化
 *      初始化成功，红灯亮；
 *      初始化错误，红灯灭，删除初始化任务并退出    
 */     
    err_code |= ucINIT_Peripheral();
    if(err_code != 0)
    {
        vTaskDelete(xTaskHandle_TaskINIT);
        NRF_LOG_INFO("Peripheral Initialization is Wrong!!");
        NRF_LOG_FLUSH();
        return ;
    }else{
        nrfx_gpiote_out_clear(configGPIO_LED_R);
    }    

/**
 * 1. 任务相关初始化（建立好后，自动启动）
 *      任务初始化成功，红灯亮；
 *      任务初始化错误，红灯灭，删除初始化任务并退出       
 */
    err_code |= ucINIT_Task();
    if(err_code != 0)
    {
        vTaskDelete(xTaskHandle_TaskINIT);
        NRF_LOG_INFO("TASK Initialization is Wrong!!");
        NRF_LOG_FLUSH();
        return ;
    }else{
        nrfx_gpiote_out_clear(configGPIO_LED_R);
    }       
    
/**
 * 2. 中断及定时器 启动  未完成
 */
    //(1)计时器启动
    err_code |= ucTimerStart_3();      /* TIMER3 计数器初始化*/ 
    err_code |= ucTimerStart_4();
    err_code |= ucTimerStart_2();
    
    //(2)中断启动
    ucINTStart_SDCard();
    ucINTStart_PPS();
    
    
/**
 * 3. 删除启动任务    
 */
    NRF_LOG_INFO("||  Start  ||-->TaskINI is Finished!!");
    NRF_LOG_FLUSH();
    vTaskDelete(xTaskHandle_TaskINIT);
}

   




/*-----------------------------------------------------------------------*/
/* 创建任务                                                              */
/*-----------------------------------------------------------------------*/
uint8_t vTask_CreatTask(void)
{
    BaseType_t txResult = pdPASS;
    txResult = xTaskCreate(vTask_StartINIT,
                            "StartTask",
                            configMINIMAL_STACK_SIZE+200,
                            NULL,
                            taskPRIO_INIT,
                            &xTaskHandle_TaskINIT);
    if(txResult != pdPASS)
    {
        NRF_LOG_INFO("||Warning||-->TaskINI Create is failed!!");
        NRF_LOG_FLUSH();
        return 1;
    }else
    {
        return 0;
    }
}





















///**
// * 开始数据存储   函数
//*/
//static void vTask_DataSave_Start(void *pvParameters)
//{
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知     */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/        
//        G_Ctrl_DataSave = 1;       
//        
//    }
//}

///**
// * 结束数据存储   函数
//*/
//static void vTask_DataSave_End(void *pvParameters)
//{
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知     */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/        
//        G_Ctrl_DataSave = 0;       
//        vTaskDelay( (TickType_t)100 );
//        ucSDCard_CloseFile();        
//    }
//}


///**
// * MPU9255数据采集任务   函数
//*/
//static void vTask_MPU9255_RxData(void *pvParameters)
//{
//    
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知     */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/        
//         /**
//         *(2) 采集MPU9255数据 */
//        memcpy(G_IMU_Data+2,&G_GPSWeekSecond,4);
//        memcpy(G_IMU_Data+6,&G_MicroSecond,2);        
//        memcpy(G_IMU_Data+8,&G_IMUData_Counter,1); 
//        
//        /* 采集传感器数据 */    
//        Leo_MPU9255_Read_ACC();
//        Leo_MPU9255_Read_Gyro();
//        Leo_MPU9255_Read_Magnetic(); 
//        
//        /*若是可以存储 */
//        if(G_Ctrl_DataSave == 1)
//        {
//            /* 获取资源存放数据   等待2ms 一个周期5ms*/
//            if(xSemaphoreTake( xMutex_SDCDBuffer_1, ( TickType_t ) 2 ) == pdTRUE)
//            {   
//                 /*  有漏洞，有可能缓存溢出  如果 存储不及时的话！！ */
//                if(G_SDCDBuffer1_NUM + sizeof(G_IMU_Data) > 1024)
//                {
//                    NRF_LOG_INFO("MPU9255 Can't Save, It's Full !!!!!!!!!!!!!!!!!");
//                    NRF_LOG_FLUSH();  
//                }else
//                {
//                    memcpy(G_SDCDBuffer1+G_SDCDBuffer1_NUM,G_IMU_Data,sizeof(G_IMU_Data)); 
//                    G_SDCDBuffer1_NUM += sizeof(G_IMU_Data);
//                }               
//                 
//                /* 释放资源 */
//                xSemaphoreGive( xMutex_SDCDBuffer_1 );            
//            }else
//            {
//                /* 未获取到资源的使用权限 */
//                NRF_LOG_INFO("MPU9255 Use Buffer is Busy!!!!!!!!!!!!!!!!!");
//                NRF_LOG_FLUSH();            
//            }            
//        }
//        

//        //test        
////        if((G_IMUData_Counter % 255) == 0)
////        {
////            NRF_LOG_INFO("MPU9255 Data is OK!___5s");
////        }        
//    }
//}


///**
// * GPS数据采集任务   函数
//*/
//static void vTask_GPS_RxData(void *pvParameters)
//{
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知
//         */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/   
//    }    
//    
//}

///**
// * UWB数据采集任务   函数
//*/    
//static void vTask_UWB_RxData(void *pvParameters)
//{
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知
//         */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/   
//    } 
//}



///**
// * 压力传感器数据采集任务   函数
//*/
//static void vTask_FootPres_RxData(void *pvParameters)
//{
//    while(1)
//    {
//        /**
//         *(1) 等待任务通知     */
//        xTaskNotifyWait(0x00000000,     
//                        0xFFFFFFFF,     
//                        NULL,                 /* 保存ulNotifiedValue到变量ulValue中 如果不用可以设为NULL */
//                        portMAX_DELAY);       /* 最大允许延迟时间 portMAX_DELAY 表示永远等待*/        
//         /**
//         *(2) 采集MPU9255数据 */
//        
//        /*若是可以存储 */
//        if(G_Ctrl_DataSave == 1)
//        {
//            /* 获取资源存放数据   等待2ms 一个周期5ms*/
//            if(xSemaphoreTake( xMutex_SDCDBuffer_1, ( TickType_t ) 2 ) == pdTRUE)
//            {   
//                 /*  有漏洞，有可能缓存溢出  如果 存储不及时的话！！ */
//                if(G_SDCDBuffer1_NUM + sizeof(G_FOOTPressure_Data) > 1024)
//                {
//                    NRF_LOG_INFO("G_FOOTPressure_Data Can't Save, It's Full !!!!!!!!!!!!!!!!!");
//                    NRF_LOG_FLUSH();  
//                }else
//                {
//                    memcpy(G_SDCDBuffer1+G_SDCDBuffer1_NUM,G_FOOTPressure_Data,sizeof(G_FOOTPressure_Data)); 
//                    G_SDCDBuffer1_NUM += sizeof(G_FOOTPressure_Data);
//                }               
//                 
//                /* 释放资源 */
//                xSemaphoreGive( xMutex_SDCDBuffer_1 );            
//            }else
//            {
//                /* 未获取到资源的使用权限 */
//                NRF_LOG_INFO("G_FOOTPressure_Data Use Buffer is Busy!!!!!!!!!!!!!!!!!");
//                NRF_LOG_FLUSH();            
//            }            
//        }
//            
//    }
//}


///**
// * 中断启动时间任务   函数
//*/
//static void TimerFunction_StartINT( TimerHandle_t xTimer )
//{
//    uint8_t err_code = 0;
///*1. 启动计时器*/
//#if configTIMER3_ENABLE    
//    /* TIMER3 计数器启动*/ 
//    err_code |= ucTimerStart_3();
//#endif 
//    NRF_LOG_INFO(("||  Start  ||-->TIMER--------->error  0x%x"),err_code);
//    NRF_LOG_FLUSH();    


///*2. 启动外部中断*/
//    err_code |= ucINTStart_SDCard();    /* SDCard中断 启动 */    
//    err_code |= ucINTStart_MPU9255();   /* MPU9255采集中断 启动 */
//    err_code |= ucINTStart_PPS();       /* 1PPS秒脉冲中断 启动 */
//    NRF_LOG_INFO(("||  Start  ||-->GPS Uart------->error  0x%x"),err_code); 
//    NRF_LOG_FLUSH();
//    
///*3. 数据存储启动*/    
//    G_Ctrl_DataSave = 1;
//    nrfx_gpiote_out_clear(configGPIO_LED_G);
//}

///**
// * SDCard循环存储任务   函数
//*/
//static void TimerFunction_SDCardSave( TimerHandle_t xTimer )
//{
//    uint8_t     erro_code = 0;
//    if(xSemaphoreTake( xMutex_SDCDBuffer_1, ( TickType_t ) 2 ) == pdTRUE)
//    {
//        if(G_SDCDBuffer1_NUM > 0)
//        {
//            memcpy(G_SDCDBuffer2,G_SDCDBuffer1,G_SDCDBuffer1_NUM);
//            G_SDCDBuffer2_NUM = G_SDCDBuffer1_NUM;
//            G_SDCDBuffer1_NUM = 0;             
//        }
//        /* 释放资源 */
//        xSemaphoreGive( xMutex_SDCDBuffer_1 );          
//                    
//        if(G_Ctrl_DataSave == 1)
//        {
//            erro_code = ucSDCard_SaveData(G_SDCDBuffer2,G_SDCDBuffer2_NUM);  
//            if(erro_code != 0)
//            {
//                NRF_LOG_INFO("SDCard DataSave is Wrong Wrong !!!!!!!!!!!!!");
//                NRF_LOG_FLUSH(); 
//            }
//        }  
//    }else
//    {
//        /* 未获取到资源的使用权限 */
//        NRF_LOG_INFO("TimerFunction_FOOT Use Buffer1 is Busy!!!!!!!!!!!!!!!!!");
//        NRF_LOG_FLUSH();            
//    } 
//}














/*******************************************************************************************************/





/*
*********************************************************************************************************
*                                    自己任务的 相关设置
*********************************************************************************************************
*/



/* 多任务测试 */

//TaskHandle_t    xTaskHandle_Test;

//void vTaskFunction_TaskTest(void * pvParameter)
//{
//    UNUSED_PARAMETER(pvParameter);
//    while (true)
//    {   
//       
//        
//        NRF_LOG_INFO("=================================================");
//        NRF_LOG_FLUSH();
//        /* Delay a task for a given number of ticks */
//        vTaskDelay(1000);

//        /* Tasks must be implemented to never return... */
//    }
//}


//void vTask_Create(void)
//{


//    if (pdPASS != xTaskCreate(vTaskFunction_TaskTest, "TTest", 256, NULL, 2, &xTaskHandle_Test))
//    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
//    }  
//    NRF_LOG_INFO("||OS_Task   ||---->>xTaskCreate---->>completed!");

//}
















