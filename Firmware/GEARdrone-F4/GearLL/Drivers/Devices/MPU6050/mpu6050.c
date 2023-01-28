#include "mpu6050.h"
void MPU6050_Init(void)
{
	MPU_I2C_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);//001 ѡ�� x ������ PLL��Ϊʱ��Դ���Ի�ø��߾��ȵ�ʱ��
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG,0x07);//2
//	MPU_Write_Byte(MPU_CFG_REG,0x03);// ����MPU6050	�ڲ�DLPF�˲�����ֹƵ��42HZ �ͺ�4.8ms
	MPU_Write_Byte(MPU_GYRO_CFG_REG,0X18);//���������������̡�2000��/s
	MPU_Write_Byte(MPU_ACCEL_CFG_REG,0X18);//���ü��ٶȼ�����������16g

}
void MPU_Set_DLPF(uint16_t lpf)
{
	uint8_t data;
	if(lpf>188)data=1;
	else if(lpf>98)data=2;
	else if(lpf>42)data=3;
	else if(lpf>20)data=4;
	else if(lpf>10)data=5;
	else data=6;
	MPU_Write_Byte(MPU_CFG_REG,data);
}
//choose sample frequency
void MPU_Set_SampleFreq(uint16_t Freq)
{
	uint8_t data;
	if(Freq>1000)
	{
		Freq=1000;
	}
	if(Freq<4)
	{
		Freq=4;
	}
	data=1000/Freq-1;
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
	MPU_Set_DLPF(Freq/2);
}


//get temperature
void MPU_Get_Temperature(float *Temperature)
{
		short temp;
	  uint8_t buff[2];	
		MPU_Read_Len(MPU_TEMP_OUTH_REG,2,buff);    //��ȡ�¶�ֵ
    temp= (buff[0] << 8) | buff[1];	
	  *Temperature=((double) temp/340.0)+36.53;
}

//�õ���̬ԭʼ����
void MPU_Get_Gyroscope(short *gyroData)
{
	uint8_t buff[6];
	MPU_Read_Len(MPU_GYRO_XOUTH_REG,6,buff);
	  gyroData[0] = (buff[0] << 8) | buff[1];
    gyroData[1] = (buff[2] << 8) | buff[3];
    gyroData[2] = (buff[4] << 8) | buff[5];
}

//�õ����ٶ�ԭʼ����
void MPU_Get_Accelerometer(short *accData)
{
	 uint8_t buff[6];
   MPU_Read_Len(MPU_ACCEL_XOUTH_REG, 6, buff);
    accData[0] = (buff[0] << 8) | buff[1];
    accData[1] = (buff[2] << 8) | buff[3];
    accData[2] = (buff[4] << 8) | buff[5];
}

//��ȡ��һ����  ����λ�ϲ���λ
//param: �������Ĵ�����λ��ַ
void MPU_Get_Single_Data(short *data,uint8_t reg)
{
	  uint8_t buff[2];	
		MPU_Read_Len(reg,2,buff);    
    *data= (buff[0] << 8) | buff[1];	
}

