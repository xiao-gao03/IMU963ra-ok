#include "pose.h"
#include "math.h"
//#include "zf_common_headfile.h"

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��initPose_Module
//@��������	����ʼ����̬����ģ��
//@����1		��*pose ��̬����ṹ��ָ��
//@����ֵ		����
void initPose_Module(Pose_Module *pose)
{
//��־λ��ʼ��
	pose->flag.run = 1;						//��������
	pose->flag.use_mag = 1;				//ʹ�õ�������
//�ӿڳ�ʼ��
	pose->interface.data.a_x = 0;
	pose->interface.data.a_y = 0;
	pose->interface.data.a_z = 0;
	pose->interface.data.g_x = 0;
	pose->interface.data.g_y = 0;
	pose->interface.data.g_z = 0;
	pose->interface.data.m_x = 0;
	pose->interface.data.m_y = 0;
	pose->interface.data.m_z = 0;
//������ʼ��	
	pose->parameter.error_ki = 0;
	pose->parameter.error_kp = 0.65f;
	pose->parameter.correct_kp = 0.45f;
//�м�������	
	pose->process.error.x = 0;
	pose->process.error.y = 0;
	pose->process.error.z = 0;
	pose->process.error_integral.x = 0;
	pose->process.error_integral.y = 0;
	pose->process.error_integral.z = 0;
	
	pose->process.quaternion[0] = 1;
	pose->process.quaternion[1] = 0;
	pose->process.quaternion[2] = 0;
	pose->process.quaternion[3] = 0;
//���ݳ�ʼ��
	pose->data.rotate_matrix[0][0] = 0;
	pose->data.rotate_matrix[0][1] = 0;
	pose->data.rotate_matrix[0][2] = 0;
	pose->data.rotate_matrix[1][0] = 0;
	pose->data.rotate_matrix[1][1] = 0;
	pose->data.rotate_matrix[1][2] = 0;
	pose->data.rotate_matrix[2][0] = 0;
	pose->data.rotate_matrix[2][1] = 0;
	pose->data.rotate_matrix[2][2] = 0;
	
	pose->data.mag_world.x = 0;
	pose->data.mag_world.y = 0;
	pose->data.mag_world.z = 0;
	
	pose->data.acc_world.x = 0;
	pose->data.acc_world.y = 0;
	pose->data.acc_world.z = 0;
	
	pose->data.mag_correct.x = 0;
	pose->data.mag_correct.y = 0;
	pose->data.mag_correct.z = 0;
	
	pose->data.acc_correct.x = 0;
	pose->data.acc_correct.y = 0;
	pose->data.acc_correct.z = 0;
	
	pose->data.gyro_correct.x = 0;
	pose->data.gyro_correct.y = 0;
	pose->data.gyro_correct.z = 0;
	
	pose->data.pit = 0;
	pose->data.rol = 0;
	pose->data.yaw = 0;
}

void simple_3d_trans(XYZ_Data_f *ref, XYZ_Data_f *in, XYZ_Data_f *out) //С��Χ����ȷ��
{
	static s8 pn;
	static float h_tmp_x,h_tmp_y;
	
	h_tmp_x = sqrt(ref->z * ref->z + ref->y * ref->y);
	h_tmp_y = sqrt(ref->z * ref->z + ref->x * ref->x);
	
	pn = ref->z < 0 ? -1 : 1;
	
	out->x = ( h_tmp_x *in->x - pn *ref->x *in->z ) ;
	out->y = ( pn *h_tmp_y *in->y - ref->y *in->z ) ;
	out->z = ref->x *in->x + ref->y *in->y + ref->z *in->z ;

}


//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��calculatePose_Module
//@��������	����̬����	--	��Ԫ������
//@����1		��*pose ��̬����ṹ��ָ��
//@����2		��cycle ����
//@����ֵ		����
void calculatePose_Module(Pose_Module *pose, float cycle) 
{
	float length;
	XYZ_Data_f acc_tmp; 
	XYZ_Data_f error;
	
	if (pose->flag.run == 0)
		return;
	
/////////////////////////////////////////////////////////////////////////////////////////////////		
	//�������̴���
	if (pose->flag.use_mag == 1)
	{	
		//���õ������̼���yaw
		length = sqrt(pose->data.mag_correct.x * pose->data.mag_correct.x + pose->data.mag_correct.y * pose->data.mag_correct.y);
		if( pose->data.mag_correct.x != 0 && pose->data.mag_correct.y != 0 && pose->data.mag_correct.z != 0 && length != 0)
		{
			pose->process.mag_yaw = arctan2(pose->data.mag_correct.y / length, pose->data.mag_correct.x / length);
		}

		//����yawƫ��
		if(pose->data.rotate_matrix[2][2] > 0.0f )	
		{
			pose->process.mag_yaw_bias = pose->parameter.correct_kp * translateAngle(pose->data.yaw - pose->process.mag_yaw);
			//����ֵ���� -- ����ֵ����
			if(pose->process.mag_yaw_bias > 360 || pose->process.mag_yaw_bias < -360)
			{
				pose->process.mag_yaw_bias = 0;
			}
		}
		
		else
		{
			pose->process.mag_yaw_bias = 0; //�Ƕȹ���ֹͣ������������Ŀ��ֵ���ܲ���ȷ
		}
	}

	else
	{
		pose->process.mag_yaw_bias = 0;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////	
	//���ٶȼƴ���
	length = sqrt(	*(pose->interface.data.a_x) * *(pose->interface.data.a_x) + 
									*(pose->interface.data.a_y) * *(pose->interface.data.a_y) + 
									*(pose->interface.data.a_z) * *(pose->interface.data.a_z));   
	
	if(	ABS(*(pose->interface.data.a_x)) < 1050.0f && 
			ABS(*(pose->interface.data.a_y)) < 1050.0f && 
			ABS(*(pose->interface.data.a_z)) < 1050.0f )
	{	
		//���ٶȼƹ�һ��
		acc_tmp.x = *(pose->interface.data.a_x) / length;
		acc_tmp.y = *(pose->interface.data.a_y) / length;
		acc_tmp.z = *(pose->interface.data.a_z) / length;
		
		//��˼���ƫ��	--	�����˻��ȶ�ʱ���в���
		if(800.0f < length && length < 1200.0f)
		{
			/* ��˵õ���� */
			error.x = (acc_tmp.y * pose->data.rotate_matrix[2][2] - acc_tmp.z * pose->data.rotate_matrix[1][2]);
			error.y = (acc_tmp.z * pose->data.rotate_matrix[0][2] - acc_tmp.x * pose->data.rotate_matrix[2][2]);
	    error.z = (acc_tmp.x * pose->data.rotate_matrix[1][2] - acc_tmp.y * pose->data.rotate_matrix[0][2]);
			
			/* ����ͨ */
			pose->process.error.x += 1.0f * 3.14f * cycle *(error.x  - pose->process.error.x );
			pose->process.error.y += 1.0f * 3.14f * cycle *(error.y  - pose->process.error.y );
			pose->process.error.z += 1.0f * 3.14f * cycle *(error.z  - pose->process.error.z );
		}
	}
	else
	{
		pose->process.error.x = 0; 
		pose->process.error.y = 0  ;
		pose->process.error.z = 0 ;
	}

	// ������	
	pose->process.error_integral.x += pose->process.error.x * pose->parameter.error_ki * cycle;
	pose->process.error_integral.y += pose->process.error.y * pose->parameter.error_ki * cycle;
	pose->process.error_integral.z += pose->process.error.z * pose->parameter.error_ki * cycle;
	
	//�����޷� -- 2������
	pose->process.error_integral.x = LIMIT(pose->process.error_integral.x, - 0.035f ,0.035f );
	pose->process.error_integral.y = LIMIT(pose->process.error_integral.y, - 0.035f ,0.035f );
	pose->process.error_integral.z = LIMIT(pose->process.error_integral.z, - 0.035f ,0.035f );	
	
/////////////////////////////////////////////////////////////////////////////////////////////////
	//��ʼ����������ֵ
	pose->data.gyro_correct.x = (*(pose->interface.data.g_x) - pose->data.rotate_matrix[0][2] * pose->process.mag_yaw_bias) * 0.01745329f + 
						(pose->parameter.error_kp * pose->process.error.x + pose->process.error_integral.x) ;
	pose->data.gyro_correct.y = (*(pose->interface.data.g_y) - pose->data.rotate_matrix[1][2] * pose->process.mag_yaw_bias) * 0.01745329f + 
						(pose->parameter.error_kp * pose->process.error.y + pose->process.error_integral.y) ;		 
	pose->data.gyro_correct.z = (*(pose->interface.data.g_z) - pose->data.rotate_matrix[2][2] * pose->process.mag_yaw_bias) * 0.01745329f +
						(pose->parameter.error_kp * pose->process.error.z + pose->process.error_integral.z) ;
	
/////////////////////////////////////////////////////////////////////////////////////////////////
	// һ���������������Ԫ��ֵ
	pose->process.quaternion[0] += (-pose->process.quaternion[1] * pose->data.gyro_correct.x - pose->process.quaternion[2] * pose->data.gyro_correct.y - pose->process.quaternion[3] * pose->data.gyro_correct.z) * cycle / 2.0f;
	pose->process.quaternion[1] +=  (pose->process.quaternion[0] * pose->data.gyro_correct.x + pose->process.quaternion[2] * pose->data.gyro_correct.z - pose->process.quaternion[3] * pose->data.gyro_correct.y) * cycle / 2.0f;
	pose->process.quaternion[2] +=  (pose->process.quaternion[0] * pose->data.gyro_correct.y - pose->process.quaternion[1] * pose->data.gyro_correct.z + pose->process.quaternion[3] * pose->data.gyro_correct.x) * cycle / 2.0f;
	pose->process.quaternion[3] +=  (pose->process.quaternion[0] * pose->data.gyro_correct.z + pose->process.quaternion[1] * pose->data.gyro_correct.y - pose->process.quaternion[2] * pose->data.gyro_correct.x) * cycle / 2.0f;  

	//��Ԫ����һ��
	length = sqrt(pose->process.quaternion[0] * pose->process.quaternion[0] + 
								pose->process.quaternion[1] * pose->process.quaternion[1] +
								pose->process.quaternion[2] * pose->process.quaternion[2] +
								pose->process.quaternion[3] * pose->process.quaternion[3]);
		
	if (length != 0)
	{
		pose->process.quaternion[0] /= length;
		pose->process.quaternion[1] /= length;
		pose->process.quaternion[2] /= length;
		pose->process.quaternion[3] /= length;
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
	//������ת����
	pose->data.rotate_matrix[0][0] = 	pose->process.quaternion[0] * pose->process.quaternion[0] + pose->process.quaternion[1] * pose->process.quaternion[1] - 
																		pose->process.quaternion[2] * pose->process.quaternion[2] - pose->process.quaternion[3] * pose->process.quaternion[3];
	pose->data.rotate_matrix[0][1] = 	2 * (pose->process.quaternion[1] * pose->process.quaternion[2] + pose->process.quaternion[0] * pose->process.quaternion[3]);
	pose->data.rotate_matrix[0][2] =  2 * (pose->process.quaternion[1] * pose->process.quaternion[3] - pose->process.quaternion[0] * pose->process.quaternion[2]);
	
	pose->data.rotate_matrix[1][0] =  2 * (pose->process.quaternion[1] * pose->process.quaternion[2] - pose->process.quaternion[0] * pose->process.quaternion[3]);
	pose->data.rotate_matrix[1][1] = 	pose->process.quaternion[0] * pose->process.quaternion[0] - pose->process.quaternion[1] * pose->process.quaternion[1] + 
																		pose->process.quaternion[2] * pose->process.quaternion[2] - pose->process.quaternion[3] * pose->process.quaternion[3];
	pose->data.rotate_matrix[1][2] =  2 * (pose->process.quaternion[2] * pose->process.quaternion[3] + pose->process.quaternion[0] * pose->process.quaternion[1]);
	
	pose->data.rotate_matrix[2][0] = 	2 * (pose->process.quaternion[1] * pose->process.quaternion[3] + pose->process.quaternion[0] * pose->process.quaternion[2]);
	pose->data.rotate_matrix[2][1] = 	2 * (pose->process.quaternion[2] * pose->process.quaternion[3] - pose->process.quaternion[0] * pose->process.quaternion[1]);
	pose->data.rotate_matrix[2][2] = 	pose->process.quaternion[0] * pose->process.quaternion[0] - pose->process.quaternion[1] * pose->process.quaternion[1] - 
																		pose->process.quaternion[2] * pose->process.quaternion[2] + pose->process.quaternion[3] * pose->process.quaternion[3];
	
	//������������ϵ�µĴ�����ֵ
	if (pose->flag.use_mag == 1)
	{
		pose->data.mag_world.x = 	pose->data.rotate_matrix[0][0] * *(pose->interface.data.m_x) + 
															pose->data.rotate_matrix[1][0] * *(pose->interface.data.m_y) + 
															pose->data.rotate_matrix[2][0] * *(pose->interface.data.m_z) ;
															
		pose->data.mag_world.y = 	pose->data.rotate_matrix[0][1] * *(pose->interface.data.m_x) + 
															pose->data.rotate_matrix[1][1] * *(pose->interface.data.m_y) + 
															pose->data.rotate_matrix[2][1] * *(pose->interface.data.m_z) ;
															
		pose->data.mag_world.z = 	pose->data.rotate_matrix[0][2] * *(pose->interface.data.m_x) + 
															pose->data.rotate_matrix[1][2] * *(pose->interface.data.m_y) + 
															pose->data.rotate_matrix[2][2] * *(pose->interface.data.m_z) ;
	}
	
	//������������ϵ�µļ��ٶ�ֵ
	pose->data.acc_world.x = 	pose->data.rotate_matrix[0][0] * *(pose->interface.data.a_x) + 
														pose->data.rotate_matrix[1][0] * *(pose->interface.data.a_y) + 
														pose->data.rotate_matrix[2][0] * *(pose->interface.data.a_z) ;
														
	pose->data.acc_world.y = 	pose->data.rotate_matrix[0][1] * *(pose->interface.data.a_x) + 
														pose->data.rotate_matrix[1][1] * *(pose->interface.data.a_y) + 
														pose->data.rotate_matrix[2][1] * *(pose->interface.data.a_z) ;
														
	pose->data.acc_world.z = 	pose->data.rotate_matrix[0][2] * *(pose->interface.data.a_x) + 
														pose->data.rotate_matrix[1][2] * *(pose->interface.data.a_y) + 
														pose->data.rotate_matrix[2][2] * *(pose->interface.data.a_z) ;
	
	//���ŷ����
	pose->data.rol = arctan2(pose->data.rotate_matrix[2][2], pose->data.rotate_matrix[1][2]);						
	pose->data.pit = -arcsin(pose->data.rotate_matrix[0][2]);
	pose->data.yaw = arctan2(pose->data.rotate_matrix[0][0], pose->data.rotate_matrix[0][1]);
	
/////////////////////////////////////////////////////////////////////////////////////////////////			
	//�����������ϵ������ļ��ٶ�--���ܸ����ͷ���Ӱ��
	pose->data.acc_correct.x =   pose->data.acc_world.x * cos(pose->data.yaw) + pose->data.acc_world.y * sin(pose->data.yaw);
	pose->data.acc_correct.y =  -pose->data.acc_world.x * sin(pose->data.yaw) + pose->data.acc_world.y * cos(pose->data.yaw);
	pose->data.acc_correct.z =   pose->data.acc_world.z;
	
	//�����������ϵ������Ĵų�--���ܸ����ͷ���Ӱ��
	if (pose->flag.use_mag == 1)
	{
		XYZ_Data_f ref_v = (XYZ_Data_f){pose->data.rotate_matrix[0][2], pose->data.rotate_matrix[1][2], pose->data.rotate_matrix[2][2]};
		XYZ_Data_f mag_tmp = (XYZ_Data_f){*pose->interface.data.m_x, *pose->interface.data.m_y, *pose->interface.data.m_z};
		
		length = 	sqrt(	*(pose->interface.data.m_x) * *(pose->interface.data.m_x) + 
										*(pose->interface.data.m_y) * *(pose->interface.data.m_y) + 
										*(pose->interface.data.m_z) * *(pose->interface.data.m_z));   
		
//		length *= sqrt(	pose->data.rotate_matrix[0][2] * pose->data.rotate_matrix[0][2] +  
//										pose->data.rotate_matrix[1][2] * pose->data.rotate_matrix[1][2] +  
//										pose->data.rotate_matrix[2][2] * pose->data.rotate_matrix[2][2] );
//		
		if (length != 0)
		{
			simple_3d_trans(&ref_v, &mag_tmp, &pose->data.mag_correct);
//			pose->data.mag_correct.z = pose->data.mag_world.z;
//			pose->data.mag_correct.x = sqrt(1 - (pose->data.mag_correct.z / length) * (pose->data.mag_correct.z / length)) * *(pose->interface.data.m_x);
//			pose->data.mag_correct.y = sqrt(1 - (pose->data.mag_correct.z / length) * (pose->data.mag_correct.z / length)) * *(pose->interface.data.m_y);
		}
	}
	
}
