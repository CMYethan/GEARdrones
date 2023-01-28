/**
  ******************************************************************************
  * Copyright (c) 2022 - ~, EXcai(��������)
  * @file    NLR_localization.cpp
  * @author  EXcai
  * @brief   UWB ���ڷ����Իع���������˻���ʼ����λ(5��)
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
#include "System_Startup.h"
#include "arm_math.h"

/* Private variables ---------------------------------------------------------*/

class NLR{
	public:
		NLR(float x0,float y0,float x1,float y1,float x2,float y2){
			known[0] = x0;
			known[1] = y0;
			known[2] = x1;
			known[3] = y1;
			known[4] = x2;
			known[5] = y2;
		}
		~NLR(){};
			
/********************************************************************************
 * @brief  ִ��һ�ι��� 
 * @param  	��������uwb��������distance[10]
 * @retval 	�������δ֪����Ĺ���unk[6]
 * @attention 
							-�ú�������5����ӵĳ�ʼ��λ�ù��ƣ����ǰ��Ҫ�ں�����ʵʱ�������ݲ�ͣ���ƣ���ܷɻ��ı�ű���Ϊ0-4����
							-�ú����Ե�0 ��1 ��2��Ϊê�㣬������Ϊ0�ķɻ�������ԭ�㴦�����Ϊ1�ķɻ���ȷ������(0,1m)�������Ϊ2�ķɻ���ȷ������(0,1m)��
							-5�ܷɻ����з�ʽ���£�	
									���Ϊ0�ķɻ�������ԭ�㴦�����Ϊ1�ķɻ���ȷ������(1m,0)�������Ϊ2�ķɻ���ȷ������(0,1m)��
									���Ϊ3�ķɻ�������(1,1m)��(���λ��)
									���Ϊ4�ķɻ�������(0.5m,0.5m)��(���λ��)
									UAV2       	UAV3
												UAV4
									UAV0				UAV1
			
 * @author EXcai(��������)
 *******************************************************************************/
		float Nlr_Estimate(float duwb[7]);//
		float unk[4] = {2.4f,2.4f,1.2f,1.2f};//�Ǹ�����������ʾδ֪����ĳ�ʼ�����ƣ�(x4,y4,x5,y5)' ���˻�Ӧ���Ա߳�Ϊ1���������Ų����м���õ�5�ܷɻ�

			
	private:
			float error_step;//NLR���ƴ���255Ϊ������鲻���������������ʮλ��Ϊ������Ĳ��裬��λ��Ϊ������������������ �������arm_math���Ӧ�Ĵ���(����˵11������1���������Ϊ1�����ΪOne or more arguments are incorrect)
			float known[6];//�Ǹ�����������ʾ��֪���꣺(x1,y1,x2,y2,x3,y3)' ������Ϊ0�ķɻ�������ԭ�㴦�����Ϊ1�ķɻ���ȷ������(1m,0)��
	
			float des[7];//���ι��ƾ���(des:distance_estimate)
			float JK[28];
			float estimate_error[7];//����"���"�����ֵunk[]�ȣ�
			
			arm_matrix_instance_f32 jk_mat;
			arm_matrix_instance_f32 unk_mat;
			arm_matrix_instance_f32 des_mat;
			arm_matrix_instance_f32 jk_trans_mat;//��ת��
			arm_matrix_instance_f32 jk_multi1;//����˷�
			arm_matrix_instance_f32 jk_inverse;//����
			arm_matrix_instance_f32 jk_final;//�����ձ任��JK���
			arm_matrix_instance_f32 jk_scale;//���Ʋ���
			arm_matrix_instance_f32 d_delta;//��delta_distance
			
			float32_t jk_trans_temp[28],jk_multi_temp1[16],jk_inverse_temp[16],jk_final_temp[28],jk_scale_temp[28],des_current[4];
};
