/**
  ******************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    Sensors.cpp
  * @author  EXcai
  * @brief   ���������࣬������ִ������������̣�����������ֲ
  *
  ==============================================================================
													How to use this library ��ôʹ������⣿
  ==============================================================================
    @note
		- �ο�HMC5883L:
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
  */
/* Includes ------------------------------------------------------------------*/
#include "Sensors.h"

/* function prototypes -------------------------------------------------------*/

/**
 * @brief  	Ϊ�˵������Ա��������Ҫһ��ʵ��ָ��,��Ҫ���ּ�ӷ�ʽ����Ա������Ϊ�ص����ݸ� C ϵͳ
 
 * @retval 	
 * @author EXcai(��������)
 */
//void Sensor::staticTask_Function(void *pvParameter)
//{
//	Sensor* sensor = reinterpret_cast<Sensor*>(pvParameter); //obtain the instance pointer
//	sensor->Task_Function(); //dispatch to the member function, now that we have an instance pointer
//}
