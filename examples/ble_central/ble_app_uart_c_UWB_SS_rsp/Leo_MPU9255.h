/*
*********************************************************************************************************
*
*    ģ������ : �ⲿ������ MPU9255
*    �ļ����� : Leo_MPU9255
*    ��    �� : V1.0
*    ˵    �� : �ⲿ������ MPU9255
*
*    �޸ļ�¼ :
*        �汾��    ����          ����     
*        V1.0    2019-01-18     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/

#ifndef LEO_MPU9255_H
#define LEO_MPU9255_H

#include "Leo_Includes.h"



#ifdef __cplusplus
extern "C" {
#endif
    

//============================== ����MPU9255�ڲ���ַ==================================================
/***************************
* ϵͳ������ؼĴ���
***************************/
#define MPU9255_WHO_AM_I 								0x75		 	//[7:0] WHOAMI �Ĵ������û�ָʾ���ڷ����ĸ��豸 
#define	MPU9255_PWR_MGMT_1 								0x6b			//��Դ�����Ĵ��� ����ֵ 0x80 reset����
#define MPU9255_PWR_MGMT_2								0x6c			//���ƼӼƺ������Ƿ�����	
#define	MPU9255_USER_CTRL 								0x6a			//�û����ƼĴ��� ����ֵ 0x30 enable spi and iic����ģ�� 	
#define MPU9255_SIGNAL_PATH_RESET						0x68			//�����ź�·���Ĵ���    <*****����Ӧ�û�û���׸����ף�*********>
#define MPU9255_ACCEL_CONFIG1  							0x1c			//���ٶȼ����ò��� ����ѡȡ��ACCEL_FS_SEL[1:0]
#define MPU9255_ACCEL_CONFIG2  							0x1d			//���ٶȼ����ò��� ���ֵ�ͨ�˲�������
#define MPU9255_GYRO_CONFIG								0x1b			//���������ò�������������GYRO_FS_SEL[4:3] , Fchoice_b[1:0]���� 
#define MPU9255_CONFIG 									0x1a			//ϵͳ��������,����:FIFO_MODE(FIFO���,0��ʾ�������old)��EXT_SYNC_SET[2:0](�ⲿ���� ֡ͬ��ʱ������)��DLPF_CFG[2:0](���ֵ�ͨ�˲�)����Ҫ������DLPF_CFG
#define MPU9255_SMPLRT_DIV								0x19			//ϵͳ�������Ƶ�����ò��� SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV);
#define MPU9255_INT_PIN									0x37			//ϵͳ �жϹܽ�/Bypass Enable  ����
#define MPU9255_INT_ENABLE								0x38			//ϵͳ �ж�ʹ��  ����	
#define MPU9255_INT_STATUS								0x3A			//ϵͳ �ж� ״̬�Ĵ���	


#define MPU9255_I2C_MST_CTRL							0x24			//I2C ����ģʽ����
#define MPU9255_I2C_MST_DELAY_CTRL					    0x67			//I2C ����ͨ���ӳٿ��ƣ� 	
#define MPU9255_I2C_MST_STATUS							0x36			//ֻ��

#define MPU9255_I2C_SLV0_ADDR							0x25			//I2C_SLV)_RNW[7] ��д��־λ��1 ����I2C_ID_0[6:0] I2C�ӻ�0��������ַ
#define MPU9255_I2C_SLV0_REG							0x26			//I2C�ӻ�0 ��ʼ��д���ݵ���ʼ�Ĵ�����ַ
#define MPU9255_I2C_SLV0_CTRL							0x27			//I2C�ӻ�0 �Ŀ��Ʋ��������忴�������� 

#define MPU9255_I2C_SLV1_ADDR							0x28
#define MPU9255_I2C_SLV1_REG							0x29
#define MPU9255_I2C_SLV1_CTRL							0x2A

#define MPU9255_I2C_SLV2_ADDR							0x2B
#define MPU9255_I2C_SLV2_REG							0x2C
#define MPU9255_I2C_SLV2_CTRL							0x2D

#define MPU9255_I2C_SLV3_ADDR							0x2E
#define MPU9255_I2C_SLV3_REG							0x2F
#define MPU9255_I2C_SLV3_CTRL							0x30

#define MPU9255_I2C_SLV4_ADDR							0x31
#define MPU9255_I2C_SLV4_REG							0x32
#define MPU9255_I2C_SLV4_DO								0x33
#define MPU9255_I2C_SLV4_CTRL							0x34
#define MPU9255_I2C_SLV4_DI								0x35			//ֻ��

/**************************
* ������������ؼĴ���
**************************/
#define	MPU9255_ACCEL_XOUT_H	                        0x3B
#define	MPU9255_ACCEL_XOUT_L	                        0x3C
#define	MPU9255_ACCEL_YOUT_H	                        0x3D
#define	MPU9255_ACCEL_YOUT_L	                        0x3E
#define	MPU9255_ACCEL_ZOUT_H	                        0x3F
#define	MPU9255_ACCEL_ZOUT_L	                        0x40

#define	MPU9255_TEMP_OUT_H		                        0x41
#define	MPU9255_TEMP_OUT_L		                        0x42

#define	MPU9255_GYRO_XOUT_H		                        0x43
#define	MPU9255_GYRO_XOUT_L		                        0x44	
#define	MPU9255_GYRO_YOUT_H		                        0x45
#define	MPU9255_GYRO_YOUT_L		                        0x46
#define	MPU9255_GYRO_ZOUT_H		                        0x47
#define	MPU9255_GYRO_ZOUT_L		                        0x48

#define	MPU9255_EXT_SENS_DATA_00	0x49	

//============================== ����MPU9255���ò���==================================================
#define MPU9255_PWR_MGMT_1_RESET						(0x80)		//[7]: 	0x80 �C Reset the internal registers and restores the default settings. Write a 1 to set the reset, the bit will auto clear.
#define MPU9255_PWR_MGMT_1_CLOCK_MASK					(0xF8)		//			<***������***>
#define MPU9255_PWR_MGMT_1_CLOCK_INTERNAL			    (0x00)			//			<***������***>
#define MPU9255_PWR_MGMT_1_CLOCK_PLL					(0x01)		//			<***������***>
#define MPU9255_PWR_MGMT_1_CLOCK_CLKSEL				    (0x03)		//[2:0]:0x03 �C Auto selects the best available clock source �C PLL if ready, else use the Internal 20MHz oscillator

#define MPU9255_INT_PIN_LATCH_INT_EN					(0x00)		//[5]:	0x20 �C INT pin level held until interrupt status is cleared.
																												//			0x00 �C INT pin indicates interrupt pulse��s is width 50us.
#define MPU9255_INT_PIN_INT_ANYRD_2CLEAR			    (0x10)		//[4]:	0x10 �C Interrupt status is cleared if any read operation is performed.
																												//			0x00 �C Interrupt status is cleared only by reading INT_STATUS register

#define MPU9255_INT_ENABLE_RAW_RDY_EN					(0x01)		//[0]:	0x01 �C Enable Raw Sensor Data Ready interrupt to propagate to interrupt pin. The timing of the interrupt can vary 
																												//							depending on the setting in register 36 I2C_MST_CTRL, bit [6] WAIT_FOR_ES.

#define MPU9255_PWR_MGMT_2_ACCEL_XYZ					(0x00)		//[5:3]	0x38 �C �ر����м��ٶȼ�
																												//			0x00 �C �����м��ٶȼ�
#define MPU9255_PWR_MGMT_2_GYRO_XYZ						(0x00)		//[2:0]	0x07 �C �ر���������
																												//			0x00 �C ����������		
																												
#define MPU9255_GYRO_CONFIG_FS							(0x10)		//[4:3]	0x10 �C ���������̲���		0x00:+/-250; 0x08:+/-500; 0x10:+/-1000; 0x18:+/-2000
#define MPU9255_GYRO_CONFIG_Fb							(0x00)		//[1:0]	0x00 �C ������ Fchoice_b �Ĳ��� �� DLPF_CFG ������� ���ݺ��¶ȼƵ� ���ֵ�ͨ�˲�����

#define MPU9255_ACCEL_CONFIG1_FS						(0x10)		//[4:3]:0x10 �C �ӱ�����	0x00:+/-2G; 0x08:+/-4G; 0x10:+/-8G; 0x18:+/-16G	

#define MPU9255_ACCEL_CONFIG2_FCHOICE_B				    (0x00)		//[3]:	0x00 �C ʹ�� ACCEL_FCHOICE Ϊ 1 ����� DLPFCFG �趨���ٶȼƵ�DLPF
#define MPU9255_ACCEL_CONFIG2_DLPFCFG					(0x01)		//[2:0]:0xxx �C ���ٶȼ����ֵ�ͨ�˲�������0x00(460Hz Band,1.94ms Delay,1kHz Fs);0x01(184Hz,5.8ms,1kHz);0x02(92Hz,7.8ms,1kHz)
																												//							0x03(41Hz,11.8ms,1kHz);0x04(20Hz,19.8ms,1kHz);0x05(10Hz,35.7,1kHz)
																												//							0x06(5Hz,66.96ms,1kHz);0x07(460Hz,1.94ms,1kHz) ��0x07 �� 0x01 ��һ���ģ���
																												//							0x08(1.13kHz,0.75ms,4kHz)  ���Ƚ����⣬Ӧ������ԭʼ����ȫ������ĸо�����

#define MPU9255_CONFIG_FIFO_MODE						(0x00)		//[6]: 	0x40 �C When set to ��1��, when the fifo is full, additional writes will not be written to fifo.
																												//			0x00 �C When set to ��0��, when the fifo is full, additional writes will be written to the fifo, replacing the oldest data.
#define MPU9255_CONFIG_EXT_SYNC_SET						(0x00)		//[5:3]:0xxx �C ���� FSYNC ���� �������ݲ�����һ��������豸���и��ߵĲ���Ƶ��ʱ ����ʹ�ã�
																												//			0x00 �C CONFIG-- EXT_SYNC_SET 0 (���þ��������)
#define MPU9255_CONFIG_DLPF_CFG							(0x03)		//[2:0]:0xxx �C ��Fchoice_b=0x00������: 0x00(250Hz Band,0.97ms Delay,8kHz Fs) ���Ƶ�ʺͼӼƲ������ֿ�ʹ�ÿ�ѡ�����ǵ���ʱ�����ѡͬ�����Ƶ�ʵ�
																												//							0x01(184Hz,2.9ms,1kHz);0x02(92Hz,3.9ms,1kHz);0x03(41Hz,5.9ms,1kHz);
																												//							0x04(20Hz,9.9ms,1kHz);0x05(10Hz,17.85ms,1kHz);0x06(5Hz,33.48ms,1kHz); �����Ժ�������Ч������

#define MPU9255_SMPLRT_DIV_Rate							(0x03)		//[7:3]:0xxx �C ϵͳ�������Ƶ�����ò���,���㹫ʽ��SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV);
																												//							ȡ0x03������1kHz���������ֵ�ͨ�˲����������250Hz

#define	MPU9255_USER_CTRL_I2C_MST_EN					(0x20)		//[5]: 	0x20 �C Enable the I2C Master I/F module; pins ES_DA and ES_SCL are isolated from pins SDA/SDI and SCL/ SCLK.
																												//			0x00 �C Disable I2C Master I/F module; pins ES_DA and ES_SCL are logically driven by pins SDA/SDI and SCL/ SCLK.
#define	MPU9255_USER_CTRL_I2C_IF_DIS					(0x10)		//[4]: 	0x10 �C Reset I2C Slave module and put the serial interface in SPI mode only. This bit auto clears after one clock cycle.																						


#define MPU9255_SIGNAL_PATH_RESET_GYRO				    (0x04)		//GYRO_RST  	�������������ź�·����ע��:�������Ĵ���δ�������ʹ��SIG_COND_RST ������������Ĵ�����
#define MPU9255_SIGNAL_PATH_RESET_ACCEL				    (0x02)		//ACCEL_RST  	���ü��ٶȼ������ź�·����ע��:�������Ĵ���δ�������ʹ��SIG_COND_RST������������Ĵ�����
#define MPU9255_SIGNAL_PATH_RESET_TEMP				    (0x01)		//TEMP_RST	 	�����¶������ź�·����ע��:�������Ĵ���δ�������ʹ��SIG_COND_RST ������������Ĵ�����																							


#define MPU9255_I2C_MST_CTRL_MULT_MST_EN			    (0x00)		//[7]: 	0x80 �C ������ģʽ
																												//			0x00 �C ������ģʽ�����߱�����ͻ������When disabled, clocking to the I2C_MST_IF can be disabled when not in use and the logic to detect lost arbitration is disabled.
#define MPU9255_I2C_MST_CTRL_WAIT_FOR_ES			    (0x00)		//[6]:	0x40 �C Delays the data ready interrupt until external sensor data is loaded. If I2C_MST_IF is disabled, the interrupt will still occur.
#define MPU9255_I2C_MST_CTRL_I2C_MST_P_NSR		        (0x00)		//[4]:	0x10 �C This bit controls the I2C Master��s transition from one slave read to the next slave read. 
																												//							If 0, there is a restart between reads. If 1, there is a stop between reads.
#define MPU9255_I2C_MST_CTRL_I2C_MST_CLK			    (0x0D)		//[3:0]:0x0D �C I2C����ѡȡ400kHzʱ��Ƶ�ʣ�������Ҫ�� �ӻ� ʱ��Ƶ�ʱ���һ�£���


#define MPU9255_I2C_SLV0_CTRL_EN						(0x80)		//[7]:	0x80:	Enable reading data from this slave at the sample rate and storing data at the first available EXT_SENS_DATA register, 
																												//					which is always EXT_SENS_DATA_00 for I2C slave 0.
																												//			0x00:	function is disabled for this slave
#define MPU9255_I2C_SLV0_CTRL_BYTE_SW					(0x00)		//[6]:	
#define MPU9255_I2C_SLV0_CTRL_REG_DIS					(0x00)		//[5]:	When set, the transaction does not write a register value, it will only read data, or write data
#define MPU9255_I2C_SLV0_CTRL_GRP						(0x00)		//[4]:	0 indicates slave register addresses 0 and 1 are grouped together (odd numbered register ends the group). 
																												//			1 indicates slave register addresses 1 and 2 are grouped together (even numbered register ends the group). 
																												//This allows byte swapping of registers that are grouped starting at any address.
#define MPU9255_I2C_SLV0_CTRL_LENG						(0x08)		//[3:0]:Number of bytes to be read from I2C slave 0   [ʹ�ܴ����д����ֽ���Ϊ8] 

#define MPU9255_I2C_SLV4_CTRL_EN						(0x80)		//[7]:	0x80 �C Enable data transfer with this slave at the sample rate.
																												//							If read command, store data in I2C_SLV4_DI register,
																												//							If write command, write data stored in I2C_SLV4_DO register.
																												//							Bit is cleared when a single transfer is complete. Be sure to write I2C_SLV4_DO first
																												//			0x00 �C function is disabled for this slave
#define MPU9255_I2C_SLV4_CTRL_DONE_INT_EN			    (0x00)		//[6]:	0x40 �C Enables the completion of the I2C slave 4 data transfer to cause an interrupt.
																												//			0x00 �C Completion of the I2C slave 4 data transfer will not cause an interrupt.
#define MPU9255_I2C_SLV4_CTRL_REG_DIS					(0x00)		//[5]:	When set, the transaction does not write a register value, it will only read data, or write data
#define MPU9255_I2C_SLV4_CTRL_MST_DLY					(0x04)		//[4:0]	����I2C_MST_DELAY_CTRLʹ�ܣ���Щ�ӻ�������(1+I2C_MST_DLY)��ÿ��������ſɹ�����
																												//			����Ƶ����SMPLRT_DIV and DLPF_CFG registers �Ĵ����������൱�ڣ����������Ĳ���Ƶ�ʣ����ӻ��ٽ���һ�η�Ƶ,����Ƶ32
#define MPU9255_I2C_MST_STATUS_SLV4_DONE			    (0x40)		//��ʾ���INT_ENABLE�Ĵ�����I2C_MST_INT_EN λʹ�ܲ���I2C_SLV4_CTRL �Ĵ�����SLV4_DONE_INT_ENλʹ��ʱ��
																												//			��I2C�ӻ�4�Ĵ������ʱ����������ж�
#define MPU9255_I2C_MST_STATUS_SLV4_NACK			    (0x10)		//��ʾ���INT_ENABLE�Ĵ�����I2C_MST_INT_EN λʹ��ʱ�����ӻ�4 ���յ�һ��NACK �ź�ʱ�������ж�

#define MPU9255_I2C_MST_DELAY_CTRL_SHADOW			    (0x80)		//[7]:	0x80 �C Delays shadowing of external sensor data until all data is received
#define MPU9255_I2C_MST_DELAY_CTRL_SLV0_EN		        (0x01)		//[0]:	0x01 �C When enabled, slave 0 will only be accessed 1+I2C_MST_DLY) samples as determined by SMPLRT_DIV and DLPF_CFG


//============================== �����ڲ�AK8963��ַ==================================================
#define	MPU9255_AK8963_I2C_ADDR							0x0C		//AK8963�� MPU9255�е�I2C������ַ
#define MPU9255_AK8963_I2C_READ							0x80		//I2C��ȡ���������λ

#define MPU9255_AK8963_WIA								0x00		//Device ID
#define MPU9255_AK8963_INFO								0x01		//Information 
#define MPU9255_AK8963_ST1								0x02		//Status 1
#define MPU9255_AK8963_XOUT_L							0x03		//
#define MPU9255_AK8963_XOUT_H							0x04		//
#define MPU9255_AK8963_YOUT_L							0x05		//
#define MPU9255_AK8963_YOUT_H							0x06		//
#define MPU9255_AK8963_ZOUT_L							0x07		//
#define MPU9255_AK8963_ZOUT_H							0x08		//
#define MPU9255_AK8963_ST2								0x09		//Status 2
#define MPU9255_AK8963_CNTL1							0x0A		//Control 1
#define MPU9255_AK8963_CNTL2							0x0B		//Control 2   �� RSV һ����     ע�⣬�°汾���� û�� CNTL2 �ˣ�������
#define MPU9255_AK8963_RSV								0x0B		//Reserved   DO NOT ACCESS

#define MPU9255_AK8963_ASTC								0x0C		//Self test
#define MPU9255_AK8963_TS1								0x0D		//Test 1   DO NOT ACCESS
#define MPU9255_AK8963_TS2								0x0E		//Test 2   DO NOT ACCESS
#define MPU9255_AK8963_I2CDIS							0x0F		//I2C disable
#define MPU9255_AK8963_ASAX								0x10		//X-axis sensitivity adjustment value
#define MPU9255_AK8963_ASAY								0x11		//Y-axis sensitivity adjustment value
#define MPU9255_AK8963_ASAZ								0x12		//Z-axis sensitivity adjustment value

//============================== �����ڲ�AK8963���ò���==================================================
#define MPU9255_AK8963_Device_ID            	        (0x48)

#define MPU9255_AK8963_RSV_SRST							(0x01)		//[7]:	0x01:	������λ ������Ϊ��1��ʱ�����мĴ���������ʼ���� ��λ��SRST λ�Զ���Ϊ��0����

#define MPU9255_AK8963_CNTL1_BIT						(0x10)		//[4]:	0x10:	 0x00��14λ�����0x10��16λ���
#define MPU9255_AK8963_CNTL1_POWER_DOWN				    (0x00)		//����ģʽ
#define MPU9255_AK8963_CNTL1_SINGLE_MEASURE		        (0x01)		//���β��� ģʽ
#define MPU9255_AK8963_CNTL1_CONTINU_MEASURE1	        (0x02)		//�������� ģʽ1
#define MPU9255_AK8963_CNTL1_CONTINU_MEASURE2	        (0x06)		//�������� ģʽ2     <��������������ʽ�����𣿣���>  ������ѡȡ����ģʽ2��˵��ˢ������Ϊ100Hz
#define MPU9255_AK8963_CNTL1_EXTERNAL_TRIGGER	        (0x04)		//�ⲿ�������� ģʽ
#define MPU9255_AK8963_CNTL1_SELF_TEST				    (0x08)		//�Լ�� ģʽ
#define MPU9255_AK8963_CNTL1_FUSE_ROM					(0x0F)		//����˿ROM����ģʽ
#define MPU9255_AK8963_ASTC_SELF						(0x00)		//0x00�������Լ죻0x40�������ų��Լ죡 <ʲô�в����ų��Լ죿>	



//@brief nRF52<--SPI-->MPU9255  nRF52��MPU9255 ��ʼ��
/*--------------------------------------------------------------------------*/
//<* 	����˵����
//<*	����ֵ˵����
//<*		NRF_SUCCESS		��ʼ���ɹ� (0)
//<*		����					ʧ��
/*--------------------------------------------------------------------------*/
uint8_t ucMPU9255_INIT(void);


//@brief nRF52<--SPI-->MPU9255  nRF52��ȡMPU9255�� ���ٶȼ�����
/*--------------------------------------------------------------------------*/
//<*	����˵��:
//<*		��ȡ�����ݷ���ȫ�ֱ�����
//<*	����ֵ˵��:
//<*		NRF_SUCCESS		��ȡ�ɹ� (0)
//<*		����							��ȡʧ��
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_ACC(int16_t *pACC_X , int16_t *pACC_Y , int16_t *pACC_Z);
uint8_t Leo_MPU9255_Read_ACC(void);


//@brief nRF52<--SPI-->MPU9255  nRF52��ȡMPU9255�� ���ݼ����ݣ����жϷ���
/*--------------------------------------------------------------------------*/
//<*	����˵��:
//<*		��ȡ�����ݷ���ȫ�ֱ�����
//<*	����ֵ˵��:
//<*		NRF_SUCCESS		��ȡ�ɹ� (0)
//<*		����							��ȡʧ��
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_Gyro(int16_t *pGYRO_X , int16_t *pGYRO_Y , int16_t *pGYRO_Z )
uint8_t Leo_MPU9255_Read_Gyro(void);


//@brief nRF52<--SPI-->MPU9255  nRF52��ȡMPU9255�� ���ݼ����ݣ����жϷ���,û��������������
/*--------------------------------------------------------------------------*/
//<*	����˵��:
//<*		��ȡ�����ݷ���ȫ�ֱ�����
//<*	����ֵ˵��:
//<*		NRF_SUCCESS		��ȡ�ɹ� (0)
//<*		����					��ȡʧ��
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_Magnetic(int16_t *pMAG_X , int16_t *pMAG_Y , int16_t *pMAG_Z)
uint8_t Leo_MPU9255_Read_Magnetic(void);
    
 
    
	
#ifdef __cplusplus
}
#endif


#endif  /* LEO_MPU9255_H.h */    