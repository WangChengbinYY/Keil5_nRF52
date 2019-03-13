/*
*********************************************************************************************************
*
*    ģ������ : �ⲿӲ���ж�����
*    �ļ����� : Leo_INT
*    ��    �� : V1.0
*    ˵    �� : �ⲿӲ���ж��������
*
*    �޸ļ�¼ :
*        �汾��    ����          ����     
*        V1.0    2019-01-14     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/


#include "Leo_INT.h"
#include "Leo_SDCard.h"

/*
*********************************************************************************************************
*                                       �ж�ʹ�õ� ȫ�ֱ���
*********************************************************************************************************
*/

extern uint32_t	        G_MPU9255_Counter; 

extern uint32_t         G_GPSWeekSecond;
extern uint16_t         G_MicroSecond;

extern TaskHandle_t     xTaskHandle_MPU9255_RxData;
extern TaskHandle_t     xTaskHandle_DataSave_Start;         /*��ʼ�洢����           ��� */
extern TaskHandle_t     xTaskHandle_DataSave_End;


extern uint8_t          G_Ctrl_DataSave;


/*
*********************************************************************************************************
*                                       SDCard �洢��ͣ �ⲿ�ж�
*********************************************************************************************************
*/


/*-----------------------------------------------------------------------*/
/* SDCard �洢��ͣ�ж� ��Ӧ����                                           */
/*----------------------------------------------------------------------*/
static void vINTHandler_SDCard(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(G_Ctrl_DataSave == 0)
    {
        xTaskNotifyFromISR(xTaskHandle_DataSave_Start,      /* Ŀ������ */
                           0,                               /* �������� */
                           eSetValueWithOverwrite,          /* ���Ŀ�������ϴε����ݻ�û�д������ϴε����ݻᱻ���� */
                            &xHigherPriorityTaskWoken);
         /* ���xHigherPriorityTaskWoken = pdTRUE����ô�˳��жϺ��е���ǰ������ȼ�����ִ�� */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        nrfx_gpiote_out_clear(configGPIO_LED_G);
    }else
    {
        xTaskNotifyFromISR(xTaskHandle_DataSave_End,        /* Ŀ������ */
                           0,                               /* �������� */
                           eSetValueWithOverwrite,          /* ���Ŀ�������ϴε����ݻ�û�д������ϴε����ݻᱻ���� */
                            &xHigherPriorityTaskWoken);
         /* ���xHigherPriorityTaskWoken = pdTRUE����ô�˳��жϺ��е���ǰ������ȼ�����ִ�� */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        nrfx_gpiote_out_set(configGPIO_LED_G);
    }

    
#if Leo_Debug
    NRF_LOG_INFO("TEST:   SDCard INT is ok!");
    NRF_LOG_FLUSH();
    
    uint8_t pcWriteBuffer[300];
    NRF_LOG_INFO("=================================================");
    NRF_LOG_INFO("\nname      namestate  priority   rest   number");
    vTaskList((char *)&pcWriteBuffer);
    NRF_LOG_INFO("\n%s",pcWriteBuffer);
    NRF_LOG_FLUSH();
    
    NRF_LOG_INFO("=================================================");
    NRF_LOG_INFO("\nname       counter         reate");
    vTaskGetRunTimeStats((char *)&pcWriteBuffer);
    NRF_LOG_RAW_INFO("\n%s",pcWriteBuffer);
    NRF_LOG_FLUSH();    
#endif
    
}


/*-----------------------------------------------------------------------*/
/* SDCard �洢��ͣ�ж� ��ʼ��                                             */
/*----------------------------------------------------------------------*/
uint8_t ucINTInital_SDCard(void)
{	
	uint8_t err_code = 0;	
    nrfx_gpiote_in_config_t txINTConfig = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(false);            //�½�����Ч
    txINTConfig.pull = NRF_GPIO_PIN_PULLUP;		                                                // ����  ��̬�ߵ�ƽ
    err_code = nrfx_gpiote_in_init(configGPIO_INT_SDCard, &txINTConfig, vINTHandler_SDCard);
    return err_code;
}


/*-----------------------------------------------------------------------*/
/* SDCard �洢��ͣ�ж� ����                                               */
/*-----------------------------------------------------------------------*/
uint8_t ucINTStart_SDCard(void)
{	
    nrfx_gpiote_in_event_enable(configGPIO_INT_SDCard, true);
    return 0;
}



/*
*********************************************************************************************************
*                                       MPU9255 ���ݽ��� �ж�
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------*/
/* MPU9255 �ж� ��Ӧ�¼�                                                 */
/*----------------------------------------------------------------------*/
static void vINTHandler_MPU9255(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t actio)
{
    //������+1
    if( G_MPU9255_Counter == 255)
    {
        G_MPU9255_Counter = 0;
    }else
    {
        G_MPU9255_Counter++;
    }

    //֪ͨ����������ݲɼ�
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     xTaskNotifyFromISR(xTaskHandle_MPU9255_RxData,      /* Ŀ������ */
                       0,                               /* �������� */
                       eSetValueWithOverwrite,           /* ���Ŀ�������ϴε����ݻ�û�д������ϴε����ݻᱻ���� */
                        &xHigherPriorityTaskWoken);
     /* ���xHigherPriorityTaskWoken = pdTRUE����ô�˳��жϺ��е���ǰ������ȼ�����ִ�� */
     portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------------------*/
/* MPU9255 �жϳ�ʼ��                                                    */
/*----------------------------------------------------------------------*/ 
uint8_t ucINTInital_MPU9255(void)
{	
	uint8_t err_code;	
	nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);    	//��������Ч
	in_config.pull = NRF_GPIO_PIN_PULLDOWN;											    //���� ��̬�͵�ƽ
	
	err_code = nrfx_gpiote_in_init(configGPIO_INT_MPU9255, &in_config, vINTHandler_MPU9255);	
	return err_code;
}

/*-----------------------------------------------------------------------*/
/* MPU9255 �ж� ����                                                     */
/*----------------------------------------------------------------------*/
uint8_t ucINTStart_MPU9255(void)
{	
    nrfx_gpiote_in_event_enable(configGPIO_INT_MPU9255, true);
    return 0;
}



/*
*********************************************************************************************************
*                                       GPS 1pps �ж�
*********************************************************************************************************
*/

/*-----------------------------------------------------------------------*/
/* 1pps �ж� ��Ӧ�¼�                                                    */
/*----------------------------------------------------------------------*/

static void vINTHandler_PPS(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t actio)
{
    //�յ�GPS 1PPS������
    G_GPSWeekSecond++;
    G_MicroSecond = 0;
//		if(G_SDCard_IsSaved == 1)
//		{
//			nrf_gpio_pin_toggle(Leo_nRF52_LED_GREEN);
//		}
}


/*-----------------------------------------------------------------------*/
/* 1pps �жϳ�ʼ��                                                    */
/*----------------------------------------------------------------------*/ 
uint8_t ucINTInital_PPS(void)
{	
	uint8_t err_code;	
	nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);    	//��������Ч
	in_config.pull = NRF_GPIO_PIN_PULLDOWN;											    //���� ��̬�͵�ƽ
	
	err_code = nrfx_gpiote_in_init(configGPIO_INT_GPSPPS, &in_config, vINTHandler_PPS);	
	return err_code;
}

/*-----------------------------------------------------------------------*/
/* 1pps �ж� ����                                                     */
/*----------------------------------------------------------------------*/
uint8_t ucINTStart_PPS(void)
{	
    nrfx_gpiote_in_event_enable(configGPIO_INT_GPSPPS, true);
    return 0;
}


/*******************************************************************************************************/


















