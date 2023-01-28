/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    Services_Sensors.cpp
  * @author  EXcai
  * @brief   ���˻���������
  *
  ==============================================================================
													How to use this library 
  ==============================================================================
    @note
			- ���˻���������

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
#include "NLR_localization.h"
#include "Drone.h"

/* Private variables ---------------------------------------------------------*/
GearDrone Drone;
Serial_Remote Remote;
Light LED;
NLR Nlr(0,0,2.4f,0,0,2.4f);//�Ե�0 ��1 ��2��Ϊê�㣬������Ϊ0�ķɻ�������ԭ�㴦�����Ϊ1�ķɻ���ȷ������(0,1m)�������Ϊ2�ķɻ���ȷ������(0,1m)��

extern pressure_data 	Spl_temp;
extern mag_data 			Mag_temp;
extern tof_data 			Tof_temp;
extern flow_data 			Flow_temp;
extern imu_data 			Imu_temp;
extern adc_data 			Adc_temp;
extern uwb_struct_data				Uwb_temp;

/* Tasks Handle variables ----------------------------------------------------*/
TaskHandle_t Process_Task_Handle;
TaskHandle_t Status_Task_Handle;
TaskHandle_t NLR_Task_Handle;

/********************************************************************************
 * @brief  	200HZ�����Թ�������
 
 * @param  	
 * @retval 	
 * @author EXcai(��������)
 *******************************************************************************/

void Process_Task_Function(void *arg){
	
	TickType_t xLastWakeTime_t;
	xLastWakeTime_t = xTaskGetTickCount();
	
	for(;;){
		/* wait for next circle */
		vTaskDelayUntil(&xLastWakeTime_t,5);
		System_time_us = Get_SystemTimer();
		LED.Update(System_time_us);
		
		Drone.Get_Sensors_Data(&Spl_temp,&Mag_temp,&Tof_temp,&Flow_temp,&Imu_temp,&Adc_temp,&Uwb_temp);
		Drone.Controller(Drone.Controller_State);
	}
}

/********************************************************************************
 * @brief  	����״̬��
 
 * @param  	
 * @retval 	
 * @author EXcai(��������)
 *******************************************************************************/
struct serial_data serial_remote;
void Status_Task_Function(void *arg){
	
	TickType_t xLastWakeTime_t;
	xLastWakeTime_t = xTaskGetTickCount();
	
	/* �����Ҫ���¸����˻���ţ����Ը���һ��Ĭ��Drone_ID��ȡ��ע������ */
//	uint32_t Drone_ID = 4;
//	Flash_erase(ADDR_FLASH_SECTOR_6, 6);
//	Flash_write_single(ADDR_FLASH_SECTOR_6, &Drone_ID, 1);
	
	Flash_read(ADDR_FLASH_SECTOR_6,(uint32_t *)&Drone.Flash_ID, 1);
	
	/* �쳣��λ��� */
	Drone.Reboot_Reason = RCC_ResetFlag_GetStatus();
	if(Drone.Reboot_Reason != 2 && Drone.Reboot_Reason != 0){
		uint8_t flash_store_temp[2];
		flash_store_temp[0] = Drone.Flash_ID;
		flash_store_temp[1] = Drone.Reboot_Reason;
		Flash_write_single(ADDR_FLASH_SECTOR_6, (uint32_t *)flash_store_temp, 2);
	}
	else{
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
	
	for(;;){
		/* wait for next circle */
		vTaskDelayUntil(&xLastWakeTime_t,25);
		System_time_us = Get_SystemTimer();
		Drone.Error_Monitor();
//		Drone.Remote_Mode = Remote.Get_Remote(&serial_remote);
		
		switch(Drone.Drone_Mode){
			/* ���²������ڼ���״̬�� */
			case 0:
				Drone.Drone_Mode = Drone.Startup(Drone.Drone_Mode);
				break;
			case 1:
				Drone.Drone_Mode = Drone.Taskoff_State(Drone.Drone_Mode);
				break;
			case 2:
				Drone.Drone_Mode = Drone.Remote_State(Drone.Drone_Mode);
				break;
			case 3:
				Drone.Drone_Mode = Drone.Formation_State(Drone.Drone_Mode);
				break;
			case 4:
				Drone.Drone_Mode = Drone.Landing_State(Drone.Drone_Mode);
				break;
			case 5:
				Drone.Drone_Mode = Drone.Crash_State(Drone.Drone_Mode);
				break;
			case 6:
				Drone.Drone_Mode = Drone.Debug_State(Drone.Drone_Mode);
				break;
			default:break;
		}
		
	}
}

/********************************************************************************
 * @brief  	NLR
 
 * @param  	
 * @retval 	
 * @author EXcai(��������)
 *******************************************************************************/
static int estimate_step = 0;
void NLR_Task_Function(void *arg){
	
	TickType_t xLastWakeTime_t;
	xLastWakeTime_t = xTaskGetTickCount();
	
	for(;;){
		/* wait for next circle */
		vTaskDelayUntil(&xLastWakeTime_t,200);
		System_time_us = Get_SystemTimer();
		Drone.NLR_Get_Data(Drone.uwb_d);
		if(estimate_step < 30 && Esp.Init_State == 2 && Drone.Flash_ID == 0){//ESP������ �� ֻ����30��
//			printf("AT+MQTTPUB=0,\"MAV%u_TX\",\"{\\\"type\\\":2\\,\\\"time\\\":%u\\,\\\"x1\\\":%f\\,\\\"y1\\\":%f\\,\\\"x2\\\":%f\\,\\\"y2\\\":%f}\",2,0\r\n",Drone.Flash_ID,Drone.System_time_ms,Nlr.unk[0],Nlr.unk[1],Nlr.unk[2],Nlr.unk[3]);
//			Nlr.Nlr_Estimate(Drone.uwb_d);
			estimate_step++;
		}
	}
}
