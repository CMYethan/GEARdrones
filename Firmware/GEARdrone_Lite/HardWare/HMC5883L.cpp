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
#ifdef __cplusplus    
extern "C" {         
#endif
	
#include "i2c.h"

#ifdef __cplusplus
}
#endif
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
}
void HMC5883L_ReadAngle(int16_t *x,int16_t *y,int16_t *z){

	uint8_t XYZ_Data[6]={0};
 
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
}
