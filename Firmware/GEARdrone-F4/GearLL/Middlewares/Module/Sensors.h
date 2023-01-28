/**
  ******************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    Sensors.h
  * @author  EXcai
  * @brief   ���������࣬������ִ������������̣�����������ֲ
  *
  ==============================================================================
													How to use this library ��ôʹ������⣿
  ==============================================================================
    @note
		- ������ο�HMC5883L:
		- 1.�����½���Ĵ�������cpp�ļ���h�ļ��̳�Sensor��ȷ�������Ա�����ͳ�Ա����
		- 2.����Init()��Calibration_Status()��Calibration()��Get_Value()��������Ҫ��������ο�Sensors.h�ڵ�ע��
		- 3.��System_Startup.cpp���ļ����½���Ĵ���������
		-	4.��System_Startup.cpp�ļ��е�System_Task_Init������д��������������������Qmc5883l.Startup();
		-	5.��Services_Sensors.cpp�ļ��е�Sensors_Task_50HZ_Function��Sensors_Task_200HZ_Function��Run����������Qmc5883l.Run();
		- 6.һ�������Ļ�ϵͳ����ִ��Calibration()֮����200HZ����50HZִ����Ĵ�����Get_Value()����
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
**/
#ifndef _SENSORS_H_
#define _SENSORS_H_

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include "main.h"
#include "cmsis_os.h"
#include "filter.h"

/** 
* @brief Class for sensors.
*/
template<typename T>
class Sensor{
	public:
		
//		Sensor(uint32_t _frequency,char* _title,const uint8_t _size,const uint8_t _priority):Task_Frequency(_frequency),Task_title(_title),Stack_Size(_size),Priority(_priority){};
		virtual ~Sensor(){};
			
/************************************************************************************************************************************
 * @brief  	��������ʼ������
 * @param  	����Init()�Ա�д��������ʼ��
 * @retval 	0: ��ʼ�����
						1: ��ʼ��ʧ�ܣ��Լ�д����������
 * @author	EXcai(��������)
**************************************************************************************************************************************/	
		virtual uint8_t Init() = 0;
			

/************************************************************************************************************************************
 * @brief  	������У׼�жϺ���
 * @param  	����Calibration_Status()�Ա�д������У׼�ж�
 * @retval 	0: ����У׼
						1: ��ҪУ׼��ϵͳ���Զ�ִ��calibration��������̳�sensor��������calibration�����ľ���У׼����
 * @author	EXcai(��������)
**************************************************************************************************************************************/		
		virtual bool Calibration_Status() = 0;
			
			
/************************************************************************************************************************************
 * @brief  	������У׼����
 * @param  	����Calibration()�Ա�д������У׼���̣�����Calibration()����1ʱ��Calibration����10msΪ���ڱ�ѭ��ִ�У�ֱ�����ص�calibration�ڷ���0��
 * @retval 	0: У׼�ɹ���������һ��Get_Value()
						1: ����У׼��У׼ʧ�ܣ�10ms�������������һ��Calibration()У׼��
 * @author	EXcai(��������)
**************************************************************************************************************************************/
		virtual uint8_t Calibration() = 0;
		
/************************************************************************************************************************************
 * @brief  	���������к���
 * @param  	����Get_Value()�Ա�д���������к���

 * @author	EXcai(��������)
**************************************************************************************************************************************/	
		virtual T Get_Value() = 0;
	
	
/************************************************************************************************************************************
 * @brief  	������ִ�к���
 
 * @param  	����Get_value()�Ա༭���������л�ȡ���ݵ���ϸ���裬
						����Calibration_Status()���ж��Ƿ���ҪУ׼������������У׼����(ע�⣡��Ҫ����У׼�򷵻�1�����򷵻�0)
						����Calibration()�Ա�д������У׼���̣�����Calibration()����1ʱ��Calibration����10msΪ���ڱ�ѭ��ִ�У�ֱ�����ص�calibration�ڷ���0(ע�⣡��Ҫ�ٴ�У׼�򷵻�1�����򷵻�0)
 * @retval 	0: �ɹ�
						1: ����ʧ��
 * @author	EXcai(��������)
**************************************************************************************************************************************/	
			T	Run(){

			/* �ж��Ƿ�ִ��У׼ */
			T t;
			if(Calibration_Status()){
				while(Calibration()){
					vTaskDelay(10);
					Calibration_state = 1;
				}
				return t;
			}
			else{
				Calibration_state = 0;
				return Get_Value();
			}
		}
		
/**************************************************************************************************************************************
 * @brief  	��������������
 
 * @param  	����Init()�Գ�ʼ����������
 
 * @retval 	0: �ɹ�
						1: ����ʧ��
 * @author	EXcai(��������)
**************************************************************************************************************************************/	
		uint8_t Startup(){
			return Init();
		}
		
//		static void staticTask_Function(void *pvParameter);
		void Task_Function();
		TaskHandle_t Task_Handle;

		uint8_t Calibration_state;
	protected:
		uint32_t Task_Frequency;
};

#endif

#endif
