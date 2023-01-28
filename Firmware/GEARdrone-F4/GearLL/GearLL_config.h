/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    GearLL_config.h
  * @author  EXcai(��������)
  * @brief   GearLL�������ļ�����������Ӳ����ͬ�������л�
  *
  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *

  *******************************************************************************/
#pragma once

#ifndef __GEARLL_CONFIG_H__
#define __GEARLL_CONFIG_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include <string.h>

#define LP_Mode 0 //uwb�ֲ���λģʽ
#define DR_Mode 1 //uwb���ģʽ

#define _CONTROL_START_TIME 10000 //��ʼ����ʱ��
#define _CONTROL_LIMIT_TIME 18000 //���Ʒ���ʱ�䣨ֻ�ڵ���ģʽ��Ч

#define _LIMIT_HEIGHT 1.4f //���Ʒ��и߶�
#define _TARGET_HEIGHT 0.4f //Ŀ����и߶�

#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);//LED ����
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);//LED off

extern uint32_t System_time_us;

/*********************************************************************************
	* @brief  ������ʹ��HMC5883L�Ĺ������:QMC5883L
	* @param  -ע�͸����,���ø�define,��Ϊʹ��HMC5883L����������֮��ΪQMC5883L
						-ʹ��QMC5883L�����������HMC5883L�����Ĵ���λ�ú�HMC��һ��(ʵ����������һ���������ֶ�����,��������)

	* @author EXcai��������
	********************************************************************************/

#define USE_QMC 

/*********************************************************************************
	* @brief  ��MPUʹ�����iic
	* @param  �����Ϊʹ�����IIC������MPU6050���ô��ǲ�����Ϊstm32��Ӳ��iic bug��������˻�������������

	* @author EXcai��������
	********************************************************************************/

//#define USE_SOFT_IIC

/*********************************************************************************
	* @brief  ʹ�õ�����λ��
	* @param  -ʹ�õ�����λ��
						-��ģʽ������private��protected��Ա������public ������uppermonitor�鿴

	* @author EXcai��������
	********************************************************************************/
	
//#define USE_UPPERMONITOR 

#ifdef USE_UPPERMONITOR
	#include "UpperMonitor.h"
#endif

/*********************************************************************************
	* @brief  ʹ�õ��Ե���ģʽ
	* @param  -��ģʽ�����˻��������Զ�������̬ģʽ���������״̬

	* @author EXcai��������
	********************************************************************************/

//#define DEBUG_MODE

/*********************************************************************************
	* @brief  ��Ҫת��������
	* @param  ȡ��ע��,���ø�define,���˻�������ת����������

	* @author EXcai��������
	********************************************************************************/

//#define NO_MOTOR

#endif
