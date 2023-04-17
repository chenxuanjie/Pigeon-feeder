#ifndef __MAIN_H__
#define __MAIN_H__


//���͵ĵ�n������
#define NORMAL_TRANSMIT			0
#define KEY_TRANSMIT			1
#define SWITCH_TRANSMIT			2
#define ROCKER_TRANSMIT			3
#define ENCODER_TRANSMIT		4	//����ֵΪң�����ϵ�ֵ
#define FEEDTIME1_TRANSMIT		5
#define FEEDTIME2_TRANSMIT		6
#define FEEDTIME3_TRANSMIT		7
//Nrmal_Transmit
#define FEED_OFF				(0x0 + 1)
#define FEED_ON					(0x1 + 1)
#define AUTO_FEED				(0x0 + 3)
#define CHECK					(0xFE)

//���� KEY_TRANSMIT
#define KEY_ROCKER_TRANSMIT		0x1
#define KEY_PIN1_NUM			0x1
#define KEY_PIN2_NUM			(0x1 << 1)
#define KEY_PIN3_NUM			(0x1 << 2)
#define KEY_PIN4_NUM			(0x1 << 3)
#define KEY_PIN5_NUM			(0x1 << 4)
#define KEY_PIN6_NUM			(0x1 << 5)
#define KEY_PIN7_NUM			(0x1 << 6)
#define EmergencyFault		0X00
//ť�ӿ��� SWITCH_TRANSMIT
#define SWITCH2_PIN1_NUM		0x1			//����ť�ӿ��أ�
#define SWITCH3_PIN1_NUM		(0x1 << 1)	//����ť�ӿ��أ�
#define SWITCH3_PIN2_NUM		(0x1 << 2)	//����ť�ӿ��أ�
#define SWITCH3_NONE			(0x1 << 3)	//����ť�ӿ��أ��м�

//ҡ��Ӧ�ò�(FΪ����ZΪ��) 		ROCKER_TRANSMIT
#define UP_1					Y_F1		//1,2,3,4��ʾ�̶ȡ�1Ϊ��С��5���
#define UP_2					Y_F2
#define UP_3					Y_F3
#define UP_4					Y_F4
#define DOWN_1					Y_Z1
#define DOWN_2					Y_Z2
#define DOWN_3					Y_Z3
#define DOWN_4					Y_Z4
#define LEFT_1					X_F1
#define LEFT_2					X_F2
#define LEFT_3					X_F3
#define LEFT_4					X_F4
#define RIGHT_1					X_Z1
#define RIGHT_2					X_Z2
#define RIGHT_3					X_Z3
#define RIGHT_4					X_Z4
#define STOP_X					X_MIDDLE	//Ӧ�ò��ϵ�X
#define STOP_Y					Y_MIDDLE	//Ӧ�ò��ϵ�Y
//�����(FΪ����ZΪ��)
#define X_F1					0x1		//1,2,3,4��ʾ�̶ȡ�1Ϊ��С��5���
#define X_F2					0x2
#define X_F3					0x3
#define X_F4					0x4
#define X_Z1					0x5
#define X_Z2					0x6
#define X_Z3					0x7
#define X_Z4					0x8
#define Y_F1					0x1
#define Y_F2					0x2
#define Y_F3					0x3
#define Y_F4					0x4
#define Y_Z1					0x5
#define Y_Z2					0x6
#define Y_Z3					0x7
#define Y_Z4					0x8
#define X_MIDDLE				0x9
#define Y_MIDDLE				0x9
#define MEDIWM					0x99

typedef enum{
	SETTINGSTATE = 1,
	CONTROLSTATE = 2,
	DEBUGSTATE = 3
}RemoteState;

#endif
