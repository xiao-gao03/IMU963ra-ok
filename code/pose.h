//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@�ļ���		��pose.h
//@����			����̬����ģ��
#ifndef _POSE_H
#define _POSE_H

#ifndef u8 
#define u8 unsigned char
#endif

#ifndef s8 
#define s8 char
#endif

#ifndef XYZ_Data
#define XYZ_Data
typedef struct XYZ_Data_f
{
	float x;
	float y;
	float z;
}XYZ_Data_f;

typedef struct XYZ_Data_s32
{
	long x;
	long y;
	long z;
}XYZ_Data_s32;

typedef struct XYZ_Data_s16
{
	short x;
	short y;
	short z;
}XYZ_Data_s16;

typedef struct XYZ_Data_s8
{
	s8 x;
	s8 y;
	s8 z;
}XYZ_Data_s8;
#endif

////////////////////////////////////////////////////
typedef struct Pose_Flag
{
	u8 run;
	u8 use_mag;
}Pose_Flag;
////////////////////////////////////////////////////
typedef struct Pose_DInterface
{
	float *a_x;
	float *a_y;
	float *a_z;
	
	float *g_x;
	float *g_y;
	float *g_z;
	
	float *m_x;
	float *m_y;
	float *m_z;
}Pose_DInterface;

typedef struct Pose_Interface
{
	Pose_DInterface data;
}Pose_Interface;
////////////////////////////////////////////////////
typedef struct Pose_Data
{
	float yaw;
	float rol;
	float pit;
	
	float rotate_matrix[3][3];	//��ת����
	
	XYZ_Data_f acc_world;				//��������ϵ�µļ��ٶ�
	XYZ_Data_f mag_world;				//��������ϵ�µĴų�ǿ��	--	ֻ�����˻�λ���йص���
	
	XYZ_Data_f acc_correct;			//��������ϵ�µļ��ٶ�	--	�����˸���������ļ��ٶ�
	XYZ_Data_f mag_correct;			//��������ϵ�µĴų�ǿ��	--	�����˸���������Ĵų�ǿ��
	XYZ_Data_f gyro_correct;		//�ںϼ��ٶȺʹ��������ݣ��������������ֵ
}Pose_Data;
////////////////////////////////////////////////////
typedef struct Pose_Process
{
	float mag_yaw;							//�����Ƶ�ƫ����
	float mag_yaw_bias;					//�����ƽ�����ƫ����ƫ��
	float quaternion[4];				//��Ԫ��
	XYZ_Data_f error;						//�ɼ��ٶȼ����Ч������ƫ��
	XYZ_Data_f error_integral;	//������
}Pose_Process;
////////////////////////////////////////////////////
typedef struct Pose_Parameter
{
	float correct_kp;
	float error_kp;
	float error_ki;
}Pose_Parameter;
////////////////////////////////////////////////////
typedef struct Pose_Module
{
	Pose_Flag flag;
	Pose_Interface interface; 
	Pose_Process process;
	Pose_Data data;
	Pose_Parameter parameter;
}Pose_Module;

//��ʼ���ṹ��
void initPose_Module(Pose_Module *pose);
//������̬
void calculatePose_Module(Pose_Module *pose, float cycle);

#endif

