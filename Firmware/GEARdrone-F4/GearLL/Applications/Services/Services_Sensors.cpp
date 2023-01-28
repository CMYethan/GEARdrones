/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    Services_Sensors.cpp
  * @author  EXcai
  * @brief   �����������Թ�������
  *
  ==============================================================================
													How to use this library 
  ==============================================================================
    @note
			- �����������Թ�������

  	@warning 
			- Standard C++11 required! 
  
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "System_Startup.h"
#include "Drone.h"
/* Private variables ---------------------------------------------------------*/
SPL06 Spl06;
HMC5883L Qmc5883l;
VL53L0X Tof;
OPTICFLOW Flow;
MPU6050 Mpu6050;
Voltage_ADC Battery_Voltage;
UWB Uwb;
ESP12F Esp;
extern GearDrone Drone;

struct pressure_data 			Spl_temp;
struct mag_data 					Mag_temp;
struct tof_data 					Tof_temp;
struct flow_data 					Flow_temp;
struct imu_data 					Imu_temp;
struct adc_data 					Adc_temp;
struct uwb_struct_data		Uwb_temp;
struct uart_data					Uart_temp;

/* Tasks Handle variables ----------------------------------------------------*/
TaskHandle_t Sensors_Task_200HZ_Handle;
TaskHandle_t Sensors_Task_50HZ_Handle;
TaskHandle_t Uwb_Task_Handle;
TaskHandle_t Flow_Task_Handle;

/**
 * @brief  	UWB��������
 
 * @param  	ͨ����ֵ�ź�������uwb���
 * @retval 	
 * @author EXcai(��������)
 */
void Uwb_Task_Function(void *arg){
	
	/* Sensors data structure ----------------------------------------------------*/

	for(;;){
		System_time_us = Get_SystemTimer();
		if(xSemaphoreTake(uwb_Semaphore, portMAX_DELAY) == pdTRUE){
			Uwb_temp = Uwb.Run();
		}
	}
}

/**
 * @brief  	������������
 
 * @param  	��������ٶ�
 * @retval 	
 * @author EXcai(��������)
 */
void Flow_Task_Function(void *arg){
	
	/* Sensors data structure ----------------------------------------------------*/

	for(;;){
		System_time_us = Get_SystemTimer();
		if(xSemaphoreTake(flow_Semaphore, portMAX_DELAY) == pdTRUE){
			Flow_temp = Flow.Run();
		}
	}
}

/**
 * @brief  	50HZ�����Թ�������
 
 * @param  	����Get_Value()�Ա�д�����������Թ�������
 * @retval 	
 * @author EXcai(��������)
 */

void Sensors_Task_50HZ_Function(void *arg){
	
	/* Sensors data structure ----------------------------------------------------*/
	
	TickType_t xLastWakeTime_t;
	xLastWakeTime_t = xTaskGetTickCount();
	
	for(;;){
		/* wait for next circle */
		vTaskDelayUntil(&xLastWakeTime_t,20);
		System_time_us = Get_SystemTimer();
		
#ifdef USE_UPPERMONITOR
		Sent_Contorl(&huart6);
#endif
		
		/* �����Ի�ȡ��������ֵ */
		Spl_temp = Spl06.Run();
		
		Tof_temp = Tof.Run();
		
		Adc_temp = Battery_Voltage.Run();
		
		/* ����������������Ĵ��������к�������Ϣ���� */
		//���磺Xxx.Run();xQueueSend(Xxx_Port,&Xxx_temp,0);
		
	}
}

/**
 * @brief  	200HZ�����Թ�������
 
 * @param  	����Get_Value()�Ա�д�����������Թ�������
 * @retval 	
 * @author EXcai(��������)
 */
void Sensors_Task_200HZ_Function(void *arg){
	
	/* Sensors data structure ----------------------------------------------------*/
	TickType_t xLastWakeTime_t;
	xLastWakeTime_t = xTaskGetTickCount();
	
	for(;;){
		/* wait for next circle */
		vTaskDelayUntil(&xLastWakeTime_t,5);
		System_time_us = Get_SystemTimer();
		/* �����Ի�ȡ��������ֵ */
		Mag_temp = Qmc5883l.Run();
		
		Imu_temp = Mpu6050.Run();
		
		/* ����������������Ĵ��������к�������Ϣ���� */
		//���磺Xxx.Run();xQueueSend(Xxx_Port,&Xxx_temp,0);
	}
}
