/*
*********************************************************************************************************
*
*    模块名称 : 外部传感器 IMU
*    文件名称 : Leo_IMU
*    版    本 : V1.0
*    说    明 : 外部传感器 IMU
*
*    修改记录 :
*        版本号    日期          作者     
*        V1.0    2019-03-10     Leo   
*
*    Copyright (C), 2018-2020, Department of Precision Instrument Engineering ,Tsinghua University  
*
*********************************************************************************************************
*/



#ifndef LEO_IMU_H
#define LEO_IMU_H

#include "Leo_Includes.h"



#ifdef __cplusplus
extern "C" {
#endif
    

//============================== 定义MPU9255内部地址==================================================
/***************************
* 系统配置相关寄存器
***************************/
#define MPU9255_WHO_AM_I 								0x75		 	//[7:0] WHOAMI 寄存器向用户指示正在访问哪个设备 
#define	MPU9255_PWR_MGMT_1 								0x6b			//电源管理寄存器 典型值 0x80 reset器件
#define MPU9255_PWR_MGMT_2								0x6c			//控制加计和陀螺是否启动	
#define	MPU9255_USER_CTRL 								0x6a			//用户控制寄存器 典型值 0x30 enable spi and iic主机模块 	
#define MPU9255_SIGNAL_PATH_RESET						0x68			//重置信号路径寄存器    <*****具体应用还没彻底搞明白！*********>
#define MPU9255_ACCEL_CONFIG1  							0x1c			//加速度计配置参数 量程选取：ACCEL_FS_SEL[1:0]
#define MPU9255_ACCEL_CONFIG2  							0x1d			//加速度计配置参数 数字低通滤波器设置
#define MPU9255_GYRO_CONFIG								0x1b			//陀螺仪配置参数：量程配置GYRO_FS_SEL[4:3] , Fchoice_b[1:0]配置 
#define MPU9255_CONFIG 									0x1a			//系统参数配置,包括:FIFO_MODE(FIFO溢出,0表示溢出覆盖old)、EXT_SYNC_SET[2:0](外部输入 帧同步时钟脉冲)、DLPF_CFG[2:0](数字低通滤波)，主要是设置DLPF_CFG
#define MPU9255_SMPLRT_DIV								0x19			//系统数据输出频率设置参数 SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV);
#define MPU9255_INT_PIN									0x37			//系统 中断管脚/Bypass Enable  配置
#define MPU9255_INT_ENABLE								0x38			//系统 中断使能  配置	
#define MPU9255_INT_STATUS								0x3A			//系统 中断 状态寄存器	


#define MPU9255_I2C_MST_CTRL							0x24			//I2C 主机模式设置
#define MPU9255_I2C_MST_DELAY_CTRL					    0x67			//I2C 主机通信延迟控制， 	
#define MPU9255_I2C_MST_STATUS							0x36			//只读

#define MPU9255_I2C_SLV0_ADDR							0x25			//I2C_SLV)_RNW[7] 读写标志位，1 读；I2C_ID_0[6:0] I2C从机0的物理地址
#define MPU9255_I2C_SLV0_REG							0x26			//I2C从机0 开始读写数据的起始寄存器地址
#define MPU9255_I2C_SLV0_CTRL							0x27			//I2C从机0 的控制参数，具体看参数定义 

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
#define MPU9255_I2C_SLV4_DI								0x35			//只读

/**************************
* 传感器数据相关寄存器
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

//============================== 定义MPU9255配置参数==================================================
#define MPU9255_PWR_MGMT_1_RESET						(0x80)		//[7]: 	0x80 – Reset the internal registers and restores the default settings. Write a 1 to set the reset, the bit will auto clear.
#define MPU9255_PWR_MGMT_1_CLOCK_MASK					(0xF8)		//			<***不懂！***>
#define MPU9255_PWR_MGMT_1_CLOCK_INTERNAL			    (0x00)			//			<***不懂！***>
#define MPU9255_PWR_MGMT_1_CLOCK_PLL					(0x01)		//			<***不懂！***>
#define MPU9255_PWR_MGMT_1_CLOCK_CLKSEL				    (0x03)		//[2:0]:0x03 – Auto selects the best available clock source – PLL if ready, else use the Internal 20MHz oscillator

#define MPU9255_INT_PIN_LATCH_INT_EN					(0x00)		//[5]:	0x20 – INT pin level held until interrupt status is cleared.
																												//			0x00 – INT pin indicates interrupt pulse’s is width 50us.
#define MPU9255_INT_PIN_INT_ANYRD_2CLEAR			    (0x10)		//[4]:	0x10 – Interrupt status is cleared if any read operation is performed.
																												//			0x00 – Interrupt status is cleared only by reading INT_STATUS register

#define MPU9255_INT_ENABLE_RAW_RDY_EN					(0x01)		//[0]:	0x01 – Enable Raw Sensor Data Ready interrupt to propagate to interrupt pin. The timing of the interrupt can vary 
																												//							depending on the setting in register 36 I2C_MST_CTRL, bit [6] WAIT_FOR_ES.

#define MPU9255_PWR_MGMT_2_ACCEL_XYZ					(0x00)		//[5:3]	0x38 – 关闭所有加速度计
																												//			0x00 – 打开所有加速度计
#define MPU9255_PWR_MGMT_2_GYRO_XYZ						(0x00)		//[2:0]	0x07 – 关闭所有陀螺
																												//			0x00 – 打开所有陀螺		
																												
#define MPU9255_GYRO_CONFIG_FS							(0x10)		//[4:3]	0x10 – 陀螺仪量程参数		0x00:+/-250; 0x08:+/-500; 0x10:+/-1000; 0x18:+/-2000
#define MPU9255_GYRO_CONFIG_Fb							(0x00)		//[1:0]	0x00 – 陀螺仪 Fchoice_b 的参数 和 DLPF_CFG 配合设置 陀螺和温度计的 数字低通滤波参数

#define MPU9255_ACCEL_CONFIG1_FS						(0x10)		//[4:3]:0x10 – 加表量程	0x00:+/-2G; 0x08:+/-4G; 0x10:+/-8G; 0x18:+/-16G	

#define MPU9255_ACCEL_CONFIG2_FCHOICE_B				    (0x00)		//[3]:	0x00 – 使得 ACCEL_FCHOICE 为 1 ，结合 DLPFCFG 设定加速度计的DLPF
#define MPU9255_ACCEL_CONFIG2_DLPFCFG					(0x01)		//[2:0]:0xxx – 加速度计数字低通滤波参数：0x00(460Hz Band,1.94ms Delay,1kHz Fs);0x01(184Hz,5.8ms,1kHz);0x02(92Hz,7.8ms,1kHz)
																												//							0x03(41Hz,11.8ms,1kHz);0x04(20Hz,19.8ms,1kHz);0x05(10Hz,35.7,1kHz)
																												//							0x06(5Hz,66.96ms,1kHz);0x07(460Hz,1.94ms,1kHz) 【0x07 和 0x01 是一样的！】
																												//							0x08(1.13kHz,0.75ms,4kHz)  【比较特殊，应该数据原始数据全部输出的感觉！】

#define MPU9255_CONFIG_FIFO_MODE						(0x00)		//[6]: 	0x40 – When set to ‘1’, when the fifo is full, additional writes will not be written to fifo.
																												//			0x00 – When set to ‘0’, when the fifo is full, additional writes will be written to the fifo, replacing the oldest data.
#define MPU9255_CONFIG_EXT_SYNC_SET						(0x00)		//[5:3]:0xxx – 利用 FSYNC 引脚 进行数据采样，一般第三方设备具有更高的采样频率时 可以使用！
																												//			0x00 – CONFIG-- EXT_SYNC_SET 0 (禁用晶振输入脚)
#define MPU9255_CONFIG_DLPF_CFG							(0x03)		//[2:0]:0xxx – 当Fchoice_b=0x00起作用: 0x00(250Hz Band,0.97ms Delay,8kHz Fs) 输出频率和加计不符，分开使用可选，但是导航时，最好选同样输出频率的
																												//							0x01(184Hz,2.9ms,1kHz);0x02(92Hz,3.9ms,1kHz);0x03(41Hz,5.9ms,1kHz);
																												//							0x04(20Hz,9.9ms,1kHz);0x05(10Hz,17.85ms,1kHz);0x06(5Hz,33.48ms,1kHz); 【可以后面试验效果！】

#define MPU9255_SMPLRT_DIV_Rate							(0x03)		//[7:3]:0xxx – 系统数据输出频率设置参数,计算公式：SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV);
																												//							取0x03，采样1kHz，经过数字低通滤波，输出数据250Hz

#define	MPU9255_USER_CTRL_I2C_MST_EN					(0x20)		//[5]: 	0x20 – Enable the I2C Master I/F module; pins ES_DA and ES_SCL are isolated from pins SDA/SDI and SCL/ SCLK.
																												//			0x00 – Disable I2C Master I/F module; pins ES_DA and ES_SCL are logically driven by pins SDA/SDI and SCL/ SCLK.
#define	MPU9255_USER_CTRL_I2C_IF_DIS					(0x10)		//[4]: 	0x10 – Reset I2C Slave module and put the serial interface in SPI mode only. This bit auto clears after one clock cycle.																						


#define MPU9255_SIGNAL_PATH_RESET_GYRO				    (0x04)		//GYRO_RST  	重置陀螺数字信号路径。注意:传感器寄存器未被清除。使用SIG_COND_RST 来清除传感器寄存器。
#define MPU9255_SIGNAL_PATH_RESET_ACCEL				    (0x02)		//ACCEL_RST  	重置加速度计数字信号路径。注意:传感器寄存器未被清除。使用SIG_COND_RST来清除传感器寄存器。
#define MPU9255_SIGNAL_PATH_RESET_TEMP				    (0x01)		//TEMP_RST	 	重置温度数字信号路径。注意:传感器寄存器未被清除。使用SIG_COND_RST 来清除传感器寄存器。																							


#define MPU9255_I2C_MST_CTRL_MULT_MST_EN			    (0x00)		//[7]: 	0x80 – 多主机模式
																												//			0x00 – 单主机模式，不具备检测冲突能力。When disabled, clocking to the I2C_MST_IF can be disabled when not in use and the logic to detect lost arbitration is disabled.
#define MPU9255_I2C_MST_CTRL_WAIT_FOR_ES			    (0x00)		//[6]:	0x40 – Delays the data ready interrupt until external sensor data is loaded. If I2C_MST_IF is disabled, the interrupt will still occur.
#define MPU9255_I2C_MST_CTRL_I2C_MST_P_NSR		        (0x00)		//[4]:	0x10 – This bit controls the I2C Master’s transition from one slave read to the next slave read. 
																												//							If 0, there is a restart between reads. If 1, there is a stop between reads.
#define MPU9255_I2C_MST_CTRL_I2C_MST_CLK			    (0x0D)		//[3:0]:0x0D – I2C主机选取400kHz时钟频率，这里需要和 从机 时钟频率保持一致！！


#define MPU9255_I2C_SLV0_CTRL_EN						(0x80)		//[7]:	0x80:	Enable reading data from this slave at the sample rate and storing data at the first available EXT_SENS_DATA register, 
																												//					which is always EXT_SENS_DATA_00 for I2C slave 0.
																												//			0x00:	function is disabled for this slave
#define MPU9255_I2C_SLV0_CTRL_BYTE_SW					(0x00)		//[6]:	
#define MPU9255_I2C_SLV0_CTRL_REG_DIS					(0x00)		//[5]:	When set, the transaction does not write a register value, it will only read data, or write data
#define MPU9255_I2C_SLV0_CTRL_GRP						(0x00)		//[4]:	0 indicates slave register addresses 0 and 1 are grouped together (odd numbered register ends the group). 
																												//			1 indicates slave register addresses 1 and 2 are grouped together (even numbered register ends the group). 
																												//This allows byte swapping of registers that are grouped starting at any address.
#define MPU9255_I2C_SLV0_CTRL_LENG						(0x08)		//[3:0]:Number of bytes to be read from I2C slave 0   [使能传输和写入的字节数为8] 

#define MPU9255_I2C_SLV4_CTRL_EN						(0x80)		//[7]:	0x80 – Enable data transfer with this slave at the sample rate.
																												//							If read command, store data in I2C_SLV4_DI register,
																												//							If write command, write data stored in I2C_SLV4_DO register.
																												//							Bit is cleared when a single transfer is complete. Be sure to write I2C_SLV4_DO first
																												//			0x00 – function is disabled for this slave
#define MPU9255_I2C_SLV4_CTRL_DONE_INT_EN			    (0x00)		//[6]:	0x40 – Enables the completion of the I2C slave 4 data transfer to cause an interrupt.
																												//			0x00 – Completion of the I2C slave 4 data transfer will not cause an interrupt.
#define MPU9255_I2C_SLV4_CTRL_REG_DIS					(0x00)		//[5]:	When set, the transaction does not write a register value, it will only read data, or write data
#define MPU9255_I2C_SLV4_CTRL_MST_DLY					(0x04)		//[4:0]	首先I2C_MST_DELAY_CTRL使能，这些从机仅仅在(1+I2C_MST_DLY)的每个采样点才可工作，
																												//			采样频率由SMPLRT_DIV and DLPF_CFG registers 寄存器决定。相当于，按照主机的采样频率，将从机再进行一次分频,最大分频32
#define MPU9255_I2C_MST_STATUS_SLV4_DONE			    (0x40)		//表示如果INT_ENABLE寄存器的I2C_MST_INT_EN 位使能并且I2C_SLV4_CTRL 寄存器的SLV4_DONE_INT_EN位使能时，
																												//			当I2C从机4的传输完成时，则会引发中断
#define MPU9255_I2C_MST_STATUS_SLV4_NACK			    (0x10)		//表示如果INT_ENABLE寄存器的I2C_MST_INT_EN 位使能时，当从机4 接收到一个NACK 信号时将引发中断

#define MPU9255_I2C_MST_DELAY_CTRL_SHADOW			    (0x80)		//[7]:	0x80 – Delays shadowing of external sensor data until all data is received
#define MPU9255_I2C_MST_DELAY_CTRL_SLV0_EN		        (0x01)		//[0]:	0x01 – When enabled, slave 0 will only be accessed 1+I2C_MST_DLY) samples as determined by SMPLRT_DIV and DLPF_CFG


//============================== 定义内部AK8963地址==================================================
#define	MPU9255_AK8963_I2C_ADDR							0x0C		//AK8963在 MPU9255中的I2C物理地址
#define MPU9255_AK8963_I2C_READ							0x80		//I2C读取操作的最高位

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
#define MPU9255_AK8963_CNTL2							0x0B		//Control 2   和 RSV 一样？     注意，新版本里面 没有 CNTL2 了！！！！
#define MPU9255_AK8963_RSV								0x0B		//Reserved   DO NOT ACCESS

#define MPU9255_AK8963_ASTC								0x0C		//Self test
#define MPU9255_AK8963_TS1								0x0D		//Test 1   DO NOT ACCESS
#define MPU9255_AK8963_TS2								0x0E		//Test 2   DO NOT ACCESS
#define MPU9255_AK8963_I2CDIS							0x0F		//I2C disable
#define MPU9255_AK8963_ASAX								0x10		//X-axis sensitivity adjustment value
#define MPU9255_AK8963_ASAY								0x11		//Y-axis sensitivity adjustment value
#define MPU9255_AK8963_ASAZ								0x12		//Z-axis sensitivity adjustment value

//============================== 定义内部AK8963配置参数==================================================
#define MPU9255_AK8963_Device_ID            	        (0x48)

#define MPU9255_AK8963_RSV_SRST							(0x01)		//[7]:	0x01:	软件复位 当设置为“1”时，所有寄存器都被初始化。 复位后，SRST 位自动变为“0”。

#define MPU9255_AK8963_CNTL1_BIT						(0x10)		//[4]:	0x10:	 0x00：14位输出；0x10：16位输出
#define MPU9255_AK8963_CNTL1_POWER_DOWN				    (0x00)		//掉电模式
#define MPU9255_AK8963_CNTL1_SINGLE_MEASURE		        (0x01)		//单次测量 模式
#define MPU9255_AK8963_CNTL1_CONTINU_MEASURE1	        (0x02)		//连续测量 模式1
#define MPU9255_AK8963_CNTL1_CONTINU_MEASURE2	        (0x06)		//连续测量 模式2     <两种连续测量方式的区别？？？>  样例中选取的是模式2，说是刷新速率为100Hz
#define MPU9255_AK8963_CNTL1_EXTERNAL_TRIGGER	        (0x04)		//外部触发测量 模式
#define MPU9255_AK8963_CNTL1_SELF_TEST				    (0x08)		//自检测 模式
#define MPU9255_AK8963_CNTL1_FUSE_ROM					(0x0F)		//保险丝ROM访问模式
#define MPU9255_AK8963_ASTC_SELF						(0x00)		//0x00：正常自检；0x40：产生磁场自检！ <什么叫产生磁场自检？>	




/*=========================================== MPU9255相关 ============================================*/


//@brief nRF52<--SPI-->MPU9255  nRF52对MPU9255 初始化
/*--------------------------------------------------------------------------*/
//<* 	参数说明：
//<*	返回值说明：
//<*		NRF_SUCCESS		初始化成功 (0)
//<*		其它					失败
/*--------------------------------------------------------------------------*/
uint8_t ucMPU9255_INIT(void);


//@brief nRF52<--SPI-->MPU9255  nRF52读取MPU9255内 加速度计数据
/*--------------------------------------------------------------------------*/
//<*	参数说明:
//<*		读取的数据放入全局变量中
//<*	返回值说明:
//<*		NRF_SUCCESS		读取成功 (0)
//<*		其它							读取失败
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_ACC(int16_t *pACC_X , int16_t *pACC_Y , int16_t *pACC_Z);
uint8_t Leo_MPU9255_Read_ACC(uint8_t * Dat);


//@brief nRF52<--SPI-->MPU9255  nRF52读取MPU9255内 陀螺计数据，并判断符号
/*--------------------------------------------------------------------------*/
//<*	参数说明:
//<*		读取的数据放入全局变量中
//<*	返回值说明:
//<*		NRF_SUCCESS		读取成功 (0)
//<*		其它							读取失败
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_Gyro(int16_t *pGYRO_X , int16_t *pGYRO_Y , int16_t *pGYRO_Z )
uint8_t Leo_MPU9255_Read_Gyro(uint8_t * Dat);


//@brief nRF52<--SPI-->MPU9255  nRF52读取MPU9255内 陀螺计数据，并判断符号,没有做修正！！！
/*--------------------------------------------------------------------------*/
//<*	参数说明:
//<*		读取的数据放入全局变量中
//<*	返回值说明:
//<*		NRF_SUCCESS		读取成功 (0)
//<*		其它					读取失败
/*--------------------------------------------------------------------------*/
//uint8_t Leo_MPU9255_Read_Magnetic(int16_t *pMAG_X , int16_t *pMAG_Y , int16_t *pMAG_Z)
uint8_t Leo_MPU9255_Read_Magnetic(uint8_t * Dat);
    
 
 



/*=========================================== IMU初始化 ============================================*/

/**
 * IMU 初始化
 *   采用 管脚共享方式，初始化 IMU_A 和 IMU_B
*/
uint8_t ucIMUInitial(void);













 
 
 
    
	
#ifdef __cplusplus
}
#endif


#endif  /* LEO_IMU_H.h */    