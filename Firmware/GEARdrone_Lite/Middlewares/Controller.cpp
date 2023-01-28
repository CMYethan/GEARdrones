/********************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @author  EXcai
  * @brief   ���˻����п�����
  *
  ==============================================================================
													 ���ļ�������������
  ==============================================================================
    @note
			-RPY������ٶ�pid����
			-RPY����Ƕ�pid����
			-RPY�����ٶ�pid����
			-RPY����λ��pid����
			-matlab�ӹܵ�����λ�ÿ���

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

  ********************************************************************************/
#include "Drone.h"

extern UWB Uwb;

extern uint8_t remote_command;
extern int16_t flow_x,flow_y;
extern float pos_x_flow,pos_y_flow,tof_height;		/*�������ݴ���*/

volatile uint32_t SystemTimerCnt;
uint32_t Get_SystemTimer(void)
{
	return htim4.Instance->CNT + SystemTimerCnt * 0xffff;
} 
/************************************************************
 * @brief  	������ٶ�pid����
 
 * @param  	������ٶ�pid����
 * @retval 	
 * @author EXcai(��������)
 ***********************************************************/
void GearDrone::angular_velocity_controller(uint8_t controller_state){
	
	myPIDTimer::getMicroTick_regist(Get_SystemTimer);
	
	Yaw_Speed_PID.SetPIDParam(100,0,8,500,5000); //45 0 20
	Pitch_Speed_PID.SetPIDParam(42,0,11,8000,20000);//65 0 30
	Roll_Speed_PID.SetPIDParam(42,0,11,8000,20000);//40 0 15
	
	if(remote_command == 2 || remote_command == 1){
		if(controller_state == 1){
			/*������ٶȿ���*/
			Pitch_Speed_PID.Current = pitch_speed;
			Pitch_Speed_PID.Adjust();
			
			Roll_Speed_PID.Current = roll_speed;
			Roll_Speed_PID.Adjust();
			
			Yaw_Speed_PID.Current = yaw_speed;
			Yaw_Speed_PID.Adjust();
			
			/*���׵Ķ���ѧģ��*/
			LF_PWM = Pitch_Speed_PID.Out + Yaw_Speed_PID.Out - Roll_Speed_PID.Out + Speed_z_PID.Out + Init_PWM;
			RF_PWM = Pitch_Speed_PID.Out - Yaw_Speed_PID.Out + Roll_Speed_PID.Out + Speed_z_PID.Out + Init_PWM;
			LB_PWM = -Pitch_Speed_PID.Out - Yaw_Speed_PID.Out - Roll_Speed_PID.Out + Speed_z_PID.Out + Init_PWM;
			RB_PWM = -Pitch_Speed_PID.Out + Yaw_Speed_PID.Out + Roll_Speed_PID.Out + Speed_z_PID.Out + Init_PWM;
			
			/*����޷�*/
			if(LF_PWM > 20000){LF_PWM = 20000;}
			if(RF_PWM > 20000){RF_PWM = 20000;}
			if(LB_PWM > 20000){LB_PWM = 20000;}
			if(RB_PWM > 20000){RB_PWM = 20000;}
			
			if(LF_PWM <= 0){LF_PWM = 0;}
			if(RF_PWM <= 0){RF_PWM = 0;}
			if(LB_PWM <= 0){LB_PWM = 0;}
			if(RB_PWM <= 0){RB_PWM = 0;}
			
			#if DISABLE_MOTORS //�����Ҫת��
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);//lb
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);//rb
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,0);//lf
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);//rf
			#else
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,LB_PWM);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,RB_PWM);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,LF_PWM);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,RF_PWM); 
			#endif
		}
	}
}

/*����Ƕ�pid����*/
void GearDrone::angle_controller(void){
	
	myPIDTimer::getMicroTick_regist(Get_SystemTimer);
	
	Yaw_PID.SetPIDParam(8,0.5,0,30,100);//3
	Pitch_PID.SetPIDParam(5.5,0.5f,0,75,60);//6.7f,0.05f,0
	Pitch_PID.I_SeparThresh = 10; 
	Roll_PID.SetPIDParam(5.5,0.5f,0,75,60);//5.5f//7
	Roll_PID.I_SeparThresh = 10; 
	
	/*��(����matlab���׿��ƣ����˻��ڿ���������ͣ����matlab�Ѿ��´���ٶ�Ŀ��ָ��ʱ)���Ż���matlab�����ݸ�����̬pid������Ŀ��ֵ*/
	#if MATLAB_SECOND_OREDER_CONTROL
	if(fabsf(pitch) < 20 && fabsf(roll) < 20 && remote_command == 1){
		if(acc_target[0] != 0 || acc_target[1] != 0){
			Roll_PID.Target = acc_target[0];
			Pitch_PID.Target = acc_target[1];
		}
	}
	#endif
	
	Pitch_PID.Current = pitch;
	Pitch_PID.Adjust();	
	Pitch_Speed_PID.Target = Pitch_PID.Out;
	
	Roll_PID.Current = roll;
	Roll_PID.Adjust();
	Roll_Speed_PID.Target = Roll_PID.Out;

	Yaw_PID.Target = yaw_target_angle;
	Yaw_PID.Current = yaw - yaw_init;
	Yaw_PID.Adjust();
	Yaw_Speed_PID.Target = Yaw_PID.Out;
	
}

/************************************************************
 * @brief  	�����ٶ�pid����
 
 * @param  	�����ٶ�pid����
 * @retval 	
 * @author EXcai(��������)
 ***********************************************************/
void GearDrone:: PID_velocity_controller(float x_target, float y_target, float z_target){
	
	myPIDTimer::getMicroTick_regist(Get_SystemTimer);
	
	Speed_x_PID.SetPIDParam(0.62f,0.13f,0.03f,5,10);//0.34f,0.13f,0.03f,5,10
	Speed_x_PID.I_SeparThresh = 0.5;
	Speed_y_PID.SetPIDParam(0.62f,0.13f,0.03f,5,10);//0.43f,0.13f,0.03f,5,10
	Speed_y_PID.I_SeparThresh = 0.5;
	Speed_z_PID.SetPIDParam(3000, 100, 0, 4000, 5000);//9500 500 0
	
	/*���ٶȿ���*/
	if(z_pos < 2.2 && z_pos > 0.05 && remote_command == 1){
		
		Speed_x_PID.Target = x_target;
		Speed_x_PID.Current = x_vel*40;
		Speed_x_PID.Adjust();
		
		Speed_y_PID.Target = y_target;
		Speed_y_PID.Current = y_vel*40;
		Speed_y_PID.Adjust();
		
		Speed_z_PID.Target = z_target;
		Speed_z_PID.Current = z_vel;
		Speed_z_PID.Adjust();
		
		Roll_PID.Target = Speed_x_PID.Out;
		Pitch_PID.Target = -Speed_y_PID.Out;
		
	}
	else{
		Speed_x_PID.Out = 0;
		Speed_y_PID.Out = 0;
		Speed_z_PID.Out = 0;
		Pitch_PID.Target = -target_y_vel/3;
		Roll_PID.Target = target_x_vel/3;
	}
	
	angle_controller();
	
}
/************************************************************
 * @brief  	����λ��pid����
 
 * @param  	����λ��pid����
 * @retval 	
 * @author EXcai(��������)
 ***********************************************************/
void GearDrone:: PID_position_controller(uint8_t controller_state){
	
	float velocity_target[3];// x y z velocity
	
	myPIDTimer::getMicroTick_regist(Get_SystemTimer);
	
	Pos_x_PID.SetPIDParam(6,0.03,0 , 4, 15);//0.55f,0.15f,1.4f, 4, 10
	Pos_y_PID.SetPIDParam(6,0.03,0 , 4, 15);//0.55f,0.15f,1.4f, 4, 10
	Pos_z_PID.SetPIDParam(1.6,0.06,0.15f, 10, 3);//0.65,0.08, 0.7, 3, 4
	Pos_z_PID.I_SeparThresh = 0.15;
	
	if(controller_state == 1){
		
		/*x��λ�ÿ���*/
		if(fabsf(roll) < 20){
			if(z_pos < 0.15){Pos_x_PID.I_Term = 0;Pos_y_PID.I_Term = 0;}//����15cm������λ�û���
			if(target_x_vel != 0){//matlab��ң�������ⲿ�豸�����˿����ٶ�
				target_x_pos = x_pos;
				Pos_x_PID.I_Term = 0;
				velocity_target[0] = target_x_vel;
			}
			else{//matlab��ң�������ⲿ�豸û�д�������ٶȣ���Ҫ��ͣ
				Pos_x_PID.Target = target_x_pos;
				Pos_x_PID.Current = x_pos;
				Pos_x_PID.Adjust();
				velocity_target[0] = Pos_x_PID.Out;
			}
		}
		else{
			velocity_target[0] = 0;
		}
		
		/*y��λ�ÿ���*/
		if(fabsf(pitch) < 20){
			if(z_pos < 0.15){Pos_x_PID.I_Term = 0;Pos_y_PID.I_Term = 0;}//����15cm������λ�û���
			if(target_y_vel != 0){//matlab��ң�������ⲿ�豸�����˿����ٶ�
				target_y_pos = y_pos;//���µ�ǰλ��Ϊ������ͣλ��
				Pos_y_PID.I_Term = 0;
				velocity_target[1] = target_y_vel;
			}
			else{//matlab��ң�������ⲿ�豸û�д�������ٶȣ���Ҫ��ͣ
				Pos_y_PID.Target = target_y_pos;
				Pos_y_PID.Current = y_pos;
				Pos_y_PID.Adjust();
				velocity_target[1] = Pos_y_PID.Out;
			}
		}
		else{
			velocity_target[1] = 0;
		}
		
		/*z��λ�ÿ���*/
		Pos_z_PID.Target = target_z_pos;
		Pos_z_PID.Current = z_pos;
		Pos_z_PID.Adjust();
		
		if(target_z_vel != 0){
			velocity_target[2] = target_z_vel;
		}
		else{
			velocity_target[2] = Pos_z_PID.Out;
		}
		
		 PID_velocity_controller(velocity_target[0],velocity_target[1],velocity_target[2]);
	}
}
/************************************************************
 * @brief  	����λ�ÿ���
 
 * @param  	matlab�ӹܵ�����λ�ÿ���
 * @retval 	
 * @author EXcai(��������)
 ***********************************************************/
void GearDrone::OR_position_controller(uint8_t controller_state,float period){
	
	float velocity_target[3];// x y z velocity
	static uint16_t start_time_10ms;
	static float K_x_axis = 0.91f,K_y_axis = 0.91f,K_z_axis = 0.985f;
	static float height_target = 0.4f;
	static float u[3];
	
	if(controller_state == 1){
		if(start_time_10ms == 0){start_time_10ms = systemtime_10ms;}
		
		/*xλ�ÿ���*/
		if(fabsf(roll) < 20){
			if(target_x_vel != 0){//matlab��ң�������ⲿ�豸�����˿����ٶ�
				target_x_pos = x_pos;
				velocity_target[0] = target_x_vel;
			}
			else{//matlab��ң�������ⲿ�豸û�д�������ٶȣ���Ҫ��ͣ
				u[0] = ((target_x_pos - x_pos) + K_x_axis*(x_pos - target_x_pos))/T;
				velocity_target[0] = u[0];
			}
		}
		else{
			velocity_target[0] = 0;
		}
		
		/*yλ�ÿ���*/
		if(fabsf(pitch) < 20){
			if(target_y_vel != 0){//matlab��ң�������ⲿ�豸�����˿����ٶ�
				target_y_pos = y_pos;//���µ�ǰλ��Ϊ������ͣλ��
				velocity_target[1] = target_y_vel;
			}
			else{//matlab��ң�������ⲿ�豸û�д�������ٶȣ���Ҫ��ͣ
				u[1] = ((target_y_pos - y_pos) + K_y_axis*(y_pos - target_y_pos))/T;
				velocity_target[1] = u[1];
			}
		}
		else{
			velocity_target[1] = 0;
		}

		/*z��λ�ÿ���*/
		u[2] = ((height_target - z_pos) + K_z_axis*(z_pos - height_target))/T;
		velocity_target[2] = u[2];
		
		PID_velocity_controller(velocity_target[0],velocity_target[1],velocity_target[2]);
	}
}
