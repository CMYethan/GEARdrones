#ifndef __BUTTERWORTH_H_
#define __BUTTERWORTH_H_

#include "main.h"
#include "cmsis_os.h"

/*********************************************************************************
		* @brief  ������˹�˲���
		* @author EXcai��������
		********************************************************************************/

#define BLOCK_SIZE 1 /* ����һ��arm_biquad_cascade_df1_f32����Ĳ�������� */
static uint32_t blockSize = BLOCK_SIZE;
class ButterworthFilter{
	public:
		ButterworthFilter(){}
		~ButterworthFilter(){}
		
		/*********************************************************************************
		* @brief  ������˹�˲�����ʼ��
		* @param  params:matlab���ɵ��˲���ϵ��������Ϊorder*5��float�����飬�磺float IIRCoeffs32LP[5*order] = {1.0f, 2.0f, 1.0f, 1.143f,-0.413f};
			        scale��matlab���ɵ�����ϵ��
              order:2��IIR�˲��ĸ���,��6�׾�Ϊ3
			
		* @author EXcai��������
		********************************************************************************/
		ButterworthFilter(float *params, float scale, uint8_t order){
			/* ������˹��ͨ�˲�����ʼ�� */
			IIRCoeffs32LP = params;
			ScaleValue = scale;
			arm_biquad_cascade_df1_init_f32(&S, order, (float *)&IIRCoeffs32LP[0], (float *)&IIRStateF32[0]);
		} 
		
		/*********************************************************************************
		* @brief  �˲�����
		* @param  input����Ҫ���˲���ԭʼ����
			
		* @author EXcai��������
		********************************************************************************/
		float f(float input){
			input_data = input;
			arm_biquad_cascade_df1_f32(&S, &input_data,&output_data,blockSize);
			return output_data*ScaleValue;
		}
	
	private:
		float input_data,output_data;
		float ScaleValue;
		float IIRStateF32[4]; /* ״̬���� */
		float *IIRCoeffs32LP; /* ������˹��ͨ�˲���ϵ��*/ 
		arm_biquad_casd_df1_inst_f32 S;
};

/*********************************************************************************
		* @brief  �����˲���
		* @author EXcai��������
		********************************************************************************/

class ComplementaryFilter
{
	public:
		ComplementaryFilter(float k): K(k){} 
		float f(float value, float d_value,float timespan){
				result = K * value + (1-K) * (last_value + d_value * timespan);
				last_value = result;
				return result;
		}
	protected:
		float K,last_value,result;
};

#endif
