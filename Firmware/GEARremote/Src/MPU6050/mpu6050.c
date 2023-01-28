#include "mpu6050.h"
#include "IMU_transform.h"
void MPU_Init(void)
{
	
	MPU_I2C_Init();
//	Delay_MS(10);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);//1
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG,0x07);//2
	MPU_Write_Byte(MPU_CFG_REG,0x06);//3
	MPU_Write_Byte(MPU_GYRO_CFG_REG,0X18);//4
	MPU_Write_Byte(MPU_ACCEL_CFG_REG,0X00);//5

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

//�������˲�
#if 1//

#define PI 3.14159
//�ǶȲ���
float angle, angle_dot; 
float Angle_aXZ;          //��X��Z���ϵļ��ٶȴ�������õ���ֵ���������б�Ƕ�
float Angle_gY;           //��Y��������Ǵ�������õ���ֵ����������ٶ�
//����������		
float  Q_angle=0.002;            
float  Q_gyro=0.01;              
float  R_angle=0.003;	//�ǶȲ����ٶȿ죬�ӳٵ�

//float  Q_angle=0.001;            
//float  Q_gyro=0.003;              
//float  R_angle=0.5;	

float  dt=0.005;	       //dtΪkalman�˲�������ʱ��;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } };
// �������˲�
void Kalman_Filter(float Accel,float Gyro)		
{
	angle+=(Gyro - Q_bias) * dt; //�������

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= - PP[1][1];
	Pdot[2]= - PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	angle_dot = Gyro - Q_bias; //���ֵ(�������)��΢��=���ٶ�
}
float Angle_X, Angle_Y,Angle_Z;
void Get_Angle(void)
{ 
	   short  Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_X,Gyro_Z;  
			//��ȡY��gyro
			MPU_Get_Single_Data(&Gyro_Y,MPU_GYRO_YOUTH_REG);
			//��ȡZ��gyro
			MPU_Get_Single_Data(&Gyro_Z,MPU_GYRO_ZOUTH_REG);
			//��ȡX��gyro
			MPU_Get_Single_Data(&Gyro_X,MPU_GYRO_XOUTH_REG);
		  //��ȡX��accel
			MPU_Get_Single_Data(&Accel_X,MPU_ACCEL_XOUTH_REG);
	  	//��ȡZ��accel
			MPU_Get_Single_Data(&Accel_Z,MPU_ACCEL_ZOUTH_REG);
			//��ȡZ��accel
			MPU_Get_Single_Data(&Accel_Y,MPU_ACCEL_YOUTH_REG);
			
			IMUupdate(Gyro_X,Gyro_Y,Gyro_Z,Accel_X,Accel_Y,Accel_Z);
			
			if(Gyro_Y>32767)  Gyro_Y-=65535;     
			if(Gyro_Z>32767)  Gyro_Z-=65535;
			if(Gyro_X>32767)  Gyro_X-=65535; 	
	  	if(Accel_X>32767) Accel_X-=65535;  
			if(Accel_Y>32767) Accel_X-=65535; 
		  if(Accel_Z>32767) Accel_Z-=65535;   
	   	Accel_Y=atan2((Accel_X+2277),(Accel_Z-1136))*180/PI;       //���������ļн�	
			Accel_X=atan2((Accel_Y+2277),(Accel_Z-1136))*180/PI;       //���������ļн�
		  Gyro_Y=Gyro_Y*0.061036;                                    //����������ת��	  +-2000  65535/4000=16.4  1/16.4=0.061036
			Gyro_X=Gyro_X*0.061036;                                    //����������ת��	  +-2000  65535/4000=16.4  1/16.4=0.061036
			Angle_Y=Gyro_Y+2;                                 		 //*****����ƽ����ٶ�
      Kalman_Filter(Accel_Y,Gyro_Y);//�������˲�	
	    Angle_Y=-angle + 14;                                 //*****����ƽ�����
			Angle_X = Gyro_X;
			Angle_Z = Gyro_Z;
			
}


////��Ǽ���
//void Angle_Calcu(void)	 
//{	
//	//�������б�Ƕ�
//	Angle_aXZ = atan2((MPU_Get_Single_Data(MPU_ACCEL_XOUTH_REG)-0), (MPU_Get_Single_Data(MPU_ACCEL_ZOUTH_REG))-0) *180/PI ;
//  //��б���ٶ�  0.061036=1/16.4  
//	Angle_gY = (MPU_Get_Single_Data(MPU_GYRO_YOUTH_REG)-0)*0.061036 ;		   
//	Kalman_Filter(Angle_aXZ - 0, Angle_gY - 0);       //�������˲��������,��ȥ���ƫ�ƣ�
//}

#endif
