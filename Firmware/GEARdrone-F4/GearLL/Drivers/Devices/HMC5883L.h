/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    HMC5883L.h
  * @author  EXcai
  * @brief   ������HMC5883L����
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
#ifndef __HMC5883L_H_
#define __HMC5883L_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "Sensors.h"
#include "math.h"
#include "flash_driver.h"

/* Variables -------------------------------------------------------------------*/

struct mag_data{
	
	bool Status;
	uint32_t timestamp_ms;
	
	int16_t X_HM,Y_HM,Z_HM;
	int16_t Axis_offset[6];
};

void HMC5883L_ReadAngle(int16_t *x,int16_t *y,int16_t *z);
uint8_t HMC5883L_Init(void);
uint8_t QMC_Data_Status(void);
extern void delay_us(uint16_t nus);

/** 
* @brief Class for HMC5883L.
*/
class HMC5883L : public Sensor<mag_data>{
public:
	HMC5883L(){};
	virtual ~HMC5883L(){};
	uint8_t HMC5883L_ID;

	virtual uint8_t Init(){
		HMC5883L_ID = HMC5883L_Init();
		//�����Ҫ�����Ѿ�У׼��Ĵ��������ݣ���ȡ��ע����������ָ�Ȼ������һ�����ע�ͻ�ȥ
//		Flash_erase(ADDR_FLASH_SECTOR_7, 6);
//		Flash_write_single(ADDR_FLASH_SECTOR_7, (uint32_t *)Axis_offset, 6);
		HAL_Delay(50);
		Flash_read(ADDR_FLASH_SECTOR_7, (uint32_t *)Axis_offset, 6);
		if(HMC5883L_ID == 0xff){return 0;}
		else{return 1;}
	}
	
	virtual bool Calibration_Status(){
		if(Axis_offset[0] == 0 || Axis_offset[1] == 0 || Axis_offset[2] == 0){
			return 1;
		}
		else return 0;
	}
	
	virtual uint8_t Calibration(){
		if(System_time_us < 40000000){
			HMC5883L_ReadAngle(&X_HM,&Y_HM,&Z_HM);
			if(Axis_max[0] < X_HM){Axis_max[0] = X_HM;}
			if(Axis_min[0] > X_HM){Axis_min[0] = X_HM;}
			if(Axis_max[1] < Y_HM){Axis_max[1] = Y_HM;}
			if(Axis_min[1] > Y_HM){Axis_min[1] = Y_HM;}
			if(Axis_max[2] < Z_HM){Axis_max[2] = Z_HM;}
			if(Axis_min[2] > Z_HM){Axis_min[2] = Z_HM;}
			Axis_offset[0] = (Axis_max[0] + Axis_min[0])/2;
			Axis_offset[1] = (Axis_max[1] + Axis_min[1])/2;
			Axis_offset[2] = (Axis_max[2] + Axis_min[2])/2;
			Axis_offset[3] = (Axis_max[0] - Axis_min[0]);
			Axis_offset[4] = (Axis_max[1] - Axis_min[1]);
			Axis_offset[5] = (Axis_max[2] - Axis_min[2]);
		}
		else{
			Flash_erase(ADDR_FLASH_SECTOR_7, 6);
			Flash_write_single(ADDR_FLASH_SECTOR_7, (uint32_t *)Axis_offset, 6);
			return 0;
		}
		return 1;
	}
	virtual struct mag_data Get_Value(){
		
		mag_data data;
		
//		#ifdef USE_QMC
//			Status = QMC_Data_Status();
//		#endif
		
		HMC5883L_ReadAngle(&X_temp,&Y_temp,&Z_temp);
		if(X_temp != (int16_t)0xffff && Y_temp != (int16_t)0xffff && Z_temp != (int16_t)0xffff){X_HM = X_temp;Y_HM = Y_temp;Z_HM = Z_temp;}//��֤��ͷ��ǰ�����£�XH��ǰָ��YH����ָ���Դ�Ϊ��׼ȷ��XH��YH���ڵ���ʵXY��������XHYH��оƬXY���Ĺ�ϵ
		data.timestamp_ms = System_time_us/1000;
		
		data.X_HM = ((X_HM - Axis_offset[0])*1000.0f/Axis_offset[3]);//��֤��ͷ��ǰ�����£�XH��ǰָ��YH����ָ���Դ�Ϊ��׼ȷ��XH��YH���ڵ���ʵXY��������XHYH��оƬXY���Ĺ�ϵ
		data.Y_HM = ((Y_HM - Axis_offset[1])*1000.0f/Axis_offset[4]);
		data.Z_HM = ((Z_HM - Axis_offset[2])*1000.0f/Axis_offset[5]);
		memcpy(data.Axis_offset,Axis_offset,sizeof(Axis_offset));
		
		XVA = data.X_HM;
		YVA = data.Y_HM;
		ZVA = data.Z_HM;
		return data;
	}
	uint8_t Status;
	
#ifndef USE_UPPERMONITOR
	private:
#endif
	int16_t X_temp,Y_temp,Z_temp;
	int16_t X_HM,Y_HM,Z_HM;
	float XVA,YVA,ZVA;
	int16_t Axis_offset[6],Axis_min[3],Axis_max[3];//0~2˳��x y z�� Axis_offset[6]�ֱ���xyz��offset��xyz�ķ�Χ�����ڹ�һ����

};

#endif
