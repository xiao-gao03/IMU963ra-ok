/*********************************************************************************************************************
* CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB ��Դ���һ����
*
* CYT4BB ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main_cm7_0
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-4       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "pose.h"
//#define q30  1073741824.0f
#define M_PI (3.1415927f)
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// *************************** ����Ӳ������˵�� ***************************
// ʹ����ɿƼ� CMSIS-DAP ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ
// ���� imu963ra
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL/SPC             �鿴 zf_device_imu963ra.h �� imu963ra_SPC_PIN �궨�� Ĭ�� P20_11
//      SDA/DSI             �鿴 zf_device_imu963ra.h �� imu963ra_SDI_PIN �궨�� Ĭ�� P20_14
//      SA0/SDO             �鿴 zf_device_imu963ra.h �� imu963ra_SDO_PIN �궨�� Ĭ�� P20_12
//      CS                  �鿴 zf_device_imu963ra.h �� imu963ra_CS_PIN  �궨�� Ĭ�� P20_13
//      GND                 ��Դ�� GND
//      3V3                 ��Դ 3V3

// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�����Ӻñ��������ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��� ����ϵ� ���� H2 LED ����˸
// 3.������ʹ�� ������� �򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.������ ������� �Ͽ������´�����Ϣ��
//      imu963ra acc data: x-..., y-..., z-...
//      imu963ra gyro data: x-..., y-..., z-...
//      imu963ra mag data: x-..., y-..., z-...
// 5.�ƶ���ת imu963ra �ͻῴ����ֵ�仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************
#define LED1                    (P19_0)                                         // �������� SPI ������ ����궨����д IPS200_TYPE_SPI

Pose_Module pose;

int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();                  // ���Դ�����Ϣ��ʼ��
    
    // �˴���д�û����� ���������ʼ�������
		//IMU963ra_fly_Init();
		//pit_enable(PIT_CH0);
		//IMU_963ra_Init();
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
		
		
		
		float acc_x = 0 , acc_y = 0 , acc_z = -980.0f;
		float gyro_x = 0, gyro_y = 0, gyro_z = 0;
		float mag_x = 0, mag_y = 0, mag_z = 0;
		float pit, rol, yaw;
		
		
				//��ʼ���ṹ��
		initPose_Module(&pose);
		//���ӽӿ�
		pose.interface.data.a_x = &acc_x;
		pose.interface.data.a_y = &acc_y;
		pose.interface.data.a_z = &acc_z;
		pose.interface.data.g_x = &gyro_x;
		pose.interface.data.g_y = &gyro_y;
		pose.interface.data.g_z = &gyro_z;
		pose.interface.data.m_x = &mag_x;
		pose.interface.data.m_y = &mag_y;
		pose.interface.data.m_z = &mag_z;
	
    while(1)
    {
        if(imu963ra_init())
        {
           printf("\r\n imu963ra init error.");                                 // imu963ra ��ʼ��ʧ��
        }
        else
        {
           break;
        }
        gpio_toggle_level(LED1);                                                // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
    }

    // �˴���д�û����� ���������ʼ�������
    while(true)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
      imu963ra_get_acc();                                                     // ��ȡ imu963ra �ļ��ٶȲ�����ֵ
      imu963ra_get_gyro();                                                    // ��ȡ imu963ra �Ľ��ٶȲ�����ֵ
      imu963ra_get_mag();                                                     // ��ȡ IMU963RA �ĵشżƲ�����ֵ
			
			acc_x = imu963ra_acc_transition(imu963ra_acc_x)*100;
			acc_y = imu963ra_acc_transition(imu963ra_acc_y)*100;
			acc_z = imu963ra_acc_transition(imu963ra_acc_z)*100;
			gyro_x = imu963ra_gyro_transition(imu963ra_gyro_x);
			gyro_y = imu963ra_gyro_transition(imu963ra_gyro_y);
			gyro_z = imu963ra_gyro_transition(imu963ra_gyro_z);
			mag_x = imu963ra_mag_transition(imu963ra_mag_x);
			mag_y = imu963ra_mag_transition(imu963ra_mag_y);
			mag_z = imu963ra_mag_transition(imu963ra_mag_z);
			
			calculatePose_Module(&pose, 0.005f);
			
			pit = pose.data.pit;
 			rol = pose.data.rol;
			yaw = pose.data.yaw;
			
			printf("\r\n roll:%f   yaw:%f   pitch:%f \r\n",rol,yaw,pit);
      //system_delay_ms(500);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴 ������� �򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� CMSIS-DAP �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴 ������� ���õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
// ����3��������� imu963ra init error.
//      ���imu963ra�Ľ����Ƿ���ȷ
//      ���imu963ra��ģ���ǲ��ǻ���
//      ���ź��߼���������
// ����4��imu963ra ��ֵ�쳣
//      �����ǲ��������� �����ź��߱���·��
//      ����ģ�鲿������