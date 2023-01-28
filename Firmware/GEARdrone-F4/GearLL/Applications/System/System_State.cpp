/**
  ******************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    System_State.cpp
  * @author  EXcai
  * @brief   ���˻�����״̬
  *
  ==============================================================================
													How to use this library ��ôʹ������⣿
  ==============================================================================
    @note
		- �ο�
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
#include "Drone.h"
#include "Serial_Remote.h"
/* Private variables --------------------------------------------------------*/
MeanFilter<25> F_takeoff;
extern Light LED;
/* function prototypes -------------------------------------------------------*/

/********************************************************************************
 * @brief  	���˻�����ģʽ 6
 
 * @param  	����ģʽ
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Debug_State(uint8_t mode){
	
	if(System_time_ms > _CONTROL_START_TIME && System_time_ms < _CONTROL_LIMIT_TIME){
//		target.pitch_target = 0;
//		target.roll_target = 0;
//		target.yaw_target = 0;
		
		Controller_State = 2;//0:����ʧЧ;1:���п�������;2:ֻ������̬����;3:ֻ���и߶�+��̬����
		mode = 6;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	else if(System_time_ms > _CONTROL_LIMIT_TIME){
		Init_PWM = 0;
		Controller_State = 0;
		mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻��쳣 5
 
 * @param  	�쳣����
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Crash_State(uint8_t mode){
	
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);//LED
	Controller_State = 0;//0:����ʧЧ;1:���п�������;2:ֻ������̬����;3:ֻ���и߶�+��̬����
	Init_PWM = 0;
	
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻��������� 0
 
 * @param  	Startup()�Գ�ʼ�����˻���
 
 * @retval 	0: mode
						1: mode
						5: mode
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Startup(uint8_t mode){
	
	/*��������ʼ��ʧ��*/
	if(Sensors_State != 0){
		mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	
	/*��ǰ1000ms������(���define��NO_MOTOR����������Ч)*/
	if(System_time_ms > _CONTROL_START_TIME - 1000){
		Init_PWM = 2000;
	}
	/*ǰ10������mpu6050�͹������ȶ���������ȷֵ*/
	if(System_time_ms < _CONTROL_START_TIME){
		return mode;
	}
	
	/*�ȴ�������У׼,ESP ready?*/
	if(Qmc5883l.Calibration_state == 1 || Battery_Voltage.Calibration_state == 1 || Esp.Init_State != 2) {
		return mode;
	}
#ifdef DEBUG_MODE
	mode = 6;
#else
	
	if(Remote_Mode == 0 || Remote_Mode == 4){
		Init_PWM = 0;
	}
	
	/*�ж��������*/
	if(init_data.VBAT > 3.85f && Remote_Mode == 2 && System_time_ms > _CONTROL_START_TIME){
		mode = 1;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	else if(init_data.VBAT <= 3.85f){
		mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
#endif	
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻����״̬ 1
 
 * @param  	���˻����״̬
 
 * @retval 	0: mode
						1: mode
						5: mode
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Taskoff_State(uint8_t mode){
	
	float temp_height;
	LED.Task(System_time_us,1);
	
	/*һ���޳����˲����������ж��ǲ�����Ŀ��߶ȸ�������*/
	F_takeoff << current_data.pos_z;
	F_takeoff >> temp_height;
	
	target.pos_z_target = _TARGET_HEIGHT;//Ŀ��߶�
	
	if(current_data.pos_z > _LIMIT_HEIGHT){//�߶ȳ���_LIMIT_HEIGHT ����ͣ��
		mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		return mode;
	}
	
	/*�ж���������*/
	if(current_data.pos_z < 0.1f){//�����л��ж�
		if(current_data.pos_z < 0.06f){//�ж��Ƿ��ڵ���
			Controller_State = 2;//2��ֻ������̬����
			if(Init_PWM < 8000){
				Init_PWM = 4000;
				Init_PWM += 10;
			}
		}
		else{//�ж��Ƿ�����
			Controller_State = 3;//3:ֻ���и߶�+��̬����
		}
		Flow.sum_flow_x = 0;
		Flow.sum_flow_y = 0;
	}
	else{
		Controller_State = 1;//1:���п�������
		mode = 2;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻�ң��������ģʽ 2
 
 * @param  	ң��������ģʽ
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
extern Serial_Remote Remote;
extern struct serial_data serial_remote;
uint8_t GearDrone::Remote_State(uint8_t mode){
	
	LED.Task(System_time_us,2);
	
	if(Remote_Mode == 0){//ң��������
		mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	if(Remote_Mode == 2){
		mode = 2;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
	}
	#ifndef DEBUG_MODE
	else if(Remote_Mode == 3){//formation mode
		mode = 3;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		//...
	}
	else if(Remote_Mode == 4){//landing
		mode = 4;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		//...
	}
//	else if(target.pos_z_target < 0.25f){
//		Drone_Mode = 4;
//	}
	#endif
	return mode;
}

/********************************************************************************
 * @brief  	���˻���ӿ���ģʽ 3
 
 * @param  	����д��ӿ����㷨������Ŀ��λ�� �ٶ� ���ٶ�
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Formation_State(uint8_t mode){
	
	LED.Task(System_time_us,3);
	
	if(Remote_Mode == 2){//remote mode
		mode = 2;
	}
	else if(1){
		mode = 4;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		//...
	}
	
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻�����ģʽ 4
 
 * @param  	����ģʽ
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Landing_State(uint8_t mode){
	
	LED.Task(System_time_us,4);
	
	if(Init_PWM >= 3500){
//		Controller_State = 2;//ֻ������̬����
//		Height_PID.Out = 0;
//		Height_Speed_PID.Out = 0;
//		Init_PWM -= 150;
		Controller_State = 0;//0:����ʧЧ
		Init_PWM = 0;
		mode = 5;
	}
	else if(current_data.pos_z < 0.2f){
		Controller_State = 0;//0:����ʧЧ
		Init_PWM = 0;
		mode = 5;
	}
	
	return mode;
	
}

/********************************************************************************
 * @brief  	���˻�״̬������
 
 * @param  	ʱ��׼�������쳣ģʽ
 
 * @retval 	
 * @author	EXcai(��������)
 *******************************************************************************/
uint8_t GearDrone::Error_Monitor(){
	
	if(Drone_Mode != 0 && Drone_Mode != 5){
		
		/*�Ƕȳ�����ֵ*/
		if(fabsf(current_data.pitch) > 35 || fabsf(current_data.roll) > 35){
			Drone_Mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		}
		/*MPU6050�����*/
		if(current_data.pitch == 0 || current_data.roll == 0){
			Drone_Mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		}
		
		/*�߶ȳ�����ֵ*/
		if(current_data.pos_z > _LIMIT_HEIGHT){
			Drone_Mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		}
	}
	
#ifndef DEBUG_MODE
	if(Esp.heart != 0 && Esp.Init_State == 2){//ң�������߹�
		if(Remote_Mode == 0){//ң��������
			Drone_Mode = 5;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		}
		if(Remote_Mode == 4){//landing
			Drone_Mode = 4;//0��ʼ�� 1��� 2ң�������� 3��ӿ��� 4���� 5����ͣ�� 6����ģʽ
		}
	}
#endif
	
	return 0;
	
}

