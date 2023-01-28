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
			- "An Ultra-Wideband-based Multi-UAV Localization System in GPS-denied environments"
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
#include "NLR_localization.h"
#include "Drone.h"

/* Private variables ---------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

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
float NLR::Nlr_Estimate(float duwb[7]){
	
//	for(int i=0;i<7;i++){
//		if(duwb[i] == 0){
//			error_step = 255;
//			return error_step;//������鲻����
//		}
//	}
	
	/* Cols1 -------------------------------------------------------*/
	JK[0] = (unk[0]-known[0])/__sqrtf(powf((unk[0]-known[0]),2) + powf((unk[1]-known[1]),2));
	JK[1] = (unk[1]-known[1])/__sqrtf(powf((unk[0]-known[0]),2) + powf((unk[1]-known[1]),2));
	/* Cols2 -------------------------------------------------------*/
	JK[6] = (unk[2]-known[0])/__sqrtf(powf((unk[2]-known[0]),2) + powf((unk[3]-known[1]),2));
	JK[7] = (unk[3]-known[1])/__sqrtf(powf((unk[2]-known[0]),2) + powf((unk[3]-known[1]),2));
	/* Cols3 -------------------------------------------------------*/
	JK[8] = (unk[0]-known[2])/__sqrtf(powf((unk[0]-known[2]),2) + powf((unk[1]-known[3]),2));
	JK[9] = (unk[1]-known[3])/__sqrtf(powf((unk[0]-known[2]),2) + powf((unk[1]-known[3]),2));
	/* Cols4 -------------------------------------------------------*/
	JK[14] = (unk[2]-known[2])/__sqrtf(powf((unk[2]-known[2]),2) + powf((unk[3]-known[3]),2));
	JK[15] = (unk[3]-known[3])/__sqrtf(powf((unk[2]-known[2]),2) + powf((unk[3]-known[3]),2));
	/* Cols5 -------------------------------------------------------*/
	JK[16] = (unk[0]-known[4])/__sqrtf(powf((unk[0]-known[4]),2) + powf((unk[1]-known[5]),2));
	JK[17] = (unk[1]-known[5])/__sqrtf(powf((unk[0]-known[4]),2) + powf((unk[1]-known[5]),2));
	/* Cols6 -------------------------------------------------------*/
	JK[22] = (unk[2]-known[4])/__sqrtf(powf((unk[2]-known[4]),2) + powf((unk[3]-known[5]),2));
	JK[23] = (unk[3]-known[5])/__sqrtf(powf((unk[2]-known[4]),2) + powf((unk[3]-known[5]),2));
	/* Cols7 -------------------------------------------------------*/
	JK[24] = (unk[0]-unk[2])/__sqrtf(powf(unk[0]-unk[2],2) + powf(unk[1]-unk[3],2));
	JK[25] = (unk[1]-unk[3])/__sqrtf(powf(unk[0]-unk[2],2) + powf(unk[1]-unk[3],2));
	JK[26] = (unk[2]-unk[0])/__sqrtf(powf(unk[2]-unk[0],2) + powf(unk[3]-unk[1],2));
	JK[27] = (unk[3]-unk[1])/__sqrtf(powf(unk[2]-unk[0],2) + powf(unk[3]-unk[1],2));
	
	jk_mat.numCols = 4;//��
	jk_mat.numRows = 7;//��
	jk_mat.pData = JK;
	
	jk_trans_mat.numCols = 7;
	jk_trans_mat.numRows = 4;
	jk_trans_mat.pData = jk_trans_temp;
	error_step = arm_mat_trans_f32(&jk_mat,&jk_trans_mat);//��ת��
	if(error_step != 0){error_step = 10-error_step;return error_step;}
	
	jk_multi1.numCols = 4;
	jk_multi1.numRows = 4;
	jk_multi1.pData = jk_multi_temp1;
	error_step = arm_mat_mult_f32(&jk_trans_mat,&jk_mat,&jk_multi1);//����˷�
	if(error_step != 0){error_step = 20-error_step;return error_step;}
	
	jk_inverse.numCols = 4;
	jk_inverse.numRows = 4;
	jk_inverse.pData = jk_inverse_temp;
	error_step = arm_mat_inverse_f32(&jk_multi1,&jk_inverse);//����
	if(error_step != 0){error_step = 30-error_step;return error_step;}
	
	jk_final.numCols = 7;
	jk_final.numRows = 4;
	jk_final.pData = jk_final_temp;
	error_step = arm_mat_mult_f32(&jk_inverse,&jk_trans_mat,&jk_final);//�����ձ任��JK���
	if(error_step != 0){error_step = 40-error_step;return error_step;}
	 
	jk_scale.numCols = 7;
	jk_scale.numRows = 4;
	jk_scale.pData = jk_scale_temp;
	error_step = arm_mat_scale_f32(&jk_final,0.5f,&jk_scale);//���Ʋ���Ϊ0.5f
	if(error_step != 0){error_step = 50-error_step;return error_step;}
	
	/* ���deltaD = ���β������� - ���ι��ƾ���*/
	des[0] = duwb[0] - __sqrtf(powf((unk[0]-known[0]),2) + powf((unk[1]-known[1]),2));
	des[1] = duwb[1] - __sqrtf(powf((unk[2]-known[0]),2) + powf((unk[3]-known[1]),2));
	des[2] = duwb[2] - __sqrtf(powf((unk[0]-known[2]),2) + powf((unk[1]-known[3]),2));
	des[3] = duwb[3] - __sqrtf(powf((unk[2]-known[2]),2) + powf((unk[3]-known[3]),2));
	des[4] = duwb[4] - __sqrtf(powf((unk[0]-known[4]),2) + powf((unk[1]-known[5]),2));
	des[5] = duwb[5] - __sqrtf(powf((unk[2]-known[4]),2) + powf((unk[3]-known[5]),2));
	des[6] = duwb[6] - __sqrtf(powf(unk[0]-unk[2],2) + powf(unk[1]-unk[3],2));
	des_mat.numCols = 1;
	des_mat.numRows = 7;
	des_mat.pData = des;
	
	d_delta.numCols = 1;
	d_delta.numRows = 4;
	d_delta.pData = des_current;
	error_step = arm_mat_mult_f32(&jk_scale,&des_mat,&d_delta);//��delta_distance
	if(error_step != 0){error_step = 60-error_step;return error_step;}
	
	unk[0] = unk[0] + des_current[0];
	unk[1] = unk[1] + des_current[1];
	unk[2] = unk[2] + des_current[2];
	unk[3] = unk[3] + des_current[3];
	
	return 0;
}
