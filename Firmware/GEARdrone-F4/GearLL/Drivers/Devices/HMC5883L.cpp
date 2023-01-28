/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    HMC5883L.cpp
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
/* Includes ------------------------------------------------------------------*/
#include "filter.h"
#include "i2c_def.h"
#include "HMC5883L.h"

uint8_t QMC_Data_Status(){
	
	uint8_t Sta;
	
	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); 
  TOF_I2C_WaitAck(); 
  TOF_I2C_WriteByte(0x06);
  TOF_I2C_WaitAck(); 
  TOF_I2C_Stop();

  TOF_I2C_Start();          
  TOF_I2C_WriteByte(0x1b); 
  TOF_I2C_WaitAck();
	Sta = TOF_I2C_ReadByte(1); 
	TOF_I2C_Stop();
	
	return Sta;
}

uint8_t HMC5883L_Init(void)
{
	uint8_t Sta;
	#ifdef USE_QMC
	/* ��λ*/
	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x0A);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x80); 
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();
	TOF_I2C_Start();
	
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x0B);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x01); 
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();

	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x20);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x40); 
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();
	
	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x21);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x01); 
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();
	
  TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x09);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x1D); //������Χ8G,��������ģʽ,��������512hz,�������100hz
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();
	
	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); //дָ��
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x0A);
  TOF_I2C_WaitAck();
  TOF_I2C_WriteByte(0x41); //����λ���Զ�������ȡָ�룬�ر��ж�ģʽ
  TOF_I2C_WaitAck();
  TOF_I2C_Stop();
	
	TOF_I2C_Start();
  TOF_I2C_WriteByte(0x1a); 
  TOF_I2C_WaitAck(); 
  TOF_I2C_WriteByte(0x0D);
  TOF_I2C_WaitAck(); 
  TOF_I2C_Stop();

  TOF_I2C_Start();          
  TOF_I2C_WriteByte(0x1b); 
  TOF_I2C_WaitAck();
	Sta = TOF_I2C_ReadByte(1); 
//	TOF_I2C_Stop();
	return Sta;
	#endif
	#ifndef USE_QMC
    TOF_I2C_Start();
    TOF_I2C_WriteByte(0x3c); 
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x00);
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x58);  //�������75hz
    TOF_I2C_Stop();

    TOF_I2C_Start();
    TOF_I2C_WriteByte(0x3c); //дָ��
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x01);
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x60); //������Χ
    TOF_I2C_WaitAck();
    TOF_I2C_Stop();

    TOF_I2C_Start();
    TOF_I2C_WriteByte(0x3c); //дָ��
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x02);
    TOF_I2C_WaitAck();
    TOF_I2C_WriteByte(0x00); //��������ģʽ
    TOF_I2C_WaitAck();
    TOF_I2C_Stop();
		return 0;
	#endif
}
void HMC5883L_ReadAngle(int16_t *x,int16_t *y,int16_t *z){

	uint8_t XYZ_Data[6]={0};
 
	#ifndef USE_QMC
    TOF_I2C_Start();
    TOF_I2C_WriteByte(0x3c); 
    TOF_I2C_WaitAck(); 
    TOF_I2C_WriteByte(0x03);  //X�����ݵ�ַ
    TOF_I2C_WaitAck(); 
    TOF_I2C_Stop();

    TOF_I2C_Start();          
    TOF_I2C_WriteByte(0x3d); 
    TOF_I2C_WaitAck();
    
    for(uint8_t i=0;i<5;i++)     
    {
    XYZ_Data[i]=TOF_I2C_ReadByte(1);
    }

    XYZ_Data[5] =TOF_I2C_ReadByte(0);  
    TOF_I2C_Stop();

    *x = (int16_t)(XYZ_Data[0]<<8)|XYZ_Data[1];
    *y = (int16_t)(XYZ_Data[4]<<8)|XYZ_Data[5];
		*z = (int16_t)(XYZ_Data[2]<<8)|XYZ_Data[3];
	#endif
	#ifdef USE_QMC
			TOF_I2C_Start();
			TOF_I2C_WriteByte(0x1A); 
			TOF_I2C_WaitAck(); 
			TOF_I2C_WriteByte(0x00);  //X�����ݵ�ַ
			TOF_I2C_WaitAck(); 
			TOF_I2C_Stop();
	
			TOF_I2C_Start();          
			TOF_I2C_WriteByte(0x1B); 
			TOF_I2C_WaitAck();
			
			for(uint8_t i=0;i<5;i++)     
			{
			XYZ_Data[i]=TOF_I2C_ReadByte(1);
			}
	
			XYZ_Data[5] =TOF_I2C_ReadByte(0);  
			TOF_I2C_Stop();
			
			*x = (int16_t)(XYZ_Data[1]<<8)|XYZ_Data[0];
			*y = (int16_t)(XYZ_Data[3]<<8)|XYZ_Data[2];
			*z = (int16_t)(XYZ_Data[5]<<8)|XYZ_Data[4];
	#endif
}
